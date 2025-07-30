
/*$
** MODULENAME - file_rw.c
**
** $Name: R1V4C13 $
** $Revision: 1.12 $
** $Date: 1999/03/11 08:28:47 $
**
** Synopsis:
**   Description of the functions of this module
**
** External Function List:
**   List of external functions contained in this module
**
** Notes:
**   Any special linkage requirements, other notes about this
**   module.
**
**
**  (c) Copyright by ISC-Bunker Ramo Corp., 1989
**        All rights reserved.
**
*/

#include "pch.h"

#include <typedefs.h>
#include <convers.h>

#include <defines.h>
#include <rtdef.h>
#include <csrdef.h>
#include <rtprotos.h>

byte	*_get_mem(register short i, register byte *bp);
void	_file_sethdl (int file);
void	_file_gethdl (int file);

#define EOS '\0'
static 	FILE	*fc_hdl = ((FILE *)0);
static 	FILE	*f1_hdl = ((FILE *)0);
static 	FILE	*f2_hdl = ((FILE *)0);
static 	FILE	*f3_hdl = ((FILE *)0);
static 	FILE	*f4_hdl = ((FILE *)0);
static 	FILE	*f5_hdl = ((FILE *)0);
static 	FILE	*f6_hdl = ((FILE *)0);
static 	FILE	*f7_hdl = ((FILE *)0);
static 	FILE	*f8_hdl = ((FILE *)0);

/***********************************************************************/
/*  file_open                                                          */
/*        Opens a file.                                                */
/*        parms:  num - 1 - 8 a handle for other functions             */
/*                filename - full path to file to open                 */
/*                type - r  = read                                     */
/*                       w  = truncate or create for write             */
/*                       a  = append - open for writing at end of file */
/*                       r+ = open for update (reading and writing)    */
/*                       w+ = truncate of create for update            */
/*                       a+ = open for update to end of file           */
/*                       b  = open in binary mode, no translation,     */
/*                            use in combination with above types      */
/*        returns: return value 0 = opend worked                       */
/*                 return value -103 = File num already in use         */
/*                 return value -99  = open failed                     */
/***********************************************************************/
XDLL int 
file_open (int argc, rtobj *argv[])
{
	char	filename[128];
	char	file_type[3];
	char	*fptr;
	int		file;

	/* check for correct argument count	*/
	if (argc != 3)
		return(CSR_FAIL_ARGCOUNT);

	if (rtobj2int(argv[0], &file) != RT_SUCCESS ||
		rtobj2str(argv[1], filename, sizeof(filename)) != RT_SUCCESS ||
		rtobj2str(argv[2], file_type, sizeof(file_type)) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);

	_file_gethdl (file);
	if (fc_hdl != (FILE *) 0 )
	{
		return(-103);
	}
	/* get pointer to filename, open file, etc. */
	fptr = xlate_filename(filename);
	fc_hdl = fopen(fptr,file_type);
	if (fc_hdl == (FILE *) 0 )
	{
		return(-99);
	}
	_file_sethdl (file);
	return(VAL_PASS);
}


/***********************************************************************/
/*  file_write                                                         */
/*        Writes to a file                                             */
/*        parms:  num - 1 - 8 a handle of opened file                  */
/*                field - field to write to file                       */
/*                uselen - if present field is write to its max length */
/*                         if NOT present field is writen to numchars  */
/*        returns: return value 0 = opend worked                       */
/*                 return value -101 = File not open                   */
/*                 return value -100 = write failed                    */
/***********************************************************************/
XDLL int 
file_write(int argc, rtobj *argv[])
{
	int		file,type = -1;
	unsigned int nitems;
	field	*fp;

	if (rtobj2int(argv[0], &file) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);

	fp = (field *)argv[1]->ptr;
	if (argc == 3) {
		if (rtobj2int(argv[2], &type) != RT_SUCCESS)
			return(CSR_FAIL_ARGTYPE);
	}
	
	_file_gethdl (file);
	if (fc_hdl == (FILE *) 0 ) {
		return(-101);
	}
	
	nitems = fp->numchars;
	
	if (type == -1)	{
		if (fwrite((char *)fp->storage,1,nitems,fc_hdl) == EOF) {
			return(-100);
		}
		return(VAL_PASS);
	}
	
	if (type == 1) {
        switch (fp->flags.type) {
			case FT_INT:
			case FT_FLAGS:
			case FT_IND:
			/* these types are all 4-byte (32 bit) values   */
				nitems = 4;
			break;
			case FT_FIXED:
				/* use numchars for the amount of data entered, then add    */
				/* 3 bytes of overhead for the fixed field header           */
				nitems = fp->numchars + 3;
			break;
		}
		if (fwrite((char *)&(nitems),1,sizeof(int),fc_hdl) == EOF)
		{
			return(-100);
		}
	    if (nitems != 0)
	    {
			if (fwrite((char *)fp->storage,1,nitems,fc_hdl) == EOF)
			{
				return(-100);
			}
		}
		return(VAL_PASS);
	}
	return(-100); // type must be 1 or -1
}	


