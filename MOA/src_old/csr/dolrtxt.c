/*$
** MODULENAME - dolrtxt.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 $
** $Revision: 1.9 $
** $Date: 1997/11/19 21:56:34 $
**
** Synopsis:
**   
**
** External Function List:
**
** Notes:
**		
**  (c) Copyright by Olivetti NA, 1996
**        All rights reserved.
*/

#include	"pch.h"

#include	<defines.h>
#include	<typedefs.h>
#include	<ppdefs.h>
#include	<rtprotos.h>
#include	<rtdef.h>
#include	<csrdef.h>
#include	<decmath.h>
#include	<convers.h>

#define	TEXT_FORMAT			0	/* everything is OK			*/
#define	NUMERIC_FORMAT		1	/* indicates a 'soft' error	*/

#define	OUT_OF_RANGE			-1	/* hard errors				*/
#define	BUFFER_TOO_SMALL		-2
#define	STRING_TOO_SMALL		-3

#define	FIRST_LINE				'1'
#define	SECOND_LINE				'2'

#define ENGLISH					1
#define SPANISH					2
#define S_CENT				" CENTAVO"
#define S_CENTS				"S"
#define S_HUNDRED			"CIEN"
#define S_THOUSAND			" MIL*"			
#define S_MILLON			" MILLON*"			/* Singular */
#define S_MILLONES			" MILLONES*"		/* Plural  */

typedef union {
	char	filler[MAXDECWORK];
	decwork	number;
	} decno;

static int spanish = 0;

static char one_cent[5]			= {1, 2, 2, 0, 1};
static decwork	dolr_zero		= {1, 1, 0, '\0'};
static decwork	dolr_one		= {1, 1, 0, '\1'};
static decwork	dolr_two		= {1, 1, 0, '\2'};
static decno	dolr_max;
#define	MAXDIGITS	17		/*  999,999,999,999,999.99	*/
static field	*dollarresult;

/* internal function prototypes	*/
int			dollar_to_alpha(int, rtobj **);
static void	dtoa_bld_number(int, char *, char *);
static void	dollar_to_string(char *, int, char *);
static void	dtoa_fill(char *, int, char, char);
static void	dollar_init();
static void	dollar_error();
static void	init_spanish();
static void init_english();

static char	*dollarpat;
static char **digit_names;
static char **ten_names;
static char **teen_names;
static char **twenty_names;
static char **hundred_names;
static char *minus_name;
static char *no_name;
static char *trillion_name;
static char *billion_name;
static char *million_name;
static char *thousand_name;
static char *upper_and;
static char *lower_and;
static char *dollar_name;
static char *dollars_name;
static char *hundred_name;
static char *tens_separator;

static char		*dollarpat_e = "-$ZZZ,ZZZ,ZZZ,ZZZ,ZZ9.99";
static char	*digit_enames[] =
{
	"",		"ONE",	"TWO",	"THREE",	"FOUR",
	"FIVE",	"SIX",	"SEVEN","EIGHT",	"NINE"
};
static char *ten_enames[] =
{
	"",			"",		"TWENTY",	"THIRTY",	"FORTY",
	"FIFTY",	"SIXTY","SEVENTY",	"EIGHTY",	"NINETY"
};
static char *teen_enames[] =
{
	"TEN",		"ELEVEN",	"TWELVE",	"THIRTEEN",	"FOURTEEN",
	"FIFTEEN",	"SIXTEEN",	"SEVENTEEN","EIGHTEEN",	"NINETEEN"
};
static char minus_ename[] = "MINUS ";
static char no_ename[] = "NO ";
static char trillion_ename[] = " TRILLION*";
static char billion_ename[] = " BILLION*";
static char million_ename[] = " MILLION*";
static char thousand_ename[] = " THOUSAND*";
static char upper_eand[] = "*AND ";
static char lower_eand[] = "*and ";
static char dollar_ename[] = " DOLLAR";
static char dollars_ename[] = "S";
static char hundred_ename[] = " HUNDRED ";
static char tens_eseparator[] = "-";

