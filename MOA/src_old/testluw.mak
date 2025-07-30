# Microsoft Developer Studio Generated NMAKE File, Based on testluw.dsp
!IF "$(CFG)" == ""
CFG=testluw - Win32 Debug
!MESSAGE No configuration specified. Defaulting to testluw - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "testluw - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "testluw.mak" CFG="testluw - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "testluw - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe
OUTDIR=.\..\bin
INTDIR=.\tcl\WinDebug
# Begin Custom Macros
OutDir=.\..\bin
# End Custom Macros

ALL : "$(OUTDIR)\testluw.exe"


CLEAN :
	-@erase "$(INTDIR)\testluw.obj"
	-@erase "$(INTDIR)\TESTLUW.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\testluw.exe"
	-@erase "$(OUTDIR)\testluw.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\testluw.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /entry:"WinMainCRTStartup" /subsystem:console /incremental:no /pdb:"$(OUTDIR)\testluw.pdb" /debug /machine:I386 /out:"$(OUTDIR)\testluw.exe" 
LINK32_OBJS= \
	"$(INTDIR)\testluw.obj" \
	".\lib\pch.obj" \
	"$(INTDIR)\TESTLUW.res" \
	".\tcl\WinDebug\buildbuf.lib" \
	".\lib\cdscl.lib"

"$(OUTDIR)\testluw.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

CPP_PROJ=/nologo /MDd /W3 /GX /Z7 /Od /I ".\tcl" /I ".\include" /I ".\include\ofb" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "NT" /D "DEBUG" /D "LITTLE_ENDIAN" /Fp".\lib\pch.pch" /YX"pch.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TESTLUW.res" /d "_DEBUG" 

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("testluw.dep")
!INCLUDE "testluw.dep"
!ELSE 
!MESSAGE Warning: cannot find "testluw.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "testluw - Win32 Debug"
SOURCE=.\tcl\testluw.c

"$(INTDIR)\testluw.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\tcl\TESTLUW.RC

"$(INTDIR)\TESTLUW.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\TESTLUW.res" /i "tcl" /i ".\tcl" /d "_DEBUG" $(SOURCE)



!ENDIF 

