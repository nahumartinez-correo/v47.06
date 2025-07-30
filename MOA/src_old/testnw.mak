# Microsoft Developer Studio Generated NMAKE File, Based on testnw.dsp
!IF "$(CFG)" == ""
CFG=testnw - Win32 Debug
!MESSAGE No configuration specified. Defaulting to testnw - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "testnw - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "testnw.mak" CFG="testnw - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "testnw - Win32 Debug" (based on "Win32 (x86) Console Application")
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

ALL : "$(OUTDIR)\testnw.exe"


CLEAN :
	-@erase "$(INTDIR)\testnw.obj"
	-@erase "$(INTDIR)\TESTNW.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\testnw.exe"
	-@erase "$(OUTDIR)\testnw.ilk"
	-@erase "$(OUTDIR)\testnw.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\testnw.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /entry:"WinMainCRTStartup" /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\testnw.pdb" /debug /machine:I386 /out:"$(OUTDIR)\testnw.exe" 
LINK32_OBJS= \
	"$(INTDIR)\testnw.obj" \
	".\lib\pch.obj" \
	"$(INTDIR)\TESTNW.res" \
	".\tcl\WinDebug\buildbuf.lib" \
	".\lib\cdscl.lib"

"$(OUTDIR)\testnw.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TESTNW.res" /d "_DEBUG" 

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("testnw.dep")
!INCLUDE "testnw.dep"
!ELSE 
!MESSAGE Warning: cannot find "testnw.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "testnw - Win32 Debug"
SOURCE=.\tcl\testnw.c

"$(INTDIR)\testnw.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\tcl\TESTNW.RC

"$(INTDIR)\TESTNW.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\TESTNW.res" /i "tcl" /i ".\tcl" /d "_DEBUG" $(SOURCE)



!ENDIF 

