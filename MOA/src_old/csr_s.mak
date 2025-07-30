# Microsoft Developer Studio Generated NMAKE File, Based on csr_s.dsp
!IF "$(CFG)" == ""
CFG=csr_s - Win32 Release
!MESSAGE No configuration specified. Defaulting to csr_s - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "csr_s - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "csr_s.mak" CFG="csr_s - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "csr_s - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
OUTDIR=.\..\bin_slim
INTDIR=.\csr\winrel
# Begin Custom Macros
OutDir=.\..\bin_slim
# End Custom Macros

ALL : "$(OUTDIR)\csr.dll"


CLEAN :
	-@erase "$(INTDIR)\apl_inde.obj"
	-@erase "$(INTDIR)\c_calc.obj"
	-@erase "$(INTDIR)\cdscsr.obj"
	-@erase "$(INTDIR)\chartif.obj"
	-@erase "$(INTDIR)\chkdigit.obj"
	-@erase "$(INTDIR)\cleararr.obj"
	-@erase "$(INTDIR)\copyrec.obj"
	-@erase "$(INTDIR)\csr_v.res"
	-@erase "$(INTDIR)\csrlayer.obj"
	-@erase "$(INTDIR)\csrprint.obj"
	-@erase "$(INTDIR)\ctccsr.obj"
	-@erase "$(INTDIR)\datecsr.obj"
	-@erase "$(INTDIR)\dbcsr.obj"
	-@erase "$(INTDIR)\desktop.obj"
	-@erase "$(INTDIR)\dolrtxt.obj"
	-@erase "$(INTDIR)\file_rw.obj"
	-@erase "$(INTDIR)\fld_to.obj"
	-@erase "$(INTDIR)\fmt.obj"
	-@erase "$(INTDIR)\getini.obj"
	-@erase "$(INTDIR)\getname.obj"
	-@erase "$(INTDIR)\getofbin.obj"
	-@erase "$(INTDIR)\getrc.obj"
	-@erase "$(INTDIR)\hllapiwd.obj"
	-@erase "$(INTDIR)\ime.obj"
	-@erase "$(INTDIR)\ind_asgn.obj"
	-@erase "$(INTDIR)\listcsr.obj"
	-@erase "$(INTDIR)\loadfont.obj"
	-@erase "$(INTDIR)\lstappnd.obj"
	-@erase "$(INTDIR)\luacsr.obj"
	-@erase "$(INTDIR)\matchobj.obj"
	-@erase "$(INTDIR)\menu.obj"
	-@erase "$(INTDIR)\min_max.obj"
	-@erase "$(INTDIR)\new_pic.obj"
	-@erase "$(INTDIR)\pat_it.obj"
	-@erase "$(INTDIR)\printut.obj"
	-@erase "$(INTDIR)\proclist.obj"
	-@erase "$(INTDIR)\prt_pic.obj"
	-@erase "$(INTDIR)\rangechk.obj"
	-@erase "$(INTDIR)\removr.obj"
	-@erase "$(INTDIR)\runofb.obj"
	-@erase "$(INTDIR)\samisc.obj"
	-@erase "$(INTDIR)\sapl.obj"
	-@erase "$(INTDIR)\scrap.obj"
	-@erase "$(INTDIR)\stamptxt.obj"
	-@erase "$(INTDIR)\str_sr.obj"
	-@erase "$(INTDIR)\tag.obj"
	-@erase "$(INTDIR)\timecsr.obj"
	-@erase "$(INTDIR)\totalist.obj"
	-@erase "$(INTDIR)\tran.obj"
	-@erase "$(INTDIR)\trimfiel.obj"
	-@erase "$(INTDIR)\ubrmwin.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\wclist.obj"
	-@erase "$(INTDIR)\wingsp.obj"
	-@erase "$(INTDIR)\winhelp.obj"
	-@erase "$(INTDIR)\yesorno.obj"
	-@erase "$(OUTDIR)\csr.dll"
	-@erase "$(OUTDIR)\csr.map"
	-@erase ".\lib_slim\csr.exp"
	-@erase ".\lib_slim\csr.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\csr_s.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib /nologo /base:"0x12000000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\csr.pdb" /map:"..\bin_slim\csr.map" /machine:I386 /def:".\csr\csr.def" /out:"$(OUTDIR)\csr.dll" /implib:".\lib_slim\csr.lib" 
DEF_FILE= \
	".\csr\csr.def"
