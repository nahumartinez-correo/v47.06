# Microsoft Developer Studio Generated NMAKE File, Based on rtenu.dsp
!IF "$(CFG)" == ""
CFG=rtenu - Win32 Release
!MESSAGE No configuration specified. Defaulting to rtenu - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "rtenu - Win32 Release" && "$(CFG)" != "rtenu - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rtenu.mak" CFG="rtenu - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rtenu - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "rtenu - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "rtenu - Win32 Release"

OUTDIR=.\..\bin_slim
INTDIR=.\nls\WinRel
# Begin Custom Macros
OutDir=.\..\bin_slim
# End Custom Macros

ALL : "$(OUTDIR)\rtenu.dll"


CLEAN :
	-@erase "$(INTDIR)\dummy.obj"
	-@erase "$(INTDIR)\english.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\rtenu.dll"
	-@erase ".\lib_slim\english.exp"
	-@erase ".\lib_slim\english.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /I ".\nls" /I ".\inlcude" /I ".\include\ofb" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\english.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\rtenu.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\rtenu.pdb" /machine:I386 /out:"$(OUTDIR)\rtenu.dll" /implib:".\lib_slim\english.lib" 
LINK32_OBJS= \
	"$(INTDIR)\dummy.obj" \
	"$(INTDIR)\english.res"

"$(OUTDIR)\rtenu.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "rtenu - Win32 Debug"

OUTDIR=.\..\bin
INTDIR=.\nls\windebug
# Begin Custom Macros
OutDir=.\..\bin
# End Custom Macros

ALL : "$(OUTDIR)\rtenu.dll"


CLEAN :
	-@erase "$(INTDIR)\dummy.obj"
	-@erase "$(INTDIR)\english.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\rtenu.dll"
	-@erase ".\lib\ENGLISH.exp"
	-@erase ".\lib\ENGLISH.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /GX /Z7 /Od /I ".\nls" /I ".\inlcude" /I ".\include\ofb" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\english.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\rtenu.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /pdb:none /debug /machine:I386 /out:"$(OUTDIR)\rtenu.dll" /implib:".\lib\ENGLISH.lib" 
LINK32_OBJS= \
	"$(INTDIR)\dummy.obj" \
	"$(INTDIR)\english.res"

"$(OUTDIR)\rtenu.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

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


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("rtenu.dep")
!INCLUDE "rtenu.dep"
!ELSE 
!MESSAGE Warning: cannot find "rtenu.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "rtenu - Win32 Release" || "$(CFG)" == "rtenu - Win32 Debug"
SOURCE=.\nls\dummy.c

"$(INTDIR)\dummy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\nls\english.rc

!IF  "$(CFG)" == "rtenu - Win32 Release"


"$(INTDIR)\english.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\english.res" /i "nls" /i ".\nls" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "rtenu - Win32 Debug"


"$(INTDIR)\english.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\english.res" /i "nls" /i ".\nls" /d "_DEBUG" $(SOURCE)


!ENDIF 


!ENDIF 

