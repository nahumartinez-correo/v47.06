# =====================================================================
# MISC\MAKEFILE
#
#	An "NMAKE" makefile for the Windows/NT 3.1 and/or Daytona 3.5
#   which builds a sub-component of the mosaic OA product
# =====================================================================

!include ..\makedflt

# ----------------------------------------------------------------------
# Use the Visual C++ generated makefile to build the LIB, since it knows
# how to do all that tricky Microsoft stuff.  We put this first, before
# the !include so we won't 'contaminate' the environment and cause the
# VC++ makefile to generate warnings.
# ----------------------------------------------------------------------

#
#	Output files
#
outfiles = \
	$(exe)\tsetup.exe \

#
#	NMAKE these
#
all:  tsetup

tsetup:
	@echo ****** making $(exe)\tsetup.exe
	@cd ..
	$(MAKE) /nologo "DEBUG=1" /S /f tsetup.mak

slim:
	@$(MAKE) /nologo CFG="Win32 Release"

unicode:
	@$(MAKE) /nologo CFG="Win32 Unicode Debug"

unicodeslim:
	@$(MAKE) /nologo CFG="Win32 Unicode Release"

check:
	@rcscheck $(CHKOPTS)

name:
	@echo ****** Naming source file set
	-@rcs -N$(RCSNAME):	*.c* *.h* *.rc makefile

clean:
	@ECHO ******  Deleting $(outfiles)
	-@DEL /Q /F $(outfiles)	>NUL: 2>&1
	@ECHO ******  Deleting  $(objs)\*	
	-@DEL /Q /F $(objs)\* 	>NUL: 2>&1

slimclean:
	@$(MAKE) /nologo CFG="Win32 Release" clean

unicodeclean:
	@$(MAKE) /nologo CFG="Win32 Unicode Debug" clean

unicodeslimclean:
	@$(MAKE) /nologo CFG="Win32 Unicode Release" clean

# -----------------------------------------------------------------
#  Notes:
#       - Dependencies are all in tsetup.MAK
#
# -----------------------------------------------------------------