LINK32_OBJS= \
	"$(INTDIR)\apl_inde.obj" \
	"$(INTDIR)\c_calc.obj" \
	"$(INTDIR)\cdscsr.obj" \
	"$(INTDIR)\chartif.obj" \
	"$(INTDIR)\chkdigit.obj" \
	"$(INTDIR)\cleararr.obj" \
	"$(INTDIR)\copyrec.obj" \
	"$(INTDIR)\csrlayer.obj" \
	"$(INTDIR)\csrprint.obj" \
	"$(INTDIR)\ctccsr.obj" \
	"$(INTDIR)\datecsr.obj" \
	"$(INTDIR)\dbcsr.obj" \
	"$(INTDIR)\desktop.obj" \
	"$(INTDIR)\dolrtxt.obj" \
	"$(INTDIR)\file_rw.obj" \
	"$(INTDIR)\fld_to.obj" \
	"$(INTDIR)\fmt.obj" \
	"$(INTDIR)\getini.obj" \
	"$(INTDIR)\getname.obj" \
	"$(INTDIR)\getofbin.obj" \
	"$(INTDIR)\getrc.obj" \
	"$(INTDIR)\hllapiwd.obj" \
	"$(INTDIR)\ime.obj" \
	"$(INTDIR)\ind_asgn.obj" \
	"$(INTDIR)\listcsr.obj" \
	"$(INTDIR)\loadfont.obj" \
	"$(INTDIR)\lstappnd.obj" \
	"$(INTDIR)\luacsr.obj" \
	"$(INTDIR)\matchobj.obj" \
	"$(INTDIR)\menu.obj" \
	"$(INTDIR)\min_max.obj" \
	"$(INTDIR)\new_pic.obj" \
	"$(INTDIR)\pat_it.obj" \
	"$(INTDIR)\printut.obj" \
	"$(INTDIR)\proclist.obj" \
	"$(INTDIR)\prt_pic.obj" \
	"$(INTDIR)\rangechk.obj" \
	"$(INTDIR)\removr.obj" \
	"$(INTDIR)\runofb.obj" \
	"$(INTDIR)\samisc.obj" \
	"$(INTDIR)\sapl.obj" \
	"$(INTDIR)\scrap.obj" \
	"$(INTDIR)\stamptxt.obj" \
	"$(INTDIR)\str_sr.obj" \
	"$(INTDIR)\tag.obj" \
	"$(INTDIR)\timecsr.obj" \
	"$(INTDIR)\totalist.obj" \
	"$(INTDIR)\tran.obj" \
	"$(INTDIR)\trimfiel.obj" \
	"$(INTDIR)\ubrmwin.obj" \
	"$(INTDIR)\wclist.obj" \
	"$(INTDIR)\wingsp.obj" \
	"$(INTDIR)\winhelp.obj" \
	"$(INTDIR)\yesorno.obj" \
	".\lib_slim\pch.obj" \
	"$(INTDIR)\csr_v.res" \
	".\lib_slim\rt.lib"

"$(OUTDIR)\csr.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

CPP_PROJ=/nologo /MD /W3 /GX /I ".\include" /I ".\include\ofb" /I ".\csr" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp".\lib_slim\pch.pch" /YX"pch.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\csr_v.res" /d "NDEBUG" 

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("csr_s.dep")
!INCLUDE "csr_s.dep"
!ELSE 
!MESSAGE Warning: cannot find "csr_s.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "csr_s - Win32 Release"
SOURCE=.\csr\apl_inde.c

"$(INTDIR)\apl_inde.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\c_calc.c

"$(INTDIR)\c_calc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\cdscsr.c

"$(INTDIR)\cdscsr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\chartif.c

"$(INTDIR)\chartif.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\chkdigit.c

"$(INTDIR)\chkdigit.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\cleararr.c

"$(INTDIR)\cleararr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\copyrec.c

"$(INTDIR)\copyrec.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\csr_v.rc

"$(INTDIR)\csr_v.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\csr_v.res" /i "csr" /i ".\csr" /d "NDEBUG" $(SOURCE)


SOURCE=.\csr\csrlayer.c

"$(INTDIR)\csrlayer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\csrprint.c

"$(INTDIR)\csrprint.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\ctccsr.c

"$(INTDIR)\ctccsr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\datecsr.c

"$(INTDIR)\datecsr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\dbcsr.c

"$(INTDIR)\dbcsr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\desktop.c

"$(INTDIR)\desktop.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\dolrtxt.c

"$(INTDIR)\dolrtxt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\file_rw.c

"$(INTDIR)\file_rw.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\fld_to.c

"$(INTDIR)\fld_to.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\fmt.c

"$(INTDIR)\fmt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\getini.c

"$(INTDIR)\getini.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\getname.c

"$(INTDIR)\getname.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\getofbin.c

"$(INTDIR)\getofbin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\getrc.c

"$(INTDIR)\getrc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\hllapiwd.c

"$(INTDIR)\hllapiwd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\ime.c

"$(INTDIR)\ime.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\ind_asgn.c

"$(INTDIR)\ind_asgn.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\listcsr.c

"$(INTDIR)\listcsr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\loadfont.c

"$(INTDIR)\loadfont.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\lstappnd.c

"$(INTDIR)\lstappnd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\luacsr.c

"$(INTDIR)\luacsr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\matchobj.c

"$(INTDIR)\matchobj.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\menu.c

"$(INTDIR)\menu.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\min_max.c

"$(INTDIR)\min_max.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\new_pic.c

"$(INTDIR)\new_pic.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\pat_it.c

"$(INTDIR)\pat_it.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\printut.c

"$(INTDIR)\printut.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\proclist.c

"$(INTDIR)\proclist.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\prt_pic.c

"$(INTDIR)\prt_pic.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\rangechk.c

"$(INTDIR)\rangechk.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\removr.c

"$(INTDIR)\removr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\runofb.c

"$(INTDIR)\runofb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\samisc.c

"$(INTDIR)\samisc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\sapl.c

"$(INTDIR)\sapl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\scrap.c

"$(INTDIR)\scrap.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\stamptxt.c

"$(INTDIR)\stamptxt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\str_sr.c

"$(INTDIR)\str_sr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\tag.c

"$(INTDIR)\tag.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\timecsr.c

"$(INTDIR)\timecsr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\totalist.c

"$(INTDIR)\totalist.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\tran.c

"$(INTDIR)\tran.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\trimfiel.c

"$(INTDIR)\trimfiel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\ubrmwin.c

"$(INTDIR)\ubrmwin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\wclist.c

"$(INTDIR)\wclist.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\wingsp.c

"$(INTDIR)\wingsp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\winhelp.c

"$(INTDIR)\winhelp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\csr\yesorno.c

"$(INTDIR)\yesorno.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

