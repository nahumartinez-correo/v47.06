/*$
** MODULENAME - c_calc.c
**
**	$Name: R1V4C13 $
**	$Revision: 1.6 $
**	$Date: 1999/03/11 08:28:43 $
**
**	General 'C' calculation routine.
**		Parms. int argc - number of entries in argv
**		  char *argv[] - pointer to parameters
**		  *argv[0] - routine number
**		  *argv[1] - parm1
**		  *argv[n] - parmn
**		  *argv[n+1] - return string
**		returns - return string in argv[n+1]
**		 - return code (0 = RT_SUCCESS)
**
*/

#include "pch.h"
#include <math.h>

#undef OVERFLOW
#undef UNDERFLOW
#include <typedefs.h>
#include <rtdef.h>
#include <csrdef.h>
#include <convers.h>
#include <defines.h>
#include <ofbdefs.h>
#include <rtprotos.h>


	static int c_calc_convert(int nparms, int argc, struct RTOBJ *argv[]);
	static void clc_11(double ipr, double iamt, double icomp, double interest, 
					double ind, double yr, double ibe, double *fv);
	static void clc_12(double icomp, double ifv, double interest, 
					double ipayyear, double yr, double *aod);
	static void clc_16(double icomp, double interest, double ipv, 
					double yr, double *fv);
	static void clc_22(double ifv, double interest, double numwith, 
					double yr, double freq, double *aow);
	static void clc_25(double iamt, double icomp, double interest, 
					double iwithval, double over_three, double *pt);
	static void clc_26(double iamt, double icomp, double interest, 
					double iwithval, double yr_mat, double yr_with,
					 double over_three, double over_yr, double *pt);
	static void clc_27(double iamt, double mv, double interest, 
					double months, double over_three, double over_yr, double *pt);
	static void clc_28(double iamt, double interest, double yr_mat, double *pt);
	static void clc_30(double icomp, double fv, double interest, 
					double yr, double *pv);
	static void clc_31(double icomp, double intearned, double ipv, 
					double yr, double *ir);
	static void clc_32(double icomp, double intearned, double interest, 
					double ipv, double *tln);
	static void clc_36(double icomp, double interest, double ipv, 
					double yr, double *ie);
	static void clc_37(double icomp, double interest, double *ay);
	static void clc_100(double interest, double amt_fin, double term, 
					double freq, double ins, double day, 
					double oday, double *rp);
	static void clc_101(double amt_fin, double reg, double interest, 
					double freq, double total, double *tm);
	static void clc_102(double amt_fin, double reg, double interest, 
					double freq, double term, double *fv);
	static void clc_103(double r, double t, double f, double a, double p,double n,
					double freq, double *apr);
	static void clc_104(double interest, double amt_fin, double term, 
					double bal, double day, double *rp);
	static void clc_106(double interest, double term, double paynum, 
					double amt, double *rp);
	static void clc_108(double interest, double rpay, double paynum, 
					double term, double *la);
	static void clc_109(double p, double t, double f, double amt, 
					double freq, double *apr);

	static void clc_110 ( double interest, double amt_fin, double term, 
					 double bal, double odf, double rr, double lr,
					 double dr, double *rp);
	static void clc_111 ( double interest, double amt_fin, double payment,
					 double term, double odf, double lr, double ins,
					 double *rp);
	static void clc_112 ( double interest, double amt_fin, double term,
					 double amort, double odf, double ins, double lr,
					 double *rp);
	static void clc_113 ( double interest, double amt_fin, double term,
					 double amort, double odf, double ins, double lr,
					 double *rp);

#define FIX_PARM 1  /* number of fixed parms  0 = RTN  */

