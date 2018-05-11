Prerequisites for the wxMS project

wxWidgets 3.1.0 - compiled with MSVC 2015 for static linking
	Uses the macro $(WXWIN_310_2015) = path to wxWidgets root

MSVC 2015 IDE - Express - Community version

Inno Install builder from http://www.jrsoftware.org/isinfo.php - uses the ISS package 
	on github.com/stfx/innodependencyinstaller or 
	codeproject.com/Articles/20868/NET-Framework-1-1-2-0-3-5-Installer-for-InnoSetup

Appropriate MSVC distributable package

RCStamp utility - the executable is included in the main directory 
and is used as part of the 'release' build process to update the 
major, minor & build numbers.

To modify either by more than one increment, it is best to edit the .RC 
file by hand to change any one or all to the new starting value.

It is a command line utility and you can get some help by running it in a DOS window.

Libcurl - a current version uses libculr 7.59 compiled using the the MSVC IDE command 
prompt and the batch file shown below, executed in winbuild
Libcurl is included in the project via the macro
LIBCURL_2015 = D:\pkg\C\curl-master\
where D:\pkg\C\curl-master\ is the main libcurl directory
==============
:: My batch file to build libcurl for Windows
:: run in an MSVC 2015 system window
:: IPV6 is included as default
:: for debug
nmake /f Makefile.vc mode=static VC=14 WITH_DEVEL=../deps WITH_ZLIB=static GEN_PDB=yes DEBUG=yes MACHINE=x86
nmake /f Makefile.vc mode=static VC=14 WITH_DEVEL=../deps WITH_ZLIB=static GEN_PDB=yes DEBUG=no MACHINE=x86
=================

The help files are built using the free version of HelpNDoc from https://www.helpndoc.com/
If you will be using the appliaction for anything other than personal use, you may have to
buy a license or use some alternative means to build the help files.

The GUI is built using a licensed version of wxCrafter. It is expected that all features used
by this project are covered by the free version, if not you'd have to moved that part to
a different GUI builder of your choice or modify things by hand.

A small PHP file is included in the directory wxMSver. It is intended to be placed on a 
web server where the version string can be updated as needed and where the application
can check for newer versions. This is optional and can/could be dropped.

All MSVC preprocessor directives have been moved to the file wxMSPreProcDefsh.h to ensure all 
are up-to-date for all sub projects. Any changes should be made in this file and doing so
will avoid issues with forgetting to propagate all changes to all sub-projects.

This file MUST be included in every source and MUST be the first included file.

The MIME decoding part uses the library mimetic from Stefano Barbato with permission
to use as part of wxWidgets obtainedm as per comment in the code:

Licence:     wxWidgets licence

 This file has been copied from the project Mimetic
 
(http://codesink.org/mimetic_mime_library.html) and relicenced from the MIT
 licence 
to the wxWidgets one with authorisation received from Stefano Barbato