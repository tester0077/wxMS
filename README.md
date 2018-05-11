# wxMS
## MailScreener for POP3 Mail Accounts.

#### I am posting this code to Github to, hopefully, make it useful for others because it does what I need at present & I no longer have the time for further major development. Please use the code as you see fit. I will try to help with getting anyone to compile the project under Windows, but my time is limited ...

The program allows you to preview messages in a POP3 mail account and classify the message according to a customizable filter into legitimate mail, i. e. mail you want to download with your  mail client and SPAM, or mail you want to delete from your POP3 account without ever downloading it.
It was inspired by an early freebie version of Mailwasher.

Typically, the application can be set to check for mail in your POP3 account at start up and at specified intervals thereafter. When new messages are found on the server, the application displays the Status, Sender, Subject and Addressee, the date and the account name, as well as the total message size.

You can set up any number of POP3 accounts, and you can enable or disable any one account at any time.

You can specify the e-mail client to be invoked to actually download and display the legitimate messages.

As messages arrive, the built-in filter can classify the messages as either SPAM or legitimate mail. The status field indicates the result of the filter condition and messages determined to be SPAM are marked for deletion. You can review the result and either modify the filter or remove the 'Delete'  selection prior to downloading any messages as needed.

The accompanying help file gives more details.

## Prerequisites for the wxMS project

1. wxWidgets 3.1.0 - compiled with MSVC 2015 for static linking. 
	Uses the environment variable $(WXWIN_310_2015) = path to the wxWidgets root

2. MSVC 2015 IDE - Express - Community version

3. Inno Install builder from http://www.jrsoftware.org/isinfo.php - uses the ISS package 
	from github.com/stfx/innodependencyinstaller. A useful helper script to control the Microsoft re-distributable files also came originally from CodeProject see: https://www.codeproject.com/Articles/20868/NET-Framework-Installer-for-InnoSetup.
 I believe the latest code has now moved to Github: https://github.com/stfx/innodependencyinstaller The .sln file is set up to copy the executable, the help file as well as any documentation files to the directory ./Inno/src and the resulting executable will be left in ./Inno/bin

4. Appropriate MSVC distributable package. The files are expected to be left in ./Inno/bin/myProgramDependencies. These are typically downloaded from Microsoft. Copies of the necessary files for the version of the MSVC IDE are part of the package.

5. RCStamp utility - the executable RCStamp.exe is included in the ./Executables directory 
and is used as part of the 'release' build process to update the 
major, minor & build numbers. To modify either by more than one increment or to modify them in any other way, it is best to edit the .rc file by hand to change any one or all to the new starting value. But note, all 'copies' of each number in the .RC must be modified in sync.
It is a command line utility and you can get some help by running it in a DOS window. You will want to move or copy it to a directory on the PATH so that Windows can find it or you can add the full path in the 'release' build pre-link build event.
It is an adaptation of a CodeProject utility and you can find my modified source and more comments at
tester0077/rcstamp.

6. libcurl - the current version uses libcurl 7.59 compiled using the the MSVC IDE command 
prompt and the batch file shown below, should be executed in winbuild. 
Libcurl is included in the project via the environment variable `$(LIBCURL_2015) = D:\pkg\C\curl-master\`
where D:\pkg\C\curl-master\ is the main libcurl directory.
```
:: My batch file to build libcurl for Windows
:: run in an MSVC 2015 system window
:: IPV6 is included as default
:: for debug
nmake /f Makefile.vc mode=static VC=14 WITH_DEVEL=../deps WITH_ZLIB=static GEN_PDB=yes DEBUG=yes MACHINE=x86
nmake /f Makefile.vc mode=static VC=14 WITH_DEVEL=../deps WITH_ZLIB=static GEN_PDB=yes DEBUG=no MACHINE=x86
```

7. Checksum - Jem Berkes' ver 1.2 MD5sums.exe -- http://www.pc-tools.net/win32/md5sums/

8. UPX file compressor - UPX 3.03w Markus Oberhumer, Laszlo Molnar & John Reiser   Apr 27th 2008 see https://upx.github.io/

9. HelpNDoc - The help files are built using the free version of HelpNDoc from https://www.helpndoc.com/
If you will be using the application for anything other than personal use, you may have to
buy a license or use some alternative means to build the help files. Using this application is entirely optional if you want to produce the help files manually or have some other application. You can inspect the .SLN and project files to see how HelpNDoc is integrated to create the help file as part of a build.

10. wxCrafter - The GUI is built using a licensed version of wxCrafter. It is expected that all features used by this project are covered by the free version, if not you'd have to use 
a different GUI builder of your choice or modify things by hand.

## Other Details
- A small PHP file is included in the directory wxMsVer. It is intended to be placed on a 
web server into the directory %HtmlRoot%/wxNsVer where the version string can be updated as needed and where the application
can check for newer versions. This is optional and can/could be dropped. The Web site URLs are set up in wxMsPreProcDefs.h and a different URL can be specified for the debug and release versions.

- All MSVC preprocessor directives have been moved to the file wxMSPreProcDefsh.h to ensure all 
are up-to-date for all sub projects. Any changes should be made in this file and doing so
will avoid issues with forgetting to propagate all changes to all sub-projects.
This file MUST be included in every source and MUST be the first included file. 

- The MIME decoding part uses the library mimetic from Stefano Barbato with permission
to use as part of wxWidgets obtained,  as per comment in the code:
```Licence:     wxWidgets licence
 This file has been copied from the project Mimetic  
(http://codesink.org/mimetic_mime_library.html) and relicenced from the MIT
 licence to the wxWidgets one with authorisation received from Stefano Barbato
 ```

 - verLib - a small library used to obtain the application version information from the 
executable at run-time to display as part of 'About' dialog or wherever needed.

- exeHdrLib - a small library used to get information about the executable, in this case the linker date to get an accurate date for the build.

 - pre-compiled version of both my current version executables - 0.5 build 320 as well as the Inno installer executable for the same version along with the md5 checksum files.
 
- wxMS keeps its configuration information in a directory in %USER%, specifically in 
C:\Users\userName\AppData\Roaming\wxMS. The directory will be created if it does not exist. These files will be wxMS.INI - configuration information and wxMSFilter.Ini - the SPAM filter information. The same directory is also used to keep the help file wxMSHelp.chm, the install instructions and the change log, as well as the log file wxMSLog.txt. 

- All preprocessor directives have been moved to the file wxMsPreProcDefsh.h to make it easier to keep them all in sync between the main as all sub projects.

- Some work has been done to port the application to Ubuntu using CMake and CodeLite. See the included Codelite *.project files as well as the CMakeLists.txt files. They may need updating.