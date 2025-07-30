global_var	int	cverr;

/*
 *	conversion error codes
 */

# define  CVERCHAR	1	/* invalid character */
# define  CVERNODIG	2	/* no digits */
# define  CVERLARGE	3	/* too large */
# define  CVEROVFL	3	/* overflow */
# define  CVERSMALL	4	/* too small */
# define  CVERUNFL	4	/* underflow */

/* any other codes are non-standard */

/* "cvxll" */

# define  CVERSCALE	5	/* scale error */

/* "cvxgdate" */

# define  CVERYEAR	5
# define  CVERMONTH	6
# define  CVERDAY	7
# define  CVERHOUR	8
# define  CVERMINUTE	9
# define  CVERSECOND	10
