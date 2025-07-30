 This file contains the instructions to build the setup.exe provided with
 mosaic distribution.To build setup program Microsoft setup toolkit is required.
 Setup tookit is provided along with Win32SDK.
 To build setup program copy all files except setup.c from c:\mstools\mssetup\samples 
 into the setup directory.C:\mstools is the directory in which win32sdk is installed. 
 Copy mscuistf.dll and mscuistf.lib from \c:\moa\src\setup\bldcui directory.
 c:\moa is the directory in which mosaic files are installed.
 Type nmake to build the setup.exe