/***********************************************************************/
/*  file_read                                                          */
/*        Read from a file                                             */
/*        parms:  num   - 1 - 8 a handle of opened file                */
/*                field - field to read into                           */
/*                type  - optional, if -1 or not specified then        */
/*                        read for fp->length number of bytes, if 1    */
/*                        then read a four byte integer                */
/*        returns: return value 0 = opend worked                       */
/*                 return value -102 = File not open                   */
/*                 return value -1   = END OF FILE                     */
/***********************************************************************/
XDLL int 
file_read(int argc, rtobj *argv[])
{
	char    *ptr;
	field	*fp;
	int		file,rc,type = -1;
	unsigned int nitems;


	if (rtobj2int(argv[0], &file) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);

	fp = (field *)argv[1]->ptr;
	if (argc == 3) {
		if (rtobj2int(argv[2], &type) != RT_SUCCESS)
			return(CSR_FAIL_ARGTYPE);
	}
	
	nitems = fp->length;		// note: nitems overridden to 4 if type = 1
	ptr = (char *)fp->storage;
	memset(ptr,0,fp->length);
	_file_gethdl (file);
	
	if (fc_hdl == (FILE *) 0 ) {
		return(-102);
	}
	
	if (feof (fc_hdl) != 0) {
		fp->numchars = 0;
		return (-1);
	}
	
	if (type == -1)	{
		rc = fread (ptr,1,nitems,fc_hdl);
		fp->numchars = rc;
		return(VAL_PASS);
	}
	
	if (type == 1)	{
		rc = fread (&nitems,1,sizeof(int),fc_hdl);
		if (nitems != 0) {
            if ((unsigned)fp->length < nitems) {
				rterrorbox("file_read: Length of read exceeds size of field");
				return(-109);
			}
			rc = fread (ptr,1,nitems,fc_hdl);
		} else {
			rc = 0;
		}
		
		fp->numchars = rc;
		return(VAL_PASS);
	}
	return(-102); // type must be -1 or 1
}


/***********************************************************************/
/*  file_putline                                                       */
/*        Put a line to a file                                         */
/*        parms:  num - 1 - 8 a handle of opened file                  */
/*                field - field to write to file                       */
/*        returns: return value 0 = opend worked                       */
/*                 return value -101 = File not open                   */
/*                 return value -100 = write failed                    */
/***********************************************************************/
XDLL int 
file_putline(int argc, rtobj *argv[])
{
	char	line[300];
	int		file;

	/* check for correct argument count	*/
	if (argc != 2)
		return(CSR_FAIL_ARGCOUNT);

	if (rtobj2int(argv[0], &file) != RT_SUCCESS ||
		rtobj2str(argv[1], line, sizeof(line)) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);

	_file_gethdl (file);
	if (fc_hdl == (FILE *) 0 )
	{
		return(-101);
	}
	strcat(line,"\n");
	if (fputs(line,fc_hdl) == EOF)
	{
		return(-100);
	}
	/* return all OK code */
	return(VAL_PASS);
}	



int 
_file_putline(int file,char *line)
{
	_file_gethdl (file);
	if (fc_hdl == (FILE *) 0 )
	{
		return(-101);
	}
	strcat(line,"\n");
	if (fputs(line,fc_hdl) == EOF)
	{
		return(-100);
	}
	/* return all OK code */
	return(VAL_PASS);
}	


