#ifndef CMFIELD_H
#define CMFIELD_H

#pragma pack (push, 2)
typedef struct fieldv
{
	unsigned char  appl_id;       /* application id number */
	unsigned char  type;          /* field data type    */
	unsigned short field_id;      /* field id number */
	unsigned short length;        /* field length not including terminator */
	byte		*value;           /* pointer to field value or null */
	char		*name;            /* field name, not usually needed */
	unsigned char  dimension;
    boolchar  	   is_member;     
	unsigned short sub1, sub2;    /* if !member, these are range limits */
			/* these have no meaning if dimension == 0 */
	unsigned short items_per_row; /* if whole 2 dimensional array */
#ifdef CDS_H
	struct SELECTCB *context; /* pointr to server side context control block */
#endif
} fieldv;
#pragma pack (pop)

global_proc	void *cmfield(regpar fieldv *field, regpar byte *buffer);
global_proc int cmfind_element(unsigned short *found_len, void **found_value, 
							fieldv *search_fld, fieldv *buff_fld);
	/* on entry search_fld == buff_fld and buff_fld->value is whole array.*/


global_proc int cmdereference(fieldv *found_fld, /* result */
							  void *base,	  /* record buffer */
							  void **prev_position, /* ptr within buffer */
							  fieldv *search_fld);	/* field to dereference */
/* Returns 1 if found, 0 if not found, less than 0 if erroroneous request */
#define FOUND_NOT_ARRAY -1	/* found scalar when searching for member or array */
#define MEMBER_SEARCH_DIMENSION -2  /* found array has different dimension than search argument */
#define ASKED_FOR_ARRAY_FOUND_MEMBER -3 /* asked for whole array, found single element */


global_proc void *cm_append_field(byte *target, fieldv *f, int *remaining,bool cv);
/* returns pointer to next available space or null if remaining is too small */

global_proc void *cm_append_array_hdr(byte *target, const fieldv *f,
								 int *remaining, byte **length_ptr);
/* returns pointer to space for first element 
** or null if remaining is too small.  Sets your length_ptr to space
** to receive length as next_field_ptr - (length_ptr + 2) */

global_proc void *cm_append_element(byte *target, fieldv *f, int *remaining,bool cv);
/* returns pointer to next available space or null if remaining is too small */
int import_CDS_indirect(int dictionary_handle, char *input_string, int *ind_value);

unsigned short cdsFieldLength(field *ddf);
#ifdef CLENV_H // clenv.h is needed for the following
int clTranslateFieldName(clenv *cdsenv, fieldv *f, const char *appl_name, field **ddfld);
int translate_fld_name4app(clenv *cdsenv, byte appl_id, fieldv *f, const char *fld_name, field **ddfld);
#endif
#endif