typedef void (*PFI)(); /* pointer to a function */
#define NULLPFI ((PFI)0)
typedef struct calc_table
{
	int	rtn_num;
	int inparms;
	PFI rtn;
} CALC;
CALC ct[] =
{
   11, 7, clc_11,	// Solve for future value yearly deposits
   12, 5, clc_12,	// Solve for Amount of Regular deposit
   16, 4, clc_16,	// Solve for future value
   22, 5, clc_22,	// Solve for Amount of regular withdrawals
   25, 5, clc_25,	// Solve for CD penalty for certficates
                    //   purchased pior to 7-1-89
   26, 8, clc_26,	// Solve for CD penalty for certficates
                    //   purchased after 6-30-79 and prior to 6-2-80
   27, 6, clc_27,	// Solve for CD penalty for certficates
                    //   purchased after 6-01-80 and prior to 10-1-83
   28, 3, clc_28,	// Solve for CD penalty for certficates
                    //   purchased after 9-30-83
   30, 4, clc_30,	// Solve for present value
   31, 4, clc_31,	// Solve for interest rate
   32, 4, clc_32,	// Solve for term length
   36, 4, clc_36,	// Solve for interest earned
   37, 2, clc_37,	// Solve for annual yield
/*  *************** LOAN CALC'S */
  100, 7, clc_100,	// Solve for regular payments
  101, 5, clc_101,	// Solve for term
  102, 5, clc_102,	// Solve for future value
  103, 7, clc_103,	// Solve for A.P.R.

  104, 5, clc_104,	// Solve for regular payments WITH BALLON no ins
  106, 4, clc_106,	// Solve for ESTIMATED reg. payments
  108, 4, clc_108,	// Solve for ESTIMATED loan amount
  109, 5, clc_109,	// Solve for Bi-Weekly A.P.R.

  110, 8, clc_110,	// Solve for regular payments WITH BALLON
  111, 7, clc_111,	// Solve for Ballon with reg payments
  112, 7, clc_112,	// Solve for regular payments with amort
  113, 7, clc_113,	// Solve for ballon payment with amort
    0, 0, NULLPFI,  // END OF TABLE
};
double pow(),log();
static double pd[10], half = 0.50, quart = 0.25;
static int	req_args,error_math;

/*$
**	Function:	c_calc
**
*/

XDLL int
c_calc(int argc, struct RTOBJ *argv[])
{
	int		rc,rtn,cnt;
	field	*fp;
	
	rc = VAL_PASS;
	error_math = 0;
	if (argc < FIX_PARM) {
		rterrorbox("CSR:c_calc %d: must have at least %d argument(s)",
					CSR_FAIL_ARGCOUNT,FIX_PARM);
		return(CSR_FAIL_ARGCOUNT);
		}
	if (rtobj2int(argv[0], &rtn) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);

	cnt = 0;
	while (ct[cnt].rtn_num != rtn) {
		if (ct[cnt].rtn_num == 0) {
			rterrorbox( "CSR:c_calc called with invalid routine number %d"
						" and %d arguments",rtn,argc);
			return(CSR_FAIL_ARGTYPE);
			}
		cnt++;
		}
	rc = c_calc_convert(ct[cnt].inparms,argc,argv);
	if (rc == VAL_PASS) {
		switch (ct[cnt].inparms) {
		case 1: (ct[cnt].rtn) (pd[0],&pd[1]);
				break;
		case 2: (ct[cnt].rtn) (pd[0],pd[1],&pd[2]);
				break;
		case 3: (ct[cnt].rtn) (pd[0],pd[1],pd[2],&pd[3]);
				break;
		case 4: (ct[cnt].rtn) (pd[0],pd[1],pd[2],pd[3],&pd[4]);
				break;
		case 5: (ct[cnt].rtn) (pd[0],pd[1],pd[2],pd[3],pd[4],&pd[5]);
				break;
		case 6: (ct[cnt].rtn) (pd[0],pd[1],pd[2],pd[3],pd[4],pd[5],&pd[6]);
				break;
		case 7: (ct[cnt].rtn)(pd[0],pd[1],pd[2],pd[3],pd[4],pd[5],pd[6],&pd[7]);
				break;
		case 8: (ct[cnt].rtn) (pd[0],pd[1],pd[2],pd[3],pd[4],pd[5],pd[6],
					pd[7],&pd[8]);
				break;
		case 9: (ct[cnt].rtn) (pd[0],pd[1],pd[2],pd[3],pd[4],pd[5],pd[6],
					pd[7],pd[8],&pd[9]);
				break;
		}
		if (error_math != 0)
		{
			return(VAL_FAIL_W_MSG);
		}
		fp = (field *) argv[FIX_PARM+ct[cnt].inparms]->ptr;
		if (fp->flags.type == FT_FIXED) {
			char temp[MAXDECWORK];
			/* Can't assume target field will be big enough... */
			rc = float2decwork(pd[ct[cnt].inparms],(decwork *)temp);
			if (rc != RT_ERROR)
				rc = decwork2field((decwork *)temp,fp);
			if (rc == RT_ERROR)
				return(VAL_FAIL_W_MSG);
			}
		else {
			rterrorbox("CSR:c_calc %d: target field must be of type FIXED",rtn);
			return(CSR_FAIL_ARGTYPE);
			}
		}
	else if (rc == CSR_FAIL_ARGCOUNT)
		rterrorbox("CSR:c_calc %d: got %d arguments, expected %d",
					rtn,argc,req_args);
	else if (rc == CSR_FAIL_ARGTYPE)
		rterrorbox("CSR:c_calc %d: bad argument type, cannot evaluate",rtn);
	return(rc);
}

/***********************************************************************/
/*  c_calc_convert                                                     */
/*        General c_calc routine to convert strings to double          */
/*        parms:  nparm - number of parms to convert                   */
/*                argc  - number of parms passed to c_calc             */
/*                argv  - pointer to parms                             */
/*        returns: pln contains double                                 */
/*                 return value 0 = correct number of parms            */
/*                 return value -1 = incorrect number of parms         */
/***********************************************************************/
static int
c_calc_convert(int nparms,int argc,struct RTOBJ *argv[])
{
	int		ar, pn;
	char	decbuf[MAXDECWORK];
	decwork	*fldvalue;


/* check for total parms(argc) - FIX_PARM - 1(return value) */
	req_args = nparms + FIX_PARM + 1;		/* set this up every time */
	if (argc-FIX_PARM-1 != nparms) {
		return(CSR_FAIL_ARGCOUNT);
		}

	ar = FIX_PARM;
	pn = 0;
	fldvalue = (decwork *)decbuf;
	while (pn < nparms) {
		/* first, turn the argument into a decwork	*/
		if (rtobj2decwork(argv[ar],fldvalue))
			return(CSR_FAIL_ARGTYPE);
		/* now, translate the decwork to a float	*/
		if (decwork2float(fldvalue, &pd[pn]))
			return(CSR_FAIL_ARGTYPE);
		ar++;
		pn++;
		}
	return(VAL_PASS);
}


/***********************************************************************/
/*  clc_11                                                             */
/*        Solve for future value with yearly deposits                  */
/*        parms:  ipr      - Principle                                 */
/*                iamt     - Amount of deposit                         */
/*                icomp    - Compounding frequency                     */
/*                interest - Interest rate                             */
/*                ind      - Number of deposits per year               */
/*                yr       - Term length                               */
/*                ibe      - begin or end                              */
/*                *fv      - pointer to return value (future value)    */
/*                                                                     */
/***********************************************************************/
static void clc_11 (ipr,iamt,icomp,interest,ind,yr,ibe,fv)
	double ipr,iamt,icomp,interest,ind,yr,ibe,*fv;
{
	double tx,ty,tf;

	yr = yr * ind;
	tx = pow((1+(interest/100)/icomp),icomp) - 1;
	if (tx > HUGE_VAL)
	{
		error_math = 1;
		*fv = 0;
		return;
	}
	ty = tx/ind;
	tf = -( (ipr+ (iamt/ty) * (1+ty*ibe) * (1-pow((1+ty),-yr))) / pow((1+ty),-yr));
	if (tf > HUGE_VAL)
	{
		error_math = 1;
		*fv = 0;
		return;
	}
	if (tf < 0)
	{
		*fv = 0 - tf;
	}
	else
	{
		*fv = tf;
	}
}

/***********************************************************************/
/*  clc_12                                                             */
/*        Solve for Amount of Regular deposit                          */
/*        parms:  icomp    - Compounding frequency                     */
/*                ifv      - Future value                              */
/*                interest - Interest rate                             */
/*                ipayyear - Number of deposits per year               */
/*                yr       - Term length                               */
/*                *aod     - pointer to return value (amount of deposit)*/
/*                                                                     */
/***********************************************************************/
static void clc_12(icomp,ifv,interest,ipayyear,yr,aod)
	double icomp,ifv,interest,ipayyear,yr,*aod;
{
	double t1,t2,t3,t4,t5,t6;

	t1 = 1+((interest/100)/icomp);
	t2 = (icomp/ipayyear) - 1;
	t3 = pow (t1,t2);
	t4 = (pow(t1,icomp/ipayyear));
	t5 = (pow (t1,(yr*icomp))) - 1;
	t6 = t4 * t5;
	*aod = ifv * (t3/t6);
}
/***********************************************************************/
/*  clc_16                                                             */
/*        Solve for Furture value                                      */
/*        parms:  icomp    - Compounding frequency                     */
/*                interest - Interest rate                             */
/*                ipv      - Account balance                           */
/*                yr       - year                                      */
/*                *fv      - pointer to return value                   */
/*                                                                     */
/***********************************************************************/
static void clc_16(icomp,interest,ipv,yr,fv)
	double icomp,interest,ipv,yr,*fv;
{
	double x1;

	x1 = pow ((1+((interest/100)/icomp)),(icomp*yr));
	if (x1 > HUGE_VAL)
	{
		error_math = 1;
		*fv = 0;
		return;
	}
	*fv = ipv * x1;
}

/***********************************************************************/
/*  clc_22                                                             */
/*        Solve for Amount of regular withdrawals                      */
/*        parms:  ifv      - Account balance ar retirement             */
/*                interest - interest rate                             */
/*                numwith  - Number of withdrawals per year            */
/*                yr       - DEPLETIONAGE - RETIREAGE                  */
/*                freq     - compounding freq                          */
/*                *aow      - pointer to return value (amount of withdrawals*/
/*                                                                     */
/***********************************************************************/
static void clc_22(ifv,interest,numwith,yr,freq,aow)
	double ifv,interest,numwith,yr,freq,*aow;
{
	double rate,t1,t2,t3;

	rate = ( pow((1+(interest/100)/freq),freq) - 1);
	t1 = rate / numwith;
	t2 = pow( (1+t1) ,(numwith*yr));
	if (t2 > HUGE_VAL)
	{
		error_math = 1;
		*aow = 0;
		return;
	}
	t3 = (t1 / (t2 - 1)) + t1;
	*aow = ifv * t3;
	
}

/***********************************************************************/
/*  clc_25                                                             */
/*        Solve for CD penalty for certficates                         */
/*                         purchased pior to 7-1-89                    */
/*        parms:  iamt     - Amount of CD                              */
/*                icomp    - Compounding frequency                     */
/*                interest - Interest rate                             */
/*                iwithval - futrue value                              */
/*                over_three  - term to withdrawal                     */
/*                *pt      - pointer to return value (penalty)         */
/*                                                                     */
/***********************************************************************/
static void
clc_25(iamt,icomp,interest,iwithval,over_three,pt)
	double iamt,icomp,interest,iwithval,over_three,*pt;
{
	double f1;

	f1 = (double) 0;
	if (over_three == 1)
	{
		f1 = quart;
	}
	*pt = iwithval * ((pow( (1+((interest/100)/icomp)) , icomp*f1 )) - 1);
}

/***********************************************************************/
/*  clc_26                                                             */
/*        Solve for CD penalty for certficates                         */
/*           purchased after 6-30-79 and prior to 6-2-80               */
/*        parms:  iamt     - Amount of CD                              */
/*                icomp    - Compounding frequency                     */
/*                interest - Interest rate                             */
/*                iwithval - futrue value                              */
/*                yr_mat   - term to maturity                          */
/*                yr_with  - term to withdrawal                        */
/*                over_three  -  0 if less than 3 months               */
/*                over_yr  -  0 if less than 1 year                    */
/*                *pt      - pointer to return value (penalty)         */
/*                                                                     */
/***********************************************************************/
static void
clc_26(iamt,icomp,interest,iwithval,yr_mat,yr_with,over_three,over_yr,pt)
	double iamt,icomp,interest,iwithval,yr_mat,yr_with,over_three,over_yr,*pt;
{
	double f1;

	f1 = (double) 0;
	if (over_yr == 0)
	{
		if (over_three == 1)
		{
			f1 = quart;
		}
	}
	else
	{
		if (over_three == 2)
		{
			f1 = half;
		}
	}
	*pt = (iwithval * pow(1 + ((interest/100)/icomp),icomp*f1)) - iwithval;
}

/***********************************************************************/
/*  clc_27                                                             */
/*        Solve for CD penalty for certficates                         */
/*                         purchased after 6-01-80 and prior to 10-1-83*/
/*        parms:  iamt     - Amount of CD                              */
/*                mv       - Market value                              */
/*                interest - Interest rate                             */
/*                months  -  term to maturity                          */
/*                over_three  -  0 if less than 3 months               */
/*                over_yr  -  0 if less than 1 year                    */
/*                *pt      - pointer to return value (penalty)         */
/*                                                                     */
/***********************************************************************/
static void
clc_27(iamt,mv,interest,months,over_three,over_yr,pt)
	double iamt,mv,interest,months,over_three,over_yr,*pt;
{
	double t1;
	if (over_yr == 1)
	{
		t1 = half;
	}
	else
	{
		if (over_three == 1)
		{
			t1 = quart;
		}
		else
		{
			t1 = (months/12);
		}
	}
	*pt = (interest/100) * iamt * t1;
}

/***********************************************************************/
/*  clc_28                                                             */
/*        Solve for CD penalty for certficates                         */
/*                         purchased after 9-30-83                     */
/*        parms:  iamt     - Amount of CD                              */
/*                interest - Interest rate                             */
/*                over_yr  - 0 if <= year                              */
/*                *pt      - pointer to return value (penalty)         */
/*                                                                     */
/***********************************************************************/
static void
clc_28(iamt,interest,over_yr,pt)
	double iamt,interest,over_yr,*pt;
{
	double t1,t2;

	t2 = 365;
	if (over_yr == 0)
	{
		t1 = 31;
	}
	else
	{
		t1 = 90;
	}
	*pt = iamt * (interest/100) * t1/t2;
}

/***********************************************************************/
/*  clc_30                                                             */
/*        Solve for present value                                      */
/*        parms:  icomp    - Compounding frequency                     */
/*                fv       - furture value                             */
/*                interest - Interest rate                             */
/*                yr       - term length                               */
/*                *pv      - pointer to return value (present value)   */
/*                                                                     */
/***********************************************************************/
static void clc_30(icomp,fv,interest,yr,pv)
	double icomp,fv,interest,yr,*pv;
{
	double t1,t2;

	t1 =  1 + ((interest/100) / icomp);
	t2 = -icomp * yr;
	*pv = fv * pow( t1, t2);
}

/***********************************************************************/
/*  clc_31                                                             */
/*   Solve for interest rate                                           */
/*        parms:  icomp    - Compounding frequency                     */
/*                intearned- Interest earned                           */
/*                ipv      - amount of deposit                         */
/*                yr       - term length                               */
/*                *ir      - pointer to return value (interest rate)   */
/*                                                                     */
/***********************************************************************/
static void clc_31(icomp,intearned,ipv,yr,ir)
	double icomp,intearned,ipv,yr,*ir;
{
	double t1,t2,t3,t4;

	t1 = (intearned + ipv) / ipv;
	t2 = 1 / icomp;
	t3 = pow (t1, t2);
	t4 = ((icomp * t3) - icomp) * 100;
	*ir = t4 / yr;
}

/***********************************************************************/
/*  clc_32                                                             */
/*        Solve for term length                                        */
/*        parms:  icomp    - Compounding frequency                     */
/*                intearned- Interest earned                           */
/*                interest - Interest rate                             */
/*                ipv      - amount of deposit                         */
/*                *tln     - pointer to return value (term length in days)*/
/*                                                                     */
/***********************************************************************/
static void clc_32(icomp,intearned,interest,ipv,tln)
	double icomp,intearned,interest,ipv,*tln;
{
	double t1,t2;

	t1 = log ((intearned+ipv)/ipv);
	t2 = (interest/100) / icomp;
	*tln = t1/t2;
}

/***********************************************************************/
/*  clc_36                                                             */
/*        Solve for interest earned                                    */
/*        parms:  icomp    - Compounding frequency                     */
/*                interest - Interest rate                             */
/*                ipv      - Amount of deposit                         */
/*                yr       - term length                               */
/*                *ie      - pointer to return value (interest earned) */
/*                                                                     */
/***********************************************************************/
static void clc_36(icomp,interest,ipv,yr,ie)
	double icomp,interest,ipv,yr,*ie;
{
	double t1,t2;

	t1 = (1 + ((interest/100)/icomp));
	t2 = icomp * yr;
	*ie = (ipv * pow (t1,t2)) - ipv;
}
/***********************************************************************/
/*  clc_37                                                             */
/*        Solve for annual yield                                       */
/*        parms:  icomp    - Compounding frequency                     */
/*                interest - Interest rate                             */
/*                *ay      - pointer to return value (annual yield)    */
/*                                                                     */
/***********************************************************************/
static void clc_37(icomp,interest,ay)
	double icomp,interest,*ay;
{
	*ay = 100 * ( pow((1+(interest/100)/icomp),icomp) - 1);
}

/***********************************************************************/
/*  clc_100                                                            */
/*        Solve for reg payments                                       */
/*        parms:  interest - Interest rate                             */
/*                amt_fin  - Amount financed                           */
/*                term     - length of loan                            */
/*                freq     - payment freq                              */
/*                ins      - insurance                                 */
/*                day      - days                                      */
/*                oday     - odd days                                  */
/*                *rp      - pointer to return value (reg payments)    */
/*                                                                     */
/***********************************************************************/
static void clc_100(interest,amt_fin,term,freq,ins,day,oday,rp)
	double interest,amt_fin,term,freq,ins,day,oday,*rp;
{
	double inter,top,bot,insur,r;

	inter = interest / 100;
	inter = inter / freq;

	top = ((inter * term) * pow((1 + inter),day)) * (1 + (inter * oday));
	bot = ((1 + inter) - pow((1 + inter),-(term-1)));
	insur = 1 - (ins * (top/bot));
	r = ((amt_fin/insur) * (top/bot)) / term;
	if (r < 0)
	{
		*rp = 0 - r;
	}
	else
	{
		*rp = r;
	}

}

/***********************************************************************/
/*  clc_101                                                            */
/*        Solve for BI WEEKLY term                                     */
/*        parms:  amt_fin  - Amount financed                           */
/*                reg      - regular payment amount                    */
/*                interest - Interest rate                             */
/*                freq     - payment freq                              */
/*                total    - total payment                             */
/*                *tm      - pointer to return value (term)            */
/*                                                                     */
/***********************************************************************/
static void clc_101(amt_fin,reg,interest,freq,total,tm)
	double amt_fin,reg,interest,freq,total,*tm;
{
	double inter,b;

	inter = interest / 100;
	inter = inter / freq;

	reg = -reg;
	b = 0; /* 1 = Annuity Immediate 0 = Annuity Due */
	b = 1 + inter * b;
	*tm = -(log(-(amt_fin+b*reg/inter) / (total-b*reg/inter)) / log(1 + inter));
}
/***********************************************************************/
/*  clc_102                                                            */
/*        Solve for future value                                       */
/*        parms:  amt_fin  - Amount financed                           */
/*                reg      - regular payment amount                    */
/*                interest - Interest rate                             */
/*                freq     - payment freq                              */
/*                term     - payment term                              */
/*                *fv      - pointer to return value (future value)    */
/*                                                                     */
/***********************************************************************/
static void clc_102(amt_fin,reg,interest,freq,term,fv)
	double amt_fin,reg,interest,freq,term,*fv;
{
	double inter,b;

	inter = interest / 100;
	inter = inter / freq;

	reg = -reg;
	b = 0; /* 1 = Annuity Immediate 0 = Annuity Due */
	*fv = -( (amt_fin + (reg/inter) * (1+inter*b) * (1-pow(1+inter,-term)) ) / pow(1+inter,-term));
}

/***********************************************************************/
/*  clc_103                                                            */
/*        Solve for A.P.R.                                             */
/*        parms:  r        - interest rate                             */
/*                t        - days                                      */
/*                f        - odd days                                  */
/*                amt      - Amount financed                           */
/*                p        - regular payment amount                    */
/*                t        - term (months)                             */
/*                freq     - freq                                      */
/*                *apr      - pointer to return value (apr)             */
/*                                                                     */
/***********************************************************************/
static void clc_103(r,t,f,a,p, n , freq,apr)
	double r,t,f,a,p,n,freq,*apr;
{
	double q,u,xa,ya,pa,j,ia,s,xd;
	int xx;

	q = 1;
	u = 1;
	j = r;
	s = 0;
top1: 
	j = j + q;
	ia = j / (freq * 100);
	xa = (ia * n * pow((1 + ia), t)) * (1 + (ia * f)) / ((1 + ia) - (pow((1 + ia),-(n - 1))));
	ya = 1 - (s * xa);
	pa = ((a / ya) * xa) / n;
	xx = (int)((pa * 100) + .5);
	xd = xx;
	pa = xd / 100;

	if (p > pa)
	{
		goto top1;
	}
	if (pa > p)
	{
		q = u;
	}
	j = j - q;
	u = u * .1;
	q = u;
	if (u < .001)
	{
	    goto done_it;
	}
	else
	{
	    goto top1;
	}
done_it:
	*apr = j;
}

/***********************************************************************/
/*  clc_104                                                            */
/*        Solve for reg payments WITH BALLON                           */
/*        parms:  interest - Interest rate                             */
/*                amt_fin  - Amount financed                           */
/*                term     - length of loan                            */
/*                bal      - ballon                                    */
/*                day      - days to 1st payment                       */
/*                *rp      - pointer to return value (reg payments)    */
/*                                                                     */
/***********************************************************************/
static void clc_104(interest,amt_fin,term,bal,day,rp)
	double interest,amt_fin,term,bal,day,*rp;
{
	double f,q,v,inter,pw1,pw2,pw3,dval;

	inter = interest / 1200;

	dval = -1;
	if (day == 0)
	{
		f = 1;
	}
	else
	{
		f = pow((1 + ((interest/36000) * day)),dval);
	}
	if (bal == 0)
	{
		v = 0;
	}
	else
	{
		v = bal / pow((1+inter),term) * f;
		term = term - 1;
	}
	q = amt_fin - v;

	pw1 = pow((1+inter),term);
	pw2 = 1 - pow(pw1,dval);
	pw3 = (pw2 / inter) * f;
	*rp = q * pow(pw3,dval);

}


/***********************************************************************/
/*  clc_106                                                            */
/*        Solve for ESTIMATED reg. payments                            */
/*        parms:  interest - Interest rate                             */
/*                term      - term in years                            */
/*                paynum    - payments per year                        */
/*                amt       - loan amount                              */
/*                *rp       - pointer to return value (rp)             */
/*                                                                     */
/***********************************************************************/
static void clc_106(interest,term,paynum,amt,rp)
	double interest,term,paynum,amt,*rp;
{
	double inter,top,bot;

	inter = interest / 100;
	top = amt * inter / paynum;
	bot = 1 - pow( (1 + inter/paynum),(- term * paynum));
	*rp = top / bot;
}

/***********************************************************************/
/*  clc_108                                                            */
/*        Solve for ESTIMATED loan amount                              */
/*        parms:  interest - Interest rate                             */
/*                rpay      - reg. payments                            */
/*                paynum    - payments per year                        */
/*                term      - term in years                            */
/*                *la       - pointer to return value (la)             */
/*                                                                     */
/***********************************************************************/
static void clc_108(interest,rpay,paynum,term,la)
	double interest,rpay,paynum,term,*la;
{
	double inter,top,bot;

	inter = interest / 100;
	top = 1 - pow( (1 + inter/paynum), ( - term * paynum));
	bot = inter/paynum;
	*la = rpay * (top / bot);
}
/***********************************************************************/
/*  clc_109                                                            */
/*        Solve for A.P.R.                                             */
/*        parms:  p        - regular payment amount                    */
/*                t        - term (months)                             */
/*                f        - future value                              */
/*                amt      - Amount financed                           */
/*                freq     - freq                                      */
/*                *apr      - pointer to return value (apr)             */
/*                                                                     */
/***********************************************************************/
static void clc_109(p,t,f,amt,freq,apr)
	double p,t,f,amt,freq,*apr;
{
	double a,b,c,d,x,y,w,top,bot,percis,powx;
	int	iw;
#define FOREVER for (;;)

	a = percis = .00001;
	d = 100;
	b = 0; /* 1 = Annuity Immediate 0 = Annuity Due */
	p = -p;
	FOREVER
	{
		c = (a + d) / 2;
		if (d - c < percis)
		{
			break;
		}
		powx = 1 - pow((1+d),-t);
		top = p * (1+d*b) * powx;
		bot = f * pow((1+d),-t);
		x = top /d + bot + amt;

		powx = 1 - pow((1+c),-t);
		top = p * (1+c*b) * powx;
		bot = f * pow((1+c),-t);
		y = top /c + bot + amt;

		if (x * y < 0.0e0)
		{
			a = c;
		}
		else
		{
			d = c;
		}
	}
	w = c * 100;
	iw = (int)((w + .00005) * 10000);

	*apr = (iw / 10000.0e0) * freq;
}

/***********************************************************************/
/*  clc_110                                                            */
/*        Solve for reg payments WITH BALLON                           */
/*        parms:  interest - Interest rate                             */
/*                amt_fin  - Amount financed                           */
/*                term     - length of loan                            */
/*                bal      - ballon                                    */
/*                odf      -                                           */
/*                rr       - reducing life                             */
/*                lr       - level life                                */
/*                dr       - dis ins                                   */
/*                *rp      - pointer to return value (reg payments)    */
/*                                                                     */
/***********************************************************************/
static void clc_110 (interest,amt_fin,term,bal,odf,rf,lf,dr,rp)
	double interest,amt_fin,term,bal,odf,rf,lf,dr,*rp;
{
	double inter,an,x,y,z,tpow;

	inter = interest;
    an = (1 - pow((1 + inter),(0 - (term - 1)))) / inter;
	tpow = pow((1 + inter),term);
    x = (amt_fin * odf * tpow + (bal * (lf * odf * tpow - 1)));
	y = (an * tpow) - ((term - 1) * (rf + dr) * tpow * odf);
	z = (x / y);
	if (z < 0)
	{
		*rp = 0 - z;
	}
	else
	{
		*rp = z;
	}
}