/***********************************************************************/
/*  file_getline                                                       */
/*        Get a line from a file                                       */
/*        parms:  num - 1 - 8 a handle of opened file                  */
/*                field - field to read into                           */
/*        returns: return value 0 = opend worked                       */
/*                 return value -102 = File not open                   */
/*                 return value -1   = END OF FILE                     */
/*                 return value -2   = stack space unavialable         */
/*                                                                     */
/* Modifications:                                                      */
/*                CAH, 5-8-95 remove 255 byte limit for reads          */
/***********************************************************************/
XDLL int 
file_getline(int argc, rtobj *argv[])
{
	char	*line, *ptr, *fgets();
	field	*fp;
	int		i, file;

	/* check for correct argument count	*/
	if (argc != 2)
		return(CSR_FAIL_ARGCOUNT);

	if (rtobj2int(argv[0], &file) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);		// 1st arg not an integer

	if (argv[1]->obj.object != FLD)
		return(CSR_FAIL_ARGTYPE);		// 2nd arg not a FIELD

	_file_gethdl (file);
	if (fc_hdl == (FILE *) 0 )
		return(-102);
	fp = (field *)argv[1]->ptr;
	line = alloca(fp->length);
	if (!line)
		return(-1);		// stack space unavailable
	ptr = fgets (line,fp->length,fc_hdl);
	if (ptr == (char *)0)
		return (-1);
	i = strlen(line);
	if (i > 0)
		line[i-1] = EOS;				// Discard trailing NEWLINE?
	str2field(line,fp);
	return(VAL_PASS);
}


/***********************************************************************/
/*  file_putibs                                                       */
/*        Put a ibs form to a file                                    */
/*        parms:  num - 1 - 8 a handle of opened file                  */
/*                list  - ibs list to write from                       */
/*        returns: return value 0 = opend worked                       */
/*                 return value -101 = File not open                   */
/*                 return value -100 = write failed                    */
/***********************************************************************/
XDLL int 
file_putibs(int argc, rtobj *argv[])
{
	char	line[300],form[100],tag[100],f_type[100];
	int		file,i;
	complist	*cl;
	byte		*mem_ptr,
				mem_cnt;
	short 		objlen;
	field		*fp;

	/* check for correct argument count	*/
	if (argc != 2)
		return(CSR_FAIL_ARGCOUNT);

	if (rtobj2int(argv[0], &file) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);

	_file_gethdl (file);
	if (fc_hdl == (FILE *) 0 )
	{
		return(-101);
	}
	if (argv[1]->obj.object == CSRARG_LST) 
	{
		cl = (complist *)ddget(RTddh, argv[1]->obj.object,
					argv[1]->obj.applno, argv[1]->obj.itemno);
	}
	else
	{
		return(-110);
	}

	mem_ptr = _get_mem(0,(byte *)(cl + 1));
	mem_cnt = mem_ptr[1];
	mem_ptr += 4;
	pcl2str (mem_ptr,&f_type[0],99,NULL);
	pcl2str (mem_ptr,&line[0],99,NULL);
	strcat(line,"\n");
	if (fputs(line,fc_hdl) == EOF)
	{
		return(-100);
	}

/* write form name */
	mem_ptr += tsize(mem_ptr);
	pcl2str (mem_ptr,&form[0],99,NULL);
	strcat(form,"\n");
	if (fputs(form,fc_hdl) == EOF)
	{
		return(-100);
	}

	if (strcmp(f_type,"FORMDEF") == 0)
	{
		for (i = 1; i < cl->numitems; i++) 
		{ 
			mem_ptr = _get_mem((short)i,(byte *)(cl + 1));
			mem_cnt = mem_ptr[1];
			mem_ptr += 4;
		/* put the tag */
			pcl2str (mem_ptr,&tag[0],99,NULL);
			strcat(tag,"\n");
			if (fputs(tag,fc_hdl) == EOF)
			{
				return(-100);
			}
		/* put the field */
			mem_ptr += tsize(mem_ptr);
			fp = rt_get_fp(mem_ptr,&objlen,NULL);
			if (fp == NULL)
			{
				return CSR_FAIL_ARGTYPE;
			}
			field2str(fp, &line[0],255);
			strcat(line,"\n");
			if (fputs(line,fc_hdl) == EOF)
			{
				return(-100);
			}
		}
	}
	else
	{
		mem_ptr = _get_mem(1,(byte *)(cl + 1));
		mem_cnt = mem_ptr[1];
		mem_ptr += 4;
	/* put the field */
		fp = rt_get_fp(mem_ptr,&objlen,NULL);
		if (fp == NULL)
		{
			return CSR_FAIL_ARGTYPE;
		}
		if (fwrite((char *)fp->storage,1,fp->numchars,fc_hdl) == EOF)
		{
			return(-100);
		}
	}

	if (strcmp(f_type,"FORMDEF") == 0)
	{
		sprintf(line,"ENDFORM\n");
	}
	else
	{
		sprintf(line,"ENDTEXT\n");
	}
	if (fputs(line,fc_hdl) == EOF)
	{
		return(-100);
	}

	/* return all OK code */
	return(VAL_PASS);
}	


