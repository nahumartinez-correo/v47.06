# Microsoft Developer Studio Generated NMAKE File, Based on testddl.dsp
!IF "$(CFG)" == ""
CFG=testddl - Win32 Debug
!MESSAGE No configuration specified. Defaulting to testddl - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "testddl - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "testddl.mak" CFG="testddl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "testddl - Win32 Debug" (based on "Win32 (x86) Console Application")
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

ALL : "$(OUTDIR)\testddl.exe"


CLEAN :
	-@erase "$(INTDIR)\testddl.obj"
	-@erase "$(INTDIR)\TESTDDL.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\testddl.exe"
	-@erase "$(OUTDIR)\testddl.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\testddl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /entry:"WinMainCRTStartup" /subsystem:console /incremental:no /pdb:"$(OUTDIR)\testddl.pdb" /debug /machine:I386 /out:"$(OUTDIR)\testddl.exe" 
LINK32_OBJS= \
	"$(INTDIR)\testddl.obj" \
	".\lib\pch.obj" \
	"$(INTDIR)\TESTDDL.res" \
	".\tcl\WinDebug\buildbuf.lib" \
	".\lib\cdscl.lib"

"$(OUTDIR)\testddl.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TESTDDL.res" /d "_DEBUG" 

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("testddl.dep")
!INCLUDE "testddl.dep"
!ELSE 
!MESSAGE Warning: cannot find "testddl.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "testddl - Win32 Debug"
SOURCE=.\tcl\testddl.c

"$(INTDIR)\testddl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\tcl\TESTDDL.RC

"$(INTDIR)\TESTDDL.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\TESTDDL.res" /i "tcl" /i ".\tcl" /d "_DEBUG" $(SOURCE)



!ENDIF 

