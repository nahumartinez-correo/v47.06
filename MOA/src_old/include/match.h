#ifndef MATCH_H
#define MATCH_H

/* Regular Expressions:
*
*      [1]     char    matches itself, unless it is a special
*                      character (metachar): . \ [ ]* + ^ $
*
*      [2]     . or ?  matches any character (csh_metachars match a '.').
*
*      [3]     \       matches the character following it, except 
*          when followed by a digit 1 to 9. (see [8])
*			It is used as an escape character for all 
*			other meta-characters, and itself. When used
*			in a set ([4]), it is treated as an ordinary
*			character.
*            process all back slash characters and return as follows:
*              \Xff or \xff = byte whose hexadecimal value is equal to "ff"
*              \Dnnn or \dnnn = byte whose value is equal to decimal "nnn"
*              \Oddd or \oddd= byte whose value is equal to octal "ddd"
*              \Bd=b = byte in which bit d is b where d=0-7 and b=0 or 1
*              \b = backtab
*              \f = form feed
*              \h = horizontal tab
*              \n = new line (line feed)
*              \r = carriage return
*              \t = tab
*              \v = vertical tab
*              \0 = NULL
*              \c = c where c is any other character
*
*      [4]     [set]   matches one of the characters in the set.
*                      If the first character in the set is "^",
*                      it matches a character NOT in the set. A
*                      shorthand S-E is used to specify a set of
*                      characters S upto E, inclusive. The special
*                      characters "]" and "-" have no special
*                      meaning if they appear as the first chars
*                      in the set.
*                      examples:        match:
*
*                              [a-z]    any lowercase alpha
*
*                              [^]-]    any char except ] and -
*
*                              [^A-Z]   any char except uppercase
*                                       alpha
*
*                              [a-zA-Z] any alpha
*
*      [5]   *        any regular expression form [1] to [4], followed by
*                      closure char (*) matches zero or more matches of
*                      that form. (csh_metachars match zero or more
*					   characters, same as .* regular expression)
*
*      [5.1]   .*      matchs 0 or more occurances of any regular
*                      characters.
*
*      [6]     +       same as [5], except it matches one or more.
*                      (csh_metachars matches one or more characters, same
*				       as .+ regular expression)
*
*      [7]   (form)    a regular expression in the form [1] to [10], enclosed
*                      as (form) matches what form matches. The enclosure
*                      creates a set of tags, used for [8] and for
*                      pattern substution. The tagged forms are numbered
*			            starting from 1.
*
*      [8]   \n        a \ followed by a digit 1 to 9 matches whatever a
*                      previously tagged regular expression ([7]) matched.
*
*	    [9]	  <word>   	a regular expression starting with a < construct
*			  			and/or ending with a > construct, restricts the
*						pattern matching to the beginning of a word, and/or
*						the end of a word. A word is defined to be a character
*						string beginning and/or ending with the characters
*						A-Z a-z 0-9 and _. It must also be preceded and/or
*						followed by any character outside those mentioned.
*
*      [10]  xy          a composite regular expression xy where x and y
*                      are in the form [1] to [10] matches the longest
*                      match of x followed by a match for y.
*
*      [11]	^		a regular expression starting with a ^ character
*				$		and/or ending with a $ character, restricts the
*                      pattern matching to the beginning of the line,
*                      or the end of line. [anchors] Elsewhere in the
*						pattern, ^ and $ are treated as ordinary characters.
*					   (csh_metachars = 2, a '^' is inserted at the
*					   beginning of the pattern and a '$' is appended at
*					   the end)
*/

char *comm_re_comp(register char *pat, /* ptr to SVID regular expression */
  register unsigned char *translate,   /* pointer to translate table */
  register char csh_metachars) ;       /* flag indicating regular expressions or not */
/* return value: Returns a pointer to the compiled FSA (finite state automata)
   with the first two bytes a short (not necessarily aligned) which is the
   length or a NULL pointer if an error occurred and errno will contain the
   error number. */

int re_execb(register char *lp,       /* ptr to data to be matched */
             register char *comp_pat, /* ptr to FSA returned from comm_re_comp */
             register int length) ;   /* length to restrict match in data */

/* return value: Returns TRUE(1) if match was successful, FALSE(0) if the
   match was not successful or a negative number if an error occurred. The
   errors returned are defined in the include file "error.h". */

char *get_byte(register char *str, register char *value, char *translate);

char get_nybble(register char c);

extern char	*bopat[];				/* bopat[0] = begin of where pattern matched */
extern char *eopat[];				/* eopat[0] = end of where pattern matched */
#endif
