/*-----------------------------------------------------------------
 * Name:        wxMsPreProcDefsh.h
 * Purpose:		  by including this file in the main project app header
 *              it is hoped that all sub-projects will be compiled with
 *              the same #defines
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets license
 *-------------------------------------------------------------- */
#ifndef _WX_MS_PRE_PROC_H
#define _WX_MS_PRE_PROC_H
/*
 * Move project #define value to a separate header file
 * remove them from the Project Properties -> Preproc
 * and add them to separate header file for inclusion with all other
 * source files for all sub-projects
 *
 * This, hopefully, prevents the problem of different #defines because
 * some project preprocessor values are not updated consistently
 */
#define COPYRIGHT_YEAR_1        2018;
#define EMAIL_ATTACHMENT_LIMIT  (5 * 1024 * 1024) // 5 MB
#define USER_AGENT              _T("wxMS")
// ------------------------------------------------------------------
#if !defined( wxUSE_UNICODE )   // defining it here seesm too late
#define wxUSE_UNICODE           // for some header
#endif
#if !defined( wxUNICODE )
#define wxUNICODE
#endif
#define WIN32
#define _WINDOWS
#define NOPCH
#define __WXMSW__

// ------------------------------------------------------------------
// common #define s
#define WANT_SINGLE_INSTANCE
#define WANT_DND_NO
#define WANT_APP_PATH
#define WANT_CMD_LINE
#define WANT_FILEVERSION
#define WANT_UNICODE
#define WANT_HELP
#define WANT_STD_PATHS
#define WANT_EXE_LINK_DATE
#define WANT_LOG_WIN
#define WANT_LOG_CHAIN
#define WANT_LOG_LIMIT
#define WANT_LOG_ROTATION           // log rotation
#define WANT_VERSION_IN_TITLE
#define WANT_PROGRESS_DIALOG_NO
#define WANT_NEW_PROGRESS
#define WANT_CAPA
#define WANT_NEW_MIME       // use mimetic directly
#define WANT_CURL           // already used in Check4Update

#define WANT_WX_EMAIL_OLD_UT_NO
#define CURL_STATICLIB
#define WANT_NEW_INTERNET_TEST_NO    // use the wxDialUpManager class
#define HIDE_COLS_1_2
#define WANT_DBGRPRT
#define WANT_MSVC_INTERNET_TEST     // use the wxDialUpManager class
#define WANT_MAIL_CONNECT_TEST_NO   // use mail server connectivity
#define WANT_UIDL_ASSERT            // see wxMsThreadCurlPop3GetMessageBody.cpp - ThreadParseUidls
#define WANT_STARTUP_IN_IDLE        // mail check startup stuff in OnIdle()
                                    // MUST use this option, otherwise toolbar won't show until
                                    // after the mail check returns.
#define WANT_CONNECT_ONLY_NO           // set libcurl connect test to use the CURLOPT_CONNECT_ONLY

#define WANT_NEW_STOP
#define WANT_SEMAPHORE

#define WANT_MONITOR_CHECK        // do we want to force app to primary monitor
#define WANT_SCALING              // shrink app frame at startup is new reso;ution is
                                  // smaller than old
#define WANT_FILTER_EXPLANATION   // display the filter explanation for user
#define WANT_AUTO_START           // want code to allow autostart with Windows

#if defined( WANT_LOG_CHAIN )
#define WS_LOG_FILE_NAME _("wxMsLog.txt")
#endif
// ------------------------------------------------------------------
// version specific #defines
#if defined ( _PUBLIC_ ) 
// for PUBLIC releases use remote server for updates
#  define CHECK_4_UPDATE_URL _T("http://remote/wxMsVer/wxMSversion.php");
#else
// handle my local applications using my local server for updates
#  define CHECK_4_UPDATE_URL _T("http://local/wxMsVer/wxMSversion.php");
#endif  // _PUBLIC_
#if !defined( _MSC_VER )  // overrides for linux
#if !defined ( HAVE_STDINT_H )
#  define HAVE_STDINT_H
#endif
#undef WANT_FILEVERSION
#undef WANT_EXE_LINK_DATE
#undef WANT_VERSION_IN_TITLE
#endif

#endif  // _WX_MS_PRE_PROC_H
// ------------------------------- eof ---------------------------
