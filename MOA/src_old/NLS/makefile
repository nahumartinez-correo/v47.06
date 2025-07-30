# =====================================================================
# NLS\MAKEFILE - National Language Support makefile
#
# Targets:
#   NLS.LIB
#   RTENU.DLL
#
#  An "NMAKE" makefile for Windows/NT 3.5
#  which builds a sub-component of the mosaic OA product
# =====================================================================
!include ..\makedflt

# ----------------------------------------------------------------------
# Use the Visual C++ generated makefile to build the DLL, since it knows
# how to do all that tricky Microsoft stuff.  We put this first, before
# the !include so we won't 'contaminate' the environment and cause the
# VC++ makefile to generate warnings.
# ----------------------------------------------------------------------
# Macro Defines
ENGLISH=rtENU
ARABIC=rtARA
GERMAN=rtDEU
ITALIAN=rtITA
JAPANESE=rtJPN
KOREAN=rtKOR
TAIWAN=rtCHT
THAILAND=rtTHA
#
#	NMAKE these
#
all: nls rtenu

slim_all: slim_nls slim_rtenu

nls:
	@ECHO ****** Making $(libs)\nls.lib ...
	@cd ..
	$(MAKE) /NOLOGO "DEBUG=1" CFG="nls - Win32 Debug" /S /f nls.mak
	@cd nls

rtenu:
	@ECHO ****** Making $(exe)\$(ENGLISH).dll ...
	@attrib -r rtenu_v.rc
	rcbump $(VBOPTS) rtenu_v.rc
	@cd ..
	$(MAKE) /NOLOGO "DEBUG=1" CFG="rtenu - Win32 Debug" /S /f rtenu.mak
	@cd nls

slim_nls:
	@ECHO ****** Making $(libs)\nls.lib
	@cd ..
	$(MAKE) /NOLOGO nodebug=1 CFG="nls - Win32 Release" /S /f nls.mak
	@cd nls

slim_rtenu:
	@ECHO ****** Making $(exe)\$(ENGLISH).dll ...
	@attrib -r rtenu_v.rc
	rcbump $(VBOPTS) rtenu_v.rc	
	@cd ..
	$(MAKE) /NOLOGO nodebug=1 CFG="rtenu - Win32 Release" /S /f rtenu.mak
	@cd nls

#	@ECHO ****** Making $(ARABIC).dll ...
#	$(MAKE) /NOLOGO "DEBUG=1" /S /f rtara.mak
#
#	@ECHO ****** Making $(GERMAN).dll ...
#	$(MAKE) /NOLOGO "DEBUG=1" /S /f rtdeu.mak
#
#	@ECHO ****** Making $(ITALIAN).dll ...
#	$(MAKE) /NOLOGO "DEBUG=1" /S /f rtita.mak
#
#	@ECHO ****** Making $(JAPANESE).dll ...
#	$(MAKE) /NOLOGO "DEBUG=1" /S /f rtjpn.mak
#
#	@ECHO ****** Making $(KOREAN).dll ...
#	$(MAKE) /NOLOGO "DEBUG=1" /S /f rtkor.mak
#
#	@ECHO ****** Making $(TAIWAN).dll ...
#	$(MAKE) /NOLOGO "DEBUG=1" /S /f rtcht.mak
#
#	@ECHO ****** Making $(THAILAND).dll ...
#	$(MAKE) /NOLOGO "DEBUG=1" /S /f rttha.mak

slim:
	@$(MAKE) /nologo nodebug=1 slim_all

check:
	@rcscheck $(CHKOPTS)

name:
	@echo ****** Naming source file set
	-@rcs -N$(RCSNAME):	*.c* *.h* *.rc makefile

# Clean Up fat version
clean:
	@cd ..
	@ECHO ****** deleting debug NLS.MAK binaries
	$(MAKE) /NOLOGO "DEBUG=1" /S /f nls.mak CFG="nls - Win32 Debug" CLEAN
	@ECHO ****** deleting debug RTENU.MAK binaries
	$(MAKE) /NOLOGO "DEBUG=1" /S /f rtenu.mak CFG="rtenu - Win32 Debug" CLEAN
	@cd nls

# Clean Up slim version
slimclean:
	@cd ..
	@ECHO ****** cleaning slim nls.mak and rtenu.mak obj's, lib's and dll's
	@$(MAKE) /NOLOGO nodebug=1 /S /f nls.mak CFG="nls - Win32 Release" CLEAN
	@$(MAKE) /NOLOGO nodebug=1 /S /f rtenu.mak CFG="rtenu - Win32 Release" CLEAN
	@cd nls


# -----------------------------------------------------------------
#  Notes:
#       - Dependencies are all in $(ENGLISH).MAK or NLS.MAK
#
#       - Use the following command to obtain a list of symbols
#         being exported from DLL:
#
#             DUMPBIN /EXPORTS $(ENGLISH).DLL
#
# -----------------------------------------------------------------

# -----------------------------------------------------------------
# Inference Rules
# -----------------------------------------------------------------
.RC.Res:
    $(rc) $(rcvars) -r -fo $*.Res $*.RC
.$(ENGLISH).Dll:
    $(link) -dll /NOENTRY -out:$*.Dll $**
.$(ARABIC).Dll:
    $(link) -dll /NOENTRY -out:$*.Dll $**
.$(GERMAN).Dll:
    $(link) -dll /NOENTRY -out:$*.Dll $**
.$(ITALIAN).Dll:
    $(link) -dll /NOENTRY -out:$*.Dll $**
.$(JAPANESE).Dll:
    $(link) -dll /NOENTRY -out:$*.Dll $**
.$(KOREAN).Dll:
    $(link) -dll /NOENTRY -out:$*.Dll $**
.$(TAIWAN).Dll:
    $(link) -dll /NOENTRY -out:$*.Dll $**
.$(THAILAND).Dll:
    $(link) -dll /NOENTRY -out:$*.Dll $**


