# =====================================================================
# SRC\MAKEFILE - Root mosaic OA RunTIme, CSR, DD Tools, and Utility
#				 programs makefile for Windows/NT 3.1 and Daytona 3.5
#
# ---------------------------------------------------------------------
# mosaic OA NMAKE Options:
#
# Use the table below to determine the additional options for PPLUS/NT
# NMAKE to generate various applications for debugging, profiling and
# and performance tuning information.  The default is to make FAT 
# applications with debugging information. 
#
# Application Information Type            Invoke NMAKE
# ----------------------------            ------------
# Make FAT Version                        nmake (or make all)
# Make SLIM Version                       nmake slim
# Make UNICODE Version                    nmake unicode
# Make CLEAN FAT Version                  nmake clean
# Make CLEAN SLIM Version                 nmake slimclean
# Make CLEAN UNICODE Version              nmake unicodeclean
# Check RCS Files                         nmake check (or nmake check CHKOPT="-d -A")
# Assign Symbolic Names to RCS Files      nmake name RCSNAME="R2C3"
# Create the \CKPT directory              nmake ckpt
# For Working Set Tuner Info              nmake tune=1 (note1)
# For Call Attribute Profiling Info       nmake profile=1 (note2)
#
# Note1: Working Set Tuner and Call Attribute Profiling is available only
#		 for the Intel x86 and Pentium systems
#
# Note2: The "No Debugging", "Working Set Tuning" and "Call Attribute
#		 Profiling" are mutually exclusive (you may use only one option
#		 to compile/link the application.
#
# Note3: This makefile sometimes calls itself recursively!
#
# =====================================================================

# -----------------------------------------------------------------
#	ALLDIRS - List of all the various subdirectories which make
#             up the components of mosaic OA (except for the IBAE)
# -----------------------------------------------------------------
CHKOPT = -d -A

RTDIRS = \
		pch      \
		misc     \
		c_utils  \
		nls      \
		cbtree	 \
		cdcl     \
		cm       \
		comm     \
		dataman  \
		decmath  \
		dsplist  \
		mcn      \
		msw	 \
		netboss  \
		remsrvr  \
		ofb      \
		import   \
		export   \
		prtscdll \
		rt       \
		rtclient \
		rtserver \
		csr      \
		xrefman  \
		cdscl    \
		utility  \
		avi      \
		ezprint  \
		odb      \
		udpinfo  \
		auxcli	 \
		verifone \
		mi

CDSDIRS = \
		cds      \
		cdsv     \
		ql       \
		cdsevent \
		tcl      \
		cdsodbc	 \
		osql	 \
		moasetup \
		svrcfg	

notused =\
#		serial	 \
#		wincom	 \
#		windoc	 \
#		winval	 \
#		olipr50	 \
#		olipr46s \
#		addmast  \


# ----------------------------------------------------------
#	NMAKE these
# ----------------------------------------------------------

all: $(RTDIRS) $(CDSDIRS)

justcds:  $(CDSDIRS)

slim:
	@$(MAKE) /nologo TYPE=slim

clean:
	@ECHO Cleaning all R1V4 source subdirectories...
	@$(MAKE) /nologo TYPE=clean
	@ECHO Cleaning the LIB and BIN directories...
	-@DEL /F /Q lib\*.*			>NUL: 2>&1
	-@DEL /F /Q ..\bin\*.*			>NUL: 2>&1
	@ECHO Copying "Precious" files to ..\BIN and LIB
	-@COPY PRECIOUS\CSLIB.LIB LIB		>NUL: 2>&1
	-@COPY PRECIOUS\SETARGV.OBJ LIB		>NUL: 2>&1
	-@COPY PRECIOUS\*.bat ..\BIN		>NUL: 2>&1
	-@COPY PRECIOUS\*.cmd ..\BIN		>NUL: 2>&1
	-@COPY PRECIOUS\*.exe ..\BIN		>NUL: 2>&1
	-@COPY PRECIOUS\*.ocx ..\BIN		>NUL: 2>&1
	-@COPY PRECIOUS\*.dll ..\BIN		>NUL: 2>&1
	-@COPY PRECIOUS\*.hlp ..\BIN		>NUL: 2>&1
	-@COPY PRECIOUS\*.ico ..\BIN		>NUL: 2>&1
	-@COPY PRECIOUS\*.ini ..\BIN		>NUL: 2>&1
	-@COPY PRECIOUS\ctl3d32.lib ..\BIN	>NUL: 2>&1

slimclean:
	@ECHO Cleaning all R1V4 source subdirectories...
	@$(MAKE) /nologo TYPE=slimclean
	@ECHO Cleaning the LIB_SLIM and BIN_SLIM directories...
	-@DEL /F /Q lib_slim\*.*		>NUL: 2>&1
	-@DEL /F /Q ..\bin_slim\*.*		>NUL: 2>&1
	-@COPY PRECIOUS\CSLIB.LIB LIB_SLIM	>NUL: 2>&1
	-@COPY PRECIOUS\CSLIB.DLL ..\BIN_SLIM	>NUL: 2>&1
	-@COPY PRECIOUS\SETARGV.OBJ LIB_SLIM	>NUL: 2>&1
	-@COPY PRECSLIM\LUA.DLL ..\BIN_SLIM	>NUL: 2>&1 

check:
	@echo Checking R1V4 RCS source sub-directories
	@$(MAKE) /nologo TYPE=check CHKOPT=$(CHKOPT)

name:
	@echo Setting R1V4 RCS revision name $(RCSNAME) for sources
	@$(MAKE) /nologo TYPE=name RCSNAME=$(RCSNAME)
	-@rcs -N$(RCSNAME):	*.mak
	-@rcs -N$(RCSNAME):	makefile
	-@rcs -N$(RCSNAME):	makedflt

ckpt:
	@ECHO Cleaning up existing \CKPT\* directories...
	-@DEL/F/S/Q \ckpt
	@$(MAKE) /nologo /f buildscript

# ----------------------------------------------------------
#	Dependencies
# ----------------------------------------------------------

$(RTDIRS):	dummy
		@cd	$@
		@ECHO.
		@ECHO [$@]
		$(MAKE) /nologo /$(MAKEFLAGS) $(TYPE) VBOPTS="$(VBOPTS)"
		@cd ..

$(CDSDIRS):	dummy
		@cd	$@
		@ECHO.
		@ECHO [$@]
		$(MAKE) /nologo /$(MAKEFLAGS) $(TYPE) VBOPTS="$(VBOPTS)"
		@cd ..

dummy:


