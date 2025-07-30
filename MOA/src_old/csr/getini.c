/*$
** MODULENAME - getini.c
**
** $Name: R1V4C13 R1V4C12 R1V4C11 R1V4C10 R1V4C9 R1V4C7 R1V4C6 $
** $Revision: 1.11 $
** $Date: 1996/12/12 20:46:50 $
**
** Author: Nagesh Nookala    January 1995
**
**	Synopsis:
**		This function retrives a string from a specified 
**  section from the INI file for the project and returns it in to the 
**	ASCII field passed as a parameter
**	
**	args
**		arg0 : Name of the section to be looked at. (INPUT)
**		arg1 : Name of the item to be looked at. (INPUT)
**		arg2 : field to get the result	(OUTPUT)
**
**	RETVALS:
**		0 		: success
**		-2		: can not find the item name in the INI file
**		RT_ERROR: invalid parameters.
*/

#include "pch.h"	
#include <typedefs.h>		/* contains DD object structure defs */
#include <rtdef.h>			/* Contains the Retval global declaration */
#include <dataman.h>
#include <csrdef.h>
#include <ofbdefs.h>
#include <convers.h>
#include <rt.h>
#include <defines.h>
#include <rtprotos.h>
#include <rtdef.h>
#include <msgrtn.h>	  

XDLL int get_profile_string(int argc, rtobj *argv[]);
XDLL int write_profile_string(int argc, rtobj *argv[]);
XDLL int get_internal_id();

get_profile_string(int argc, rtobj *argv[])

{
	char	SectionName[80];
	char	KeyName[80];
	char	Value[100];
	char	*DefaultString="Unknown";
	char	*IniFileName;
	field 	*ResultField;
	int		retval;

	if (argc < 3)	{
		mre(-1,"Invalid Number of arguments passed");
		return(CSR_FAIL_ARGCOUNT);
		}
	if (rtobj2str(argv[0],SectionName,sizeof(SectionName)) == RT_ERROR)	{
		mre(-1,"get_profile_string: Bad Section name field passed");
		return(RT_ERROR);
		}
	if (rtobj2str(argv[1],KeyName,sizeof(KeyName)) == RT_ERROR)	{
		mre(-1,"get_profile_string: Bad Key name field passed");
		return(RT_ERROR);
		}
	if (argv[2]->obj.object != FLD)	{
		mre(-1,"get_profile_string: Invalid Argument type passed");
		return(RT_ERROR);
		}
	else
		ResultField = argv[2]->ptr;
		
	IniFileName = (char *) ddininame(NULL);
	retval = GetPrivateProfileString(SectionName,
							KeyName,
							DefaultString,
							Value,
							(DWORD) sizeof (Value),
							IniFileName);
	if (retval > 0)	{
		if (strcmp(Value,"Unknown") == 0)
			return -2;
		else
			str2field(Value,ResultField);
		return 0;
		}
	else
		return -2;
 }

/*$
** MODULENAME - write_profile_string
** Author: Bob Spratt   June 1995
**
**	Synopsis:
**		This function writes a string to a specified 
**              section from the INI file for the project. 
**		if the section and item do not exist they are created.
**	
**	args
**		arg0 : Name of the section to be looked at. (INPUT)
**		arg1 : Name of the item to be looked at. (INPUT)
**		arg2 : field with the value	(INPUT)
**
**	RETVALS:
**		0 		: success
**		-1		: can not write to file
**		RT_ERROR: invalid parameters.
*/
write_profile_string(int argc, rtobj *argv[])
{
	char	SectionName[80];
	char	KeyName[80];
	char	Value[255];
	char	*IniFileName;

	if (argc < 3)	{
		mre(-1,"Invalid Number of arguments passed");
		return(CSR_FAIL_ARGCOUNT);
		}
	if (rtobj2str(argv[0],SectionName,sizeof(SectionName)) == RT_ERROR)	{
		mre(-1,"write_profile_string: Bad Section name field passed");
		return(RT_ERROR);
		}
	if (rtobj2str(argv[1],KeyName,sizeof(KeyName)) == RT_ERROR)	{
		mre(-1,"write_profile_string: Bad Key name field passed");
		return(RT_ERROR);
		}
	IniFileName = (char *) ddininame(NULL);
	rtobj2str(argv[2],Value,sizeof(Value));
	
	// this function returns 0 for fail and 1 for ok.
	// since most ofb based retvals are < 0 for failure
	// and 0 for success, I will subtract 1 from the retval
	// of the C function
	return (WritePrivateProfileString(SectionName,
							KeyName,
							Value,
							IniFileName) - 1);
	
 }

/* Function to get the internal id from ID.OUT file.
   Written because the greeter does not work on windows 95
   		-- Nagesh Nookala
*/
XDLL int
get_internal_id ()

{
static char FileName[256];
static char BioIdFile[256];
static OFSTRUCT BioOfStruct;
HANDLE BioFile;
DWORD BytesRead;
DWORD Pos;
long InternalId;
if (strlen(BioIdFile) == 0)	{
	GetWindowsDirectory	(FileName,256);
	strcat(FileName,"/");
	strcat(FileName,"BIO.INI");
	GetPrivateProfileString("VISITOR QUEUE", \
							"QUEUEDATADIR", \
							"NULL", \
							BioIdFile, \
							256, \
							FileName); 
	if (strlen(BioIdFile) != 0)	
		strcat(BioIdFile,"id.out");
	}

while ((BioFile = (HANDLE ) OpenFile(BioIdFile,&BioOfStruct,
		OF_READWRITE | OF_SHARE_EXCLUSIVE)) == (HANDLE )HFILE_ERROR)
	;	 
Pos = SetFilePointer(BioFile,0,NULL,FILE_BEGIN);
ReadFile(BioFile,&InternalId,sizeof(InternalId),&BytesRead,NULL);
InternalId++;
Pos = SetFilePointer(BioFile,0,NULL,FILE_BEGIN);
WriteFile(BioFile,&InternalId,sizeof(InternalId),&BytesRead,NULL);
CloseHandle(BioFile);
return (InternalId);
}








 