/***********************************************************************/
/*  file_getibs                                                       */
/*        Get a ibs form from a file                                   */
/*        parms:  num - 1 - 8 a handle of opened file                  */
/*                list  - ibs list to read into                        */
/*        returns: return value 0 = opend worked                       */
/*                 return value -102 = File not open                   */
/*                 return value -1   = END OF FILE                     */
/***********************************************************************/
XDLL int 
file_getibs(int argc, rtobj *argv[])
{
	char	line[300],form[100],tag[100],f_type[100],*ptr,*fp_ptr,*fgets();
	int			file,i;
	complist	*cl;
	byte		*mem_ptr,
				mem_cnt;
	short 		objlen;
	field		*fp;

	/* check for correct argument count	*/
	if (argc != 2)
		return(CSR_FAIL_ARGCOUNT);

	if (rtobj2int(argv[0], &file) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);

	_file_gethdl (file);
	if (fc_hdl == (FILE *) 0 )
	{
		return(-102);
	}

	if (argv[1]->obj.object == CSRARG_LST) 
	{
		cl = (complist *)ddget(RTddh, argv[1]->obj.object,
					argv[1]->obj.applno, argv[1]->obj.itemno);
	}
	else
	{
		return(-110);
	}

/* start of a form */
	ptr = fgets (line,255,fc_hdl);
	line[strlen(&line[0])-1] = EOS;
	mem_ptr = _get_mem(0,(byte *)(cl + 1));
	mem_cnt = mem_ptr[1];
	mem_ptr += 4;
	pcl2str (mem_ptr,&f_type[0],99,NULL);
	if (strcmp(line,f_type) != 0)
	{
		return (-800);
	}
	if (strcmp(f_type,"FORMDEF") != 0 &&
	    strcmp(f_type,"TEXTDEF") != 0)
	{
		return (-801);
	}

/* get form name */
	ptr = fgets (line,255,fc_hdl);
	line[strlen(&line[0])-1] = EOS;
	if (ptr == (char *)0)
	{
		return (-1);
	}

	mem_ptr += tsize(mem_ptr);
	pcl2str (mem_ptr,&form[0],99,NULL);
	if (strncmp(line,form,strlen(&form[0])) != 0)
	{
		return (-802); /* form names dont match */
	} 

	if (strcmp(f_type,"FORMDEF") == 0)
	{
		for (i = 1; i < cl->numitems; i++) 
		{ 
			mem_ptr = _get_mem((short)i,(byte *)(cl + 1));
			mem_cnt = mem_ptr[1];
			mem_ptr += 4;
		/* get the tag */
			pcl2str (mem_ptr,&tag[0],99,NULL);
			ptr = fgets (line,255,fc_hdl);
			if (ptr == (char *)0)
			{
				return (-1);
			}
			line[strlen(&line[0])-1] = EOS;
			if (strcmp(line,tag) != 0)
			{
				return (-803); /* tags dont match */
			}
		/* get the field */
			mem_ptr += tsize(mem_ptr);
			fp = rt_get_fp(mem_ptr,&objlen,NULL);
			if (fp == NULL)
			{
				return CSR_FAIL_ARGTYPE;
			}
			ptr = fgets (line,255,fc_hdl);
			if (ptr == (char *)0)
			{
				return (-1);
			}
			line[strlen(&line[0])-1] = EOS;
			str2field(line,fp);
		}
	/* end of a form */
		ptr = fgets (line,255,fc_hdl);
		line[strlen(&line[0])-1] = EOS;
		if (strcmp(line,"ENDFORM") != 0)
		{
			return (-804);
		}
	}
	else
	{
		mem_ptr = _get_mem(1,(byte *)(cl + 1));
		mem_cnt = mem_ptr[1];
		mem_ptr += 4;
	/* get the field */
		fp = rt_get_fp(mem_ptr,&objlen,NULL);
		if (fp == NULL)
		{
			return CSR_FAIL_ARGTYPE;
		}
		ptr = (char *)fp->storage;
		memset(ptr,0,fp->length);
		fp_ptr = (char *) fp->storage;
		while (ptr != (char *)0)
		{
			ptr = fgets (line,298,fc_hdl);
			if (ptr == (char *)0)
			{
				return (-1);
			}
			if (strcmp(line,"ENDTEXT\n") == 0)
			{
				*fp_ptr = EOS;
				fp->numchars = strlen((char *)fp->storage);
				return(VAL_PASS);
			}
			if ((strlen(&line[0]) + strlen((char *)fp->storage))
				>= (unsigned)fp->length)
			{
				return (-805);
			}
			fp_ptr = memcpy (fp_ptr,&line[0],strlen(&line[0]));
			fp_ptr = fp_ptr + strlen(&line[0]);
		}
		return (-1);
	}

	return(VAL_PASS);
}


