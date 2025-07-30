# Microsoft Developer Studio Project File - Name="csr_s" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=csr_s - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "csr_s.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "csr_s.mak" CFG="csr_s - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "csr_s - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\csr_s___"
# PROP BASE Intermediate_Dir ".\csr_s___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin_slim"
# PROP Intermediate_Dir ".\csr\winrel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /I ".\include" /I ".\include\ofb" /I ".\csr" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp".\lib_slim\pch.pch" /YX"pch.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib /nologo /base:"0x12000000" /subsystem:windows /dll /map:"..\bin_slim\csr.map" /machine:I386 /out:"..\bin_slim\csr.dll" /implib:".\lib_slim\csr.lib"
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "csr_s - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\csr\apl_inde.c
# End Source File
# Begin Source File

SOURCE=.\csr\c_calc.c
# End Source File
# Begin Source File

SOURCE=.\csr\cdscsr.c
# End Source File
# Begin Source File

SOURCE=.\csr\chartif.c
# End Source File
# Begin Source File

SOURCE=.\csr\chkdigit.c
# End Source File
# Begin Source File

SOURCE=.\csr\cleararr.c
# End Source File
# Begin Source File

SOURCE=.\csr\copyrec.c
# End Source File
# Begin Source File

SOURCE=.\csr\csr.def
# End Source File
# Begin Source File

SOURCE=.\csr\csr_v.rc
# ADD BASE RSC /l 0x409 /i "csr"
# ADD RSC /l 0x409 /i "csr" /i ".\csr"
# End Source File
# Begin Source File

SOURCE=.\csr\csrlayer.c
# End Source File
# Begin Source File

SOURCE=.\csr\csrprint.c
# End Source File
# Begin Source File

SOURCE=.\csr\ctccsr.c
# End Source File
# Begin Source File

SOURCE=.\csr\datecsr.c
# End Source File
# Begin Source File

SOURCE=.\csr\dbcsr.c
# End Source File
# Begin Source File

SOURCE=.\csr\desktop.c
# End Source File
# Begin Source File

SOURCE=.\csr\dolrtxt.c
# End Source File
# Begin Source File

SOURCE=.\csr\file_rw.c
# End Source File
# Begin Source File

SOURCE=.\csr\fld_to.c
# End Source File
# Begin Source File

SOURCE=.\csr\fmt.c
# End Source File
# Begin Source File

SOURCE=.\csr\getini.c
# End Source File
# Begin Source File

SOURCE=.\csr\getname.c
# End Source File
# Begin Source File

SOURCE=.\csr\getofbin.c
# End Source File
# Begin Source File

SOURCE=.\csr\getrc.c
# End Source File
# Begin Source File

SOURCE=.\csr\hllapiwd.c
# End Source File
# Begin Source File

SOURCE=.\csr\ime.c
# End Source File
# Begin Source File

SOURCE=.\csr\ind_asgn.c
# End Source File
# Begin Source File

SOURCE=.\csr\listcsr.c
# End Source File
# Begin Source File

SOURCE=.\csr\loadfont.c
# End Source File
# Begin Source File

SOURCE=.\csr\lstappnd.c
# End Source File
# Begin Source File

SOURCE=.\csr\luacsr.c
# End Source File
# Begin Source File

SOURCE=.\csr\matchobj.c
# End Source File
# Begin Source File

SOURCE=.\csr\menu.c
# End Source File
# Begin Source File

SOURCE=.\csr\min_max.c
# End Source File
# Begin Source File

SOURCE=.\csr\new_pic.c
# End Source File
# Begin Source File

SOURCE=.\csr\pat_it.c
# End Source File
# Begin Source File

SOURCE=.\csr\printut.c
# End Source File
# Begin Source File

SOURCE=.\csr\proclist.c
# End Source File
# Begin Source File

SOURCE=.\csr\prt_pic.c
# End Source File
# Begin Source File

SOURCE=.\csr\rangechk.c
# End Source File
# Begin Source File

SOURCE=.\csr\removr.c
# End Source File
# Begin Source File

SOURCE=.\csr\runofb.c
# End Source File
# Begin Source File

SOURCE=.\csr\samisc.c
# End Source File
# Begin Source File

SOURCE=.\csr\sapl.c
# End Source File
# Begin Source File

SOURCE=.\csr\scrap.c
# End Source File
# Begin Source File

SOURCE=.\csr\stamptxt.c
# End Source File
# Begin Source File

SOURCE=.\csr\str_sr.c
# End Source File
# Begin Source File

SOURCE=.\csr\tag.c
# End Source File
# Begin Source File

SOURCE=.\csr\timecsr.c
# End Source File
# Begin Source File

SOURCE=.\csr\totalist.c
# End Source File
# Begin Source File

SOURCE=.\csr\tran.c
# End Source File
# Begin Source File

SOURCE=.\csr\trimfiel.c
# End Source File
# Begin Source File

SOURCE=.\csr\ubrmwin.c
# End Source File
# Begin Source File

SOURCE=.\csr\wclist.c
# End Source File
# Begin Source File

SOURCE=.\csr\wingsp.c
# End Source File
# Begin Source File

SOURCE=.\csr\winhelp.c
# End Source File
# Begin Source File

SOURCE=.\csr\yesorno.c
# End Source File
# Begin Source File

SOURCE=.\lib_slim\pch.obj
# End Source File
# Begin Source File

