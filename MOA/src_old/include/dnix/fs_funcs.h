/*$
**
**       Functions the FSS performs when asked by FSMAN utility
**
**   These #defines are in a separate file so that fsman.c does not
**   have to #include all the *.h files that the regular fs*.c files do.
**
*/

#define FSS_ADDLINK		0		/* Add a soft link */
#define FSS_STAT		1		/* Read status */
#define FSS_STATC		2		/* Read and clear status */
#define FSS_ADDWS		3		/* Add a workstation */
#define FSS_RMWS		4		/* Remove a workstation */
#define FSS_KILLWS		5		/* Kill a workstation */
#define FSS_ACT			6		/* Activate workstation */
#define FSS_DEACT		7		/* Deactivate workstation */
#define FSS_DEBUG_ON	8		/* turn ON debugging */
#define FSS_DEBUG_OFF	9		/* turn OFF debugging */
#define FSS_DUMP_FILES	10		/* show open Unix files */
#define FSS_ZAP_FS    	11		/* commit Sepaku... */
#define FSS_PROFILE    	12		/* Show function call statistics */
#define FSS_WSSTAT     	13		/* Get ws_stat structure in binary */
#define FSS_WSSTATC    	14		/* Get & clear ws_stat structure in binary */