/***********************************************************************/
/*  file_close                                                         */
/*        close a file                                                 */
/*        parms:  num - 1 - 8 a handle of opened file                  */
/*        returns: return value 0 = opend worked                       */
/***********************************************************************/
XDLL int 
file_close(int argc, rtobj *argv[])
{
	int		file;

	if (argc != 1)
		return(CSR_FAIL_ARGCOUNT);
	if (rtobj2int(argv[0], &file) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);

	_file_gethdl (file);
	if (fc_hdl != (FILE *) 0 )
	{
		fclose(fc_hdl);
		fc_hdl = ((FILE *)0);
		_file_sethdl (file);
	}
	return(VAL_PASS);
}


/***********************************************************************/
/*  file_unlink                                                        */
/*        unlink a file                                                */
/*        parms:  filename - full path to file to unlink               */
/*        returns: return value 0 = opend worked                       */
/***********************************************************************/
XDLL int 
file_unlink(int argc, rtobj *argv[])
{
	char	filename[128];

	/* check for correct argument count	*/
	if (argc != 1)
		return(CSR_FAIL_ARGCOUNT);

	/* get the application name and item name	*/
	if (rtobj2str(argv[0], filename, sizeof(filename)) != RT_SUCCESS )
		return(CSR_FAIL_ARGTYPE);

	unlink (filename);
	return(VAL_PASS);
}


/***********************************************************************/
/*  file_flush                                                         */
/*        flush a file - same as closing and reopening                 */
/*        parms:  num - 1 - 8 a handle of opened file                  */
/*        returns: return value 0 = opend worked                       */
/*                 return value -108 = File not open                   */
/***********************************************************************/
XDLL int 
file_flush(int argc, rtobj *argv[])
{
	int		file;

	if (argc != 1)
		return(CSR_FAIL_ARGCOUNT);
	if (rtobj2int(argv[0], &file) != RT_SUCCESS)
		return(CSR_FAIL_ARGTYPE);

	_file_gethdl (file);
	if (fc_hdl == (FILE *) 0 )
	{
		return(-108);
	}
	fflush(fc_hdl);
	return(VAL_PASS);
}


/***********************************************************************/
/*  Internal routine to get the file handle                            */
/***********************************************************************/
void
_file_gethdl (int file)
{
	fc_hdl =  ((FILE *)0);
	switch (file)
	{
		case 1:
			fc_hdl = f1_hdl;
		break;
		case 2:
			fc_hdl = f2_hdl;
		break;
		case 3:
			fc_hdl = f3_hdl;
		break;
		case 4:
			fc_hdl = f4_hdl;
		break;
		case 5:
			fc_hdl = f5_hdl;
		break;
		case 6:
			fc_hdl = f6_hdl;
		break;
		case 7:
			fc_hdl = f7_hdl;
		break;
		case 8:
			fc_hdl = f8_hdl;
		break;
	}
}


/***********************************************************************/
/*  Internal routine to set the file handle                            */
/***********************************************************************/
void
_file_sethdl (int file)
{
	switch (file)
	{
		case 1:
			f1_hdl = fc_hdl;
		break;
		case 2:
			f2_hdl = fc_hdl;
		break;
		case 3:
			f3_hdl = fc_hdl;
		break;
		case 4:
			f4_hdl = fc_hdl;
		break;
		case 5:
			f5_hdl = fc_hdl;
		break;
		case 6:
			f6_hdl = fc_hdl;
		break;
		case 7:
			f7_hdl = fc_hdl;
		break;
		case 8:
			f8_hdl = fc_hdl;
		break;
	}
}