SOURCE=.\lib_slim\rt.lib
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\include\bitobj.h
# End Source File
# Begin Source File

SOURCE=.\include\c.h
# End Source File
# Begin Source File

SOURCE=.\include\cbproto.h
# End Source File
# Begin Source File

SOURCE=.\include\cbtrdef.h
# End Source File
# Begin Source File

SOURCE=.\include\cbtree.h
# End Source File
# Begin Source File

SOURCE=.\include\cds.h
# End Source File
# Begin Source File

SOURCE=.\csr\cdscsr.h
# End Source File
# Begin Source File

SOURCE=.\include\cdsdefs.h
# End Source File
# Begin Source File

SOURCE=.\include\cdsfuncs.h
# End Source File
# Begin Source File

SOURCE=.\include\cdslmts.h
# End Source File
# Begin Source File

SOURCE=.\include\cdsmacro.h
# End Source File
# Begin Source File

SOURCE=.\include\cdstype.h
# End Source File
# Begin Source File

SOURCE=.\include\cdswords.h
# End Source File
# Begin Source File

SOURCE=.\include\clccb.h
# End Source File
# Begin Source File

SOURCE=.\include\clcdsenv.h
# End Source File
# Begin Source File

SOURCE=.\include\clcdsfn.h
# End Source File
# Begin Source File

SOURCE=.\include\clconn.h
# End Source File
# Begin Source File

SOURCE=.\include\clenv.h
# End Source File
# Begin Source File

SOURCE=.\include\clrb.h
# End Source File
# Begin Source File

SOURCE=.\include\cmfield.h
# End Source File
# Begin Source File

SOURCE=.\include\commpro.h
# End Source File
# Begin Source File

SOURCE=.\include\convers.h
# End Source File
# Begin Source File

SOURCE=.\include\cque.h
# End Source File
# Begin Source File

SOURCE=.\include\csrdef.h
# End Source File
# Begin Source File

SOURCE=.\include\dataman.h
# End Source File
# Begin Source File

SOURCE=.\include\dbuser.h
# End Source File
# Begin Source File

SOURCE=.\include\ddrevno.h
# End Source File
# Begin Source File

SOURCE=.\include\decmath.h
# End Source File
# Begin Source File

SOURCE=.\include\defines.h
# End Source File
# Begin Source File

SOURCE=.\include\dpwords.h
# End Source File
# Begin Source File

SOURCE=.\include\dsplist.h
# End Source File
# Begin Source File

SOURCE=.\include\dumper.h
# End Source File
# Begin Source File

SOURCE=.\include\er.h
# End Source File
# Begin Source File

SOURCE=.\include\errdefs.h
# End Source File
# Begin Source File

SOURCE=.\include\keycodes.h
# End Source File
# Begin Source File

SOURCE=.\include\libperf.h
# End Source File
# Begin Source File

SOURCE=.\include\listmgr.h
# End Source File
# Begin Source File

SOURCE=.\include\mcnuser.h
# End Source File
# Begin Source File

SOURCE=.\include\memalloc.h
# End Source File
# Begin Source File

SOURCE=.\include\misc.h
# End Source File
# Begin Source File

SOURCE=.\include\msgrtn.h
# End Source File
# Begin Source File

SOURCE=.\include\ncontrol.h
# End Source File
# Begin Source File

SOURCE=.\include\nls.h
# End Source File
# Begin Source File

SOURCE=.\include\nowait.h
# End Source File
# Begin Source File

SOURCE=.\include\ntddser.h
# End Source File
# Begin Source File

SOURCE=.\include\ofbwords.h
# End Source File
# Begin Source File

SOURCE=.\include\onaport.h
# End Source File
# Begin Source File

SOURCE=.\include\pch.h
# End Source File
# Begin Source File

SOURCE=.\include\pclwords.h
# End Source File
# Begin Source File

SOURCE=.\include\picklist.h
# End Source File
# Begin Source File

SOURCE=.\include\ppdefs.h
# End Source File
# Begin Source File

SOURCE=.\include\rt.h
# End Source File
# Begin Source File

SOURCE=.\include\rtable.h
# End Source File
# Begin Source File

SOURCE=.\include\rtdef.h
# End Source File
# Begin Source File

SOURCE=.\include\rtfonts.h
# End Source File
# Begin Source File

SOURCE=.\include\rtglobal.h
# End Source File
# Begin Source File

SOURCE=.\include\rtprotos.h
# End Source File
# Begin Source File

SOURCE=.\include\rttimers.h
# End Source File
# Begin Source File

SOURCE=.\include\scrlist.h
# End Source File
# Begin Source File

SOURCE=.\include\svcdsfn.h
# End Source File
# Begin Source File

SOURCE=.\include\svmode.h
# End Source File
# Begin Source File

SOURCE=.\include\swerror.h
# End Source File
# Begin Source File

SOURCE=.\include\tdspl.h
# End Source File
# Begin Source File

SOURCE=.\include\textedit.h
# End Source File
# Begin Source File

SOURCE=.\include\this_sys.h
# End Source File
# Begin Source File

SOURCE=.\include\typedefs.h
# End Source File
# Begin Source File

SOURCE=.\include\ubram.h
# End Source File
# Begin Source File

SOURCE=.\include\ubrmams.h
# End Source File
# Begin Source File

SOURCE=.\include\ubrmspl.h
# End Source File
# Begin Source File

SOURCE=.\include\unistd.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