/***********************************************************************/
/*  clc_111                                                            */
/*        Solve for reg payments WITH payment                          */
/*        parms:  interest - Interest rate                             */
/*                amt_fin  - Amount financed                           */
/*                payment  - payment                                   */
/*                term     - length of loan                            */
/*                odf      -                                           */
/*                lr       - level life                                */
/*                ins      - insurance                                 */
/*                *rp      - pointer to return value (reg payments)    */
/*                                                                     */
/***********************************************************************/
static void clc_111 (interest,amt_fin,payment,term,odf,lf,ins,rp)
	double interest,amt_fin,payment,term,odf,lf,ins,*rp;
{
	double inter,an,w,x,y,z,prem,tpow,r;

	inter = interest;
    an = (1 - pow((1 + inter),(0 - (term - 1)))) / inter;
	tpow = pow((1 + inter),term);

	w = (amt_fin * odf * tpow * lf);
	x = (payment * (term - 1) * ins);
	y = (payment * an * tpow * lf);
	z = 1 - (odf * tpow * lf);
	prem = ((w+x-y)/z);
	r = ((amt_fin + prem) * odf - payment * an) * tpow;
	*rp = r;
}

/***********************************************************************/
/*  clc_112                                                            */
/*        Solve for reg payments   WITH amort                          */
/*        parms:  interest - Interest rate                             */
/*                amt_fin  - Amount financed                           */
/*                term     - length of loan                            */
/*                amort    - amort period                              */
/*                odf      -                                           */
/*                ins      - insurance                                 */
/*                lr       - level life                                */
/*                *rp      - pointer to return value (reg payments)    */
/*                                                                     */
/***********************************************************************/
static void clc_112 (interest,amt_fin,term,amort,odf,ins,lf,rp)
	double interest,amt_fin,term,amort,odf,ins,lf,*rp;
{
	double inter,an,am,x,y,z,prem,r;

	inter = interest;
    an = (1 - pow((1 + inter),(0 - (term - 1)))) / inter;
    am = (1 - pow((1 + inter),(0 - amort))) / inter;

    x = ((term - 1) * ins / am) + (pow((1 + inter),term) * lf * (1 - (an/am)));
    y = amt_fin * odf * x;
    z = 1 - odf * x;
    prem = y / z;

    r = (((amt_fin + prem) * odf) / am);
	if (r < 0)
	{
		*rp = 0 - r;
	}
	else
	{
		*rp = r;
	}
}

/***********************************************************************/
/*  clc_113                                                            */
/*        Solve for ballon payment        WITH amort                   */
/*        parms:  interest - Interest rate                             */
/*                amt_fin  - Amount financed                           */
/*                term     - length of loan                            */
/*                amort    - amort period                              */
/*                odf      -                                           */
/*                ins      - insurance                                 */
/*                lr       - level life                                */
/*                *rp      - pointer to return value (reg payments)    */
/*                                                                     */
/***********************************************************************/
static void clc_113 (interest,amt_fin,term,amort,odf,ins,lf,rp)
   	double interest,amt_fin,term,amort,odf,ins,lf,*rp;
{
	double inter,an,am,x,y,z,prem,pmt,tpow,r;

	inter = interest;
    an = (1 - pow((1 + inter),(0 - (term - 1)))) / inter;
    am = (1 - pow((1 + inter),(0 - amort))) / inter;
	tpow = pow((1 + inter),term);

    x = ((term - 1) * ins / am) + (tpow * lf * (1 - (an/am)));
    y = amt_fin * odf * x;
    z = 1 - odf * x;
    prem = y / z;

    pmt = (((amt_fin + prem) * odf) / am);
    r = (((amt_fin + prem) * odf) - (pmt * an)) * tpow;
	if (r < 0)
	{
		*rp = 0 - r;
	}
	else
	{
		*rp = r;
	}
}
