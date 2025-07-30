# Microsoft Developer Studio Generated NMAKE File, Based on buildbuf.dsp
!IF "$(CFG)" == ""
CFG=buildbuf - Win32 Debug
!MESSAGE No configuration specified. Defaulting to buildbuf - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "buildbuf - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "buildbuf.mak" CFG="buildbuf - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "buildbuf - Win32 Debug" (based on "Win32 (x86) Static Library")
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
OUTDIR=.\tcl\WinDebug
INTDIR=.\tcl\WinDebug
# Begin Custom Macros
OutDir=.\tcl\WinDebug
# End Custom Macros

ALL : "$(OUTDIR)\buildbuf.lib"


CLEAN :
	-@erase "$(INTDIR)\buildbuf.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\buildbuf.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\buildbuf.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\buildbuf.lib" 
LIB32_OBJS= \
	"$(INTDIR)\buildbuf.obj"

"$(OUTDIR)\buildbuf.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

CPP_PROJ=/nologo /MDd /W3 /GX /Z7 /Od /I ".\tcl" /I ".\include" /I ".\include\ofb" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "NT" /D "DEBUG" /D "LITTLE_ENDIAN" /Fp".\lib\pch.pch" /YX"pch.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
!IF EXISTS("buildbuf.dep")
!INCLUDE "buildbuf.dep"
!ELSE 
!MESSAGE Warning: cannot find "buildbuf.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "buildbuf - Win32 Debug"
SOURCE=.\tcl\buildbuf.c

"$(INTDIR)\buildbuf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