static char		*dollarpat_s = "-$ZZZ,ZZZ,ZZZ,ZZZ,ZZ9.99";
static char	*digit_snames[] =
{
	"","UN","DOS","TRES","CUATRO",
	"CINCO","SEIS","SIETE","OCHO","NUEVE"
};
static char *ten_snames[] =
{
	"","","VEINTE ","TREINTA ","CUARENTA ",
	"CINCUENTA ","SESENTA ","SETENTA ","OCHENTA ","NOVENTA "
};
static char *teen_snames[] =
{
	"DIEZ ","ONCE ","DOCE ","TRECE ","CATORCE ",
	"QUINCE ","DIECISEIS ","DIECISIETE ","DIECIOCHO ","DIECINUEVE "
};
static char *twenty_snames[] =
{	"VEINTE ","VEINTIUN ","VEINTIDOS ","VEINTITRES ","VEINTICUATRO ",
	"VEINTICINCO ","VEINTISEIS ","VEINTISIETE ","VEINTIOCHO ","VEINTINUEVE "
};
static char *hundred_snames[] = 
{	"","CIENTO ","DOSCIENTOS ","TRESCIENTOS ","CUATROCIENTOS ",
	"QUINIENTOS ","SEISCIENTOS ","SETECIENTOS ","OCHOCIENTOS ","NOVECIENTOS "
};

static char minus_sname[] = "NEGATIVO ";
static char no_sname[] = "NO ";
static char trillion_sname[] = " TRILLON*";
static char billion_sname[] = " BILLON*";
static char million_sname[] = S_MILLONES;
static char thousand_sname[] = S_THOUSAND;
static char upper_sand[] = " ";
static char lower_sand[] = " ";
static char dollar_sname[] = " PESO";
static char dollars_sname[] = "S";
//static char hundred_sname[] = "CIENTOS ";
static char tens_sseparator[] = " Y ";

XDLL int 
dollar_to_alpha(int argc, rtobj *argv[])
{
	decno	amt,
			workamt;
	int		length;
	char	fill_char,
			format,
			up_case,
			print_dollar;
	char	txt_number[MAXDIGITS + 1 + 1],	/* add 1 for null and for '.'	*/
			temp_txt_number[MAXDIGITS + 1 + 1],
			place_value[4];
	int		text_length;
	int		negative;
	int		ret_code;
	char	result[256];
	char	zero_value[81];
	char	*src,
			*dest;
	int		language = ENGLISH;

	/* argument count has to be correct to get through the	*/
	/* compiler, so it doesn't need to be checked here		*/
	/* check argument count	*/
	if (argc < 7 || argc > 8)
		return(CSR_FAIL_ARGCOUNT);

	/* get parameters from rtobj's	*/
	rtobj2decwork(argv[0],
		(decwork *)&amt);				/* the amount to be printed	*/
	rtobj2int(argv[1], &length);		/* maximum output length	*/
	rtobj2char(argv[2], &fill_char);	/* character to pad with	*/
	rtobj2str(argv[3], result, 256);			/* Single, 1st or 2nd line	*/
	if (*result == 's' || *result == 'S') { /* 'S1', 'S2' 'Sl' or 'SL' */
		format = result[1];
		init_spanish();
		}
	else {
		format = result[0];
		init_english();
		}
	rtobj2char(argv[3], &format);		/* Single, 1st or 2nd line	*/
	rtobj2char(argv[4], &up_case);		/* text in all upper case	*/
	rtobj2char(argv[5], &print_dollar);/* print "dollar" at end	*/

	/* has to be an ascii type field 	*/
	if (argv[6]->obj.object != FLD)
		return(CSR_FAIL_ARGTYPE);
	dollarresult = argv[6]->ptr;
	if (isASCIIfield(dollarresult) == 0)
		return(CSR_FAIL_ARGTYPE);
	if (argc == 8) {
#ifdef NT
		if (rtobj2str(argv[7], zero_value, sizeof(zero_value)) == RT_ERROR)
#else
		if (rtobj2str(argv[7], zero_value, sizeof(zero_value)) == ERROR)
#endif
			return(CSR_FAIL_ARGTYPE);
		}
	else
		zero_value[0] = '\0';

	dollar_init();
	/* clear out stuff from last time through */
	memset(dollarresult->storage, ' ', fldstosize(dollarresult));

	/* clear result buffer	*/
	if (spanish)
	{
		result[0] = '(';
		result[1] = '\0';
	}
	else
		result[0] = '\0';

	/* terminate 3 byte ascii number buffer	*/
	place_value[3] = '\0';

	/* if negative, set negative flag and make number positive	*/
	if (deccmp((decwork *)&amt, &dolr_zero) < 0)
	{
		negative = 1;
		strcat(result, minus_name);
		amt.number.sign = 1;
	}
	else
	{
		negative = 0;
	}

	/* check for number in range	*/
	if (deccmp((decwork *)&amt, (decwork *)&dolr_max) > 0)
	{
		dollar_error();
		return(OUT_OF_RANGE);
	}

	/* make a copy of the number	*/
	memcpy(&workamt, &amt, sizeof(workamt));
	/* make sure it's got 2 decimal places	*/
	while (workamt.number.places < 2) {
		workamt.number.digits[workamt.number.length++] = '\0';
		workamt.number.places++;
		}
	/* round it if it's over 2 places	*/
	if (workamt.number.places > 2)
		remove_digits((decwork *)&workamt, (short)(workamt.number.places - 2),
			-9999);	// -9999 is magic to remove_digits()...

	/* move number to ascii storage	*/
	decwork2str((decwork *)&workamt, temp_txt_number, (short)MAXDIGITS + 1);
	sprintf(txt_number, "%*s", MAXDIGITS + 1, temp_txt_number);
	txt_number[MAXDIGITS + 1] = '\0';

	if (deccmp((decwork *)&amt, (decwork *)&dolr_one) < 0)	{
		/* leading text for amt < $1.00	*/
		/* Spanish version spells out the cents  when under a dollar */
		if (!spanish) 	
			strcat(result, no_name);
		}

	/* start with trillions	*/
	strncpy(place_value, txt_number, 3);
	dtoa_bld_number(atoi(place_value), result, trillion_name);

	/* next do billions	*/
	strncpy(place_value, txt_number + 3, 3);
	dtoa_bld_number(atoi(place_value), result, billion_name);

	/* now millions		*/
	strncpy(place_value, txt_number + 6, 3);
	dtoa_bld_number(atoi(place_value), result, million_name);

	/* thousands		*/
	strncpy(place_value, txt_number + 9, 3);
	dtoa_bld_number(atoi(place_value), result, thousand_name);

	/* ones				*/
	strncpy(place_value, txt_number + 12, 3);
	dtoa_bld_number(atoi(place_value), result, "");


	if (spanish)	{
	
		/* add the 'DOLLAR(S)' string to the end of the amount	*/
		/* it is only added if we have dollars, cents only gets printed */
		/* as centavos */
		if (print_dollar == 'y' || print_dollar == 'Y')	{
			if (deccmp((decwork *)&amt, (decwork *)&dolr_one) == 0 || 
				(deccmp((decwork *)&amt, (decwork *)&dolr_two) < 0 &&
				deccmp((decwork *)&amt, (decwork *)&dolr_one) > 0) )	{
				strcat(result,dollar_name);
				strcat(result, " ");
				}				
			else
				if (deccmp((decwork *)&amt, (decwork *)&dolr_two) == 0 || \
					deccmp((decwork *)&amt, (decwork *)&dolr_two) > 0 ) {
					/* add an "S" if it's not exactly equal to 1.00	*/
					strcat(result,dollar_name);
					strcat(result,dollars_name);
					strcat(result, " ");
				}
			}

		/* pennies	*/
		if (deccmp((decwork *)&amt, (decwork *)&dolr_one) < 0)	{
			/* if we have only cents, no dollars */
			/* we need to spell out cents */	
			if (atoi(txt_number + 16))	{
				/* we have some cents */
				sprintf(place_value," %s",txt_number + 16);
				dtoa_bld_number(atoi(place_value), result, "");
				strcat(result,S_CENT);
				/* if not one cent then make Plural*/
				if (deccmp((decwork *)&amt, (decwork *)one_cent) > 0)
					strcat(result,S_CENTS);
				strcat(result,")");
				}
			else	
				strcat(result,"CERO PESOS 00/100 M.N.)");
			}
		else	{		
			if (atoi(txt_number + 16))
				strcat(result, txt_number + 16);
		   	else
				strcat(result,"00");
			strcat(result, "/100 M.N.)");
			}
		}
	else	{
		/* pennies	*/
		if (atoi(txt_number + 16))	{
			strcat(result, up_case == 'y' || up_case == 'Y' ? upper_and : lower_and);
			strcat(result, txt_number + 16);
			strcat(result, "/100");
			}
		else
			strcat(result,zero_value); 

		/* add the 'DOLLAR(S)' string to the end of the amount	*/
		if (print_dollar == 'y' || print_dollar == 'Y')	{
			strcat(result, dollar_name);
			/* add an "S" if it's not exactly equal to 1.00	*/
			if (deccmp((decwork *)&amt, &dolr_one) != 0)
				strcat(result, dollars_name);
			}
		}
	/* remove the extra blanks that ended up in the number	*/
	src = dest = result;
	/* trim leading white space	*/
	while (*src == ' ')
		src++;
	/* move string, changing double spaces to single spaces	*/
	while (*src)
	{
		*dest++ = *src++;
		/* bump pointer as long as two spaces are side by side	*/
		while ((*src == ' ' || *src == '*') &&
			(*(src - 1) == ' ' || *(src - 1) == '*'))
		{
			if (*src == '*')
			{
				*dest = '*';
			}
			src++;
		}
	}
	/* remove trailing spaces	*/
	*dest = '\0';
	dest--;
	while (*dest == ' ' || *dest == '*')
	{
		*dest = '\0';
		dest--;
	}

	ret_code = TEXT_FORMAT;
	/* determine whether or not alpha format will fit in string	*/
	if (strlen(result) <= (unsigned)length)
	{
		if (format == SECOND_LINE)
		{	
			/* it all fit in the first line, return an empty second line	*/
			*result = '\0';
			fill_char = ' ';
		}
	}
	else
	{
		if (format != FIRST_LINE && format != SECOND_LINE)
		{
			/* string too long, switch to alternate format	*/
			dollar_to_string(txt_number, negative, result);
			ret_code = NUMERIC_FORMAT;
		}
		else
		{
			/* multiple lines requested, split line at '*'	*/
			src = result + length;
			while (src > result && *src != '*')
			{
				src--;
			}
			*src = '\0';
			if (src == result || strlen(result) > (unsigned)length ||
				strlen(src + 1) > (unsigned)length)
			{
				ret_code = NUMERIC_FORMAT;

				if (format == SECOND_LINE)
				{
					fill_char = ' ';
					*result = '\0';
				}
				else
				{
					/* string too long, switch to alternate format	*/
					dollar_to_string(txt_number, negative, result);
				}
			}
			else
			{
				if (format == FIRST_LINE)
				{
					fill_char = ' ';
				}
				else
				{
					/* copy second half to result	*/
					src++;
					dest = result;
					while (*dest++ = *src++)
						;
				}
			}
		}
	}

	if (strlen(result) > (unsigned)length)
	{
		/* still too long, return error	*/
		dollar_error();
		return(BUFFER_TOO_SMALL);
	}

	/* change '*'s to spaces	*/
	src = result;
	while (*src)
	{
		if (*src == '*')
		{
			*src = ' ';
		}
		src++;
	}
	/* switch to initial caps with lower case if requested	*/
	if (up_case != 'y' && up_case != 'Y')
	{
		text_length = 1;
		while (result[text_length])
		{
			if (isupper(result[text_length]) &&
				result[text_length - 1] != ' ' &&
					result[text_length - 1] != '-')
			{
				/* change upper to lower if not the first char in word	*/
				result[text_length] = tolower(result[text_length]);
			}
			text_length++;
		}
	}

	/* fill string to maximum length with fill character	*/
		dtoa_fill(result, length, format, fill_char);

	/* move final result to target field	*/
	str2field(result, dollarresult);

#	ifdef DEBUGWANTED
		printf("dollar_to_text: returning ->%s<-\n", dollarresult->storage);
#	endif
	return(ret_code);
}
/*
* dtoa_bld_number - append the text for a number (0 - 100) to
*                   the given buffer
*/
static void dtoa_bld_number(val, result, text_place)
	int		val;
	char	*result,
			*text_place;
{
	int		hundreds,
			tens,
			ones;

	if (val)
	{
		/* break into values	*/
		hundreds = val / 100;
		val -= (hundreds * 100);
		tens = val / 10;
		ones = val % 10;

   		if (spanish)	{
			if (hundreds)	{
				if (hundreds == 1 && tens == 0 && ones == 0)
					/* 100 has special name */
					strcat(result,S_HUNDRED);
				else				
					/* add text for hundred value	*/
					strcat(result, hundred_names[hundreds]);
				}
				
			if (tens == 1)
				/* special handling for teens includes 10's and 1's	*/
				strcat(result, teen_names[ones]);
			else if (tens == 2)
				strcat(result, twenty_names[ones]);
			else	{
				if (tens)	{
					/* add ten's name, and "Y" if there are ones	*/
					strcat(result, ten_names[tens]);
					if (ones)
						strcat(result, tens_separator);
					}
				if (ones)
					/* add name for ones	*/
					/* NOTE: that if text_place is a thousand and no hundreds or tens */
					/* we do not want "UN" infront of it, special case */
					if (strcmp(text_place,S_THOUSAND) != 0 || hundreds != 0 || tens != 0 || ones != 1) {
						strcat(result, digit_names[ones]);
						}
				}
			
			/* add name for current set (million, thousand, etc.)	*/
			if (strlen(text_place))	{
				strcat(result, " ");
				if (strcmp(text_place,S_MILLONES) == 0 && ones == 1)
					/* Special case for millon */ 
					strcat(result, S_MILLON);
				else
					strcat(result, text_place);
				strcat(result, " ");
				}
			}
		else	{
			if (hundreds)
			{
				/* add text for hundred value	*/
				strcat(result, digit_names[hundreds]);
				strcat(result, hundred_name);
			}
			if (tens == 1)
			{
				/* special handling for teens includes 10's and 1's	*/
				strcat(result, teen_names[ones]);
			}
			else
			{
				if (tens)
				{
					/* add ten's name, and hyphen if there are ones	*/
					strcat(result, ten_names[tens]);
					if (ones)
						strcat(result, tens_separator);
				}
				if (ones)
				{
					/* add name for ones	*/
					strcat(result, digit_names[ones]);
				}
			}

			/* add name for current set (million, thousand, etc.)	*/
			if (strlen(text_place))
			{
				strcat(result, " ");
				strcat(result, text_place);
				strcat(result, " ");
			}
		}  	
	}
}
static void dollar_to_string(txt_number, negative, result)
	char	*txt_number;
	int		negative;
	char	*result;
{
	char	*temp;

	/* skip leading spaces in txt_number	*/
	while (*txt_number == ' ')
		txt_number++;

	if (negative) {
		/* build a string with the '-' sign	*/
		temp = (char *)alloca(strlen(dollarpat) + 1);
		*temp = '-';
		strcpy(temp + 1, txt_number);
		}
	else
		temp = txt_number;

	editstring(result, FT_FIXED, temp, dollarpat, strlen(dollarpat),
		1, ' ');

	/* remove leading spaces	*/
	temp = result;
	while (*temp == ' ')
		temp++;
	while (*temp)
		*result++ = *temp++;
	*result = '\0';
}
static void dtoa_fill(char *result, int length, char format, char fill_char)
{
	char	*src,
			*dest;
	int		text_length;

	text_length = strlen(result);

	if (format == 'l' || format == 'L')
	{
		/* copy string to end of buffer	*/
		src = result + strlen(result) - 1;
		dest = result + length - 1;
		*(dest + 1) = '\0';
		while (src >= result)
		{
			*dest-- = *src--;
		}

		while (dest >= result)
			*dest-- = fill_char;
	}
	else
	{
		while (text_length < length)
		{
			*(result + text_length) = fill_char;
			text_length++;
		}
		*(result + text_length) = '\0';
	}
}
static void dollar_init(void)
{
	/* set up dolr_max value	*/
	dolr_max.number.sign = 1;
	dolr_max.number.length = MAXDIGITS;
	dolr_max.number.places = 2;
	strcpy(dolr_max.number.digits, "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
}
static void dollar_error(void)
{
	str2field("?e?e?e?e", dollarresult);
}
static void init_english()
{
	dollarpat = dollarpat_e;
	digit_names = digit_enames;
	ten_names = ten_enames;
	teen_names = teen_enames;
	minus_name = minus_ename;
	no_name = no_ename;
	trillion_name = trillion_ename;
	billion_name = billion_ename;
	million_name = million_ename;
	thousand_name = thousand_ename;
	upper_and = upper_eand;
	lower_and = lower_eand;
	dollar_name = dollar_ename;
	dollars_name = dollars_ename;
	hundred_name = hundred_ename;
	tens_separator = tens_eseparator;
}
static void init_spanish()
{
	dollarpat = dollarpat_s;
	digit_names = digit_snames;
	ten_names = ten_snames;
	teen_names = teen_snames;
	twenty_names = twenty_snames;
	hundred_names = hundred_snames;
	minus_name = minus_sname;
	no_name = no_sname;
	trillion_name = trillion_sname;
	billion_name = billion_sname;
	million_name = million_sname;
	thousand_name = thousand_sname;
	upper_and = upper_sand;
	lower_and = lower_sand;
	dollar_name = dollar_sname;
	dollars_name = dollars_sname;
//	hundred_name = hundred_sname;
	tens_separator = tens_sseparator;
	spanish=1;
}
