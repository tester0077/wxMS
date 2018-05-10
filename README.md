# wxMS
## MailScreener for POP3 Mail Accounts.
The program allows you to preview messages in a POP3 mail account and classify the message according to a customizable filter into legitimate mail - i. e. mail you want to download with your  mail client and SPAM, or mail you want to delete from your POP3 account without ever downloading it in your Email Client.

Typically, the application can be set to check for mail in your POP3 account at start up and at specified intervals thereafter. When new messages are found on the server, the application displays the Status, Sender, Subject and Addressee, the date and the account name, as well as the total message size.

You can set up any number of POP3 accounts, and you can enable or disable any one account at any time.

You can specify the e-mail client to be invoked when new messages have arrived.

As messages arrive, the built-in filter can classify the messages as either SPAM or legitimate mail. The status field indicates the result of the filter condition and messages determined to be SPAM are marked for deletion. You can review the result and either modify the filter or remove the 'Delete'  selection prior to downloading any messages.

The accompanying help file gives more details.

## Prerequisites for the wxMS project

1. wxWidgets 3.1.0 - compiled with MSVC 2015 for static linking
	Uses the macro $(WXWIN_310_2015) = path to the wxWidgets root

2. MSVC 2015 IDE - Express - Community version

3. Inno Install builder from http://www.jrsoftware.org/isinfo.php - uses the ISS package 
	on github.com/stfx/innodependencyinstaller or 
	codeproject.com/Articles/20868/NET-Framework-1-1-2-0-3-5-Installer-for-InnoSetup

4. Appropriate MSVC distributable package

5. RCStamp utility - the executable is included in the main directory 
and is used as part of the 'release' build process to update the 
major, minor & build numbers. To modify either by more than one increment or to modify them in any other way, it is best to edit the .RC 
file by hand to change any one or all to the new starting value. But note, all 'copies' of each number in the .RC must be modified in sync.
It is a command line utility and you can get some help by running it in a DOS window.

6. libcurl - a current version uses libculr 7.59 compiled using the the MSVC IDE command 
prompt and the batch file shown below, executed in winbuild
Libcurl is included in the project via the macro `LIBCURL_2015 = D:\pkg\C\curl-master\`
```where D:\pkg\C\curl-master\ is the main libcurl directory

:: My batch file to build libcurl for Windows
:: run in an MSVC 2015 system window
:: IPV6 is included as default
:: for debug
nmake /f Makefile.vc mode=static VC=14 WITH_DEVEL=../deps WITH_ZLIB=static GEN_PDB=yes DEBUG=yes MACHINE=x86
nmake /f Makefile.vc mode=static VC=14 WITH_DEVEL=../deps WITH_ZLIB=static GEN_PDB=yes DEBUG=no MACHINE=x86```

7. Inno Installer and support packages.
The .sln file is set up to copy the executable, the help file as well as any documentation files to the directory ./Inno/src and the resulting executable will be left in ./Inno/bin

8. An useful helper script to control the Microsoft re-distributable files also came originally from CodeProject see: https://www.codeproject.com/Articles/20868/NET-Framework-Installer-for-InnoSetup.
I beleive the latest code has now moved to Github: https://github.com/stfx/innodependencyinstaller

9. Checksum  Jem Berkes' ver 1.2 MD5sums.exe -- http://www.pc-tools.net/win32/md5sums/

10. UPX see https://upx.github.io/

11. HelpNDoc - The help files are built using the free version of HelpNDoc from https://www.helpndoc.com/
If you will be using the application for anything other than personal use, you may have to
buy a license or use some alternative means to build the help files. Using this application is entirely optional if you want to produce the help files manually or have some other application. You can inspect the .SLN and project files to see how HelpNDoc is integrated to create the help file as part of a build.

12. wxCrafter - The GUI is built using a licensed version of wxCrafter. It is expected that all features used
by this project are covered by the free version, if not you'd have to use 
a different GUI builder of your choice or modify things by hand.

## Other Details
- A small PHP file is included in the directory wxMSver. It is intended to be placed on a 
web server where the version string can be updated as needed and where the application
can check for newer versions. This is optional and can/could be dropped.

- All MSVC preprocessor directives have been moved to the file wxMSPreProcDefsh.h to ensure all 
are up-to-date for all sub projects. Any changes should be made in this file and doing so
will avoid issues with forgetting to propagate all changes to all sub-projects.
This file MUST be included in every source and MUST be the first included file.

- The MIME decoding part uses the library mimetic from Stefano Barbato with permission
to use as part of wxWidgets obtainedm as per comment in the code:
```Licence:     wxWidgets licence
 This file has been copied from the project Mimetic  
(http://codesink.org/mimetic_mime_library.html) and relicenced from the MIT
 licence to the wxWidgets one with authorisation received from Stefano Barbato```
 
- In the directory ./Executables you will find the utility rcStamp used to increment the application version numbers. A separate Github project is available should you want to recompile it or just inspect the source code. If was adapted from a CodeProject article.