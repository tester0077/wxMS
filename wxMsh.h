//-----------------------------------------------------------------------------
// Name:        wxMSh.h
// Purpose:     main header for wxMS Mail Screener
// Author:      A. Wiegert
// Copyright:   (c) A. Wiegert
// Licence:     wxWidgets license
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Begin single inclusion of this .h file condition
//-----------------------------------------------------------------------------

#ifndef _WX_MS_H_
#define _WX_MS_H_

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------
#include "wxMsPreProcDefsh.h"   // needs to be first
#include <wx/snglinst.h>
#include <wx/app.h>             // wxApp
#include "wx/stdpaths.h"        // standard paths
#include <wx/busyinfo.h>
#include <wx/progdlg.h>
#include <wx/listctrl.h>
#if defined( WANT_MSVC_INTERNET_TEST )
#include <wx/dialup.h>
#endif
// needed to handle Linux help files *.zip/*.htb
#if !defined( _MSC_VER )
#include "wx/filesys.h"
#include "wx/fs_zip.h"
#endif

// ------------------------------------------------------------------
// globals
// ------------------------------------------------------------------
#if defined( WANT_FILEVERSION )
extern int giMajorVersion;
extern int giMinorVersion;
extern int giBuildNumber;
#endif

// ------------------------------------------------------------------
#include "wxMsPreProcDefsh.h" // needs to be first
// ------------------------------------------------------------------
#include "wxMsFrameh.h"

#include <wx/fileconf.h>
#include "wxMsConfh.h"
#include "wxMsFilter/wxMsFilterConfh.h"
#include "wxMsFilter/wxMsFilterListh.h"
extern iniPrefs_t g_iniPrefs;
#if defined( WANT_LOG_CHAIN )
#include "wxMsFileLogh.h"
#endif
#if defined( WANT_TASKBAR )
#include "wxMetaTaskBarh.h"
#endif
#if defined( WANT_DBGRPRT )
#include "wx/debugrpt.h"
#include <wx/busyinfo.h>
#endif
#include "wxMsAccounth.h"
// ------------------------------------------------------------------
// command line processing

#if defined (WANT_CMD_LINE)
#include "wx/cmdline.h"
#endif
#if defined ( WANT_STD_PATHS )
#include "wx/stdpaths.h"
#endif

//-------------------------------------------------------------------
// Class definition: MyApp
//-------------------------------------------------------------------
class MyFrame;
class MessagesList;

// ------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
  MyApp();
  MyFrame *m_frame;
  wxString m_wsAppName;
  // Override base class virtuals:
  // wxApp::OnInit() is called on application startup and is a good place
  // for the app initialization (doing it here and not in the ctor
  // allows to have an error return: if OnInit() returns false, the
  // application terminates)
  bool OnInit();
  // from http://www.wxwidgets.org/wiki/index.php/Command-Line_Arguments
  int OnExit();
  int OnRun();
#if defined (WANT_CMD_LINE)
  void OnInitCmdLine(wxCmdLineParser& parser);
  bool OnCmdLineParsed(wxCmdLineParser& parser);
#endif

#if defined( WANT_SINGLE_INSTANCE )
  wxSingleInstanceChecker *m_checker;
  ~MyApp();
#endif
#if defined( WANT_DBGRPRT )
  // called when a crash occurs in this application
  virtual void OnFatalException();

  // this is where we really generate the debug report
  void GenerateReport(wxDebugReport::Context ctx);

  // if this function is called, we'll use MyDebugReport which would try to
  // upload the (next) generated debug report to its URL, otherwise we just
  // generate the debug report and leave it in a local file
  void UploadReport(bool doIt) { m_uploadReport = doIt; }
#endif
#if defined( WANT_LOG_CHAIN )
  wxString                m_wsCurLogFileName;
  wxLogChain              *m_pLogChain;
  MyFileLog               *m_pAppLog;
  bool                    CreateNewLog();
#endif
  void CreateConfig( void );
  void RestoreConfig ( void  );
  void SaveConfig( void );
  void MakeConfigCopy( iniPrefs_t& iniPrefsSrc, iniPrefs_t& iniPrefsTarg );
  wxFileConfig    *m_pConfig;
  void SaveAccounts( void );
  void RestoreAccounts( void );

  // Message filer config stuff
  void CreateFilterConfig( void );
  void RestoreFilterConfig ( void  );
  void SaveFilterConfig( void );
  void MakeFilterConfigCopy( iniFilterPrefs_t& iniPrefsSrc,
    iniFilterPrefs_t& iniPrefsTarg );
  wxFileConfig    *m_pFilterConfig;

  wxWindow        *m_pLogWindow;

#if defined( WANT_EXE_LINK_DATE )
  bool RetrieveLinkerTimestamp( wxString& wsDate, wxString& wsYear );
#endif

  // directory for data files - *.log, *.INI * *.xml
  // returned by: wxStandardPaths::Get().GetUserConfigDir();
  wxString              m_wsConfigDir;

  // ------------------------------------------------------------------
  // list of mail accounts to be checked
  MyAccountList         m_AccountList;

  // Filter list
  std::vector<MyFilterListEl> m_FilterList;   ///< std::list holding all filter data
  // ------------------------------------------------------------------
  // for stopping mail check
  //
  wxCriticalSection m_critsect;
//#if defined( _MSC_VER )
  bool  IsInternetAvailable();
//#endif
#if defined( WANT_MSVC_INTERNET_TEST )
  // Internet Connectivity test  - from nettest sample
  // event handlers
  void OnConnected(wxDialUpEvent& event);

  // accessor to dial up manager
  wxDialUpManager *GetDialer() const { return m_dial; }

  // indicates that we're shutting down and all threads should exit
  bool              m_bShuttingDown;
#endif

private:
  bool              m_bSilentMode;
#if defined( WANT_MSVC_INTERNET_TEST )
  wxDialUpManager  *m_dial;
#endif
#if defined( WANT_DBGRPRT )
  bool          m_uploadReport;
  wxBusyInfo    *m_pWaitDialog;
#endif
  wxDECLARE_EVENT_TABLE();

};

DECLARE_APP( MyApp) // allows use of wxGetApp()
// ------------------------------------------------------------------

class MessagesList : public wxFrame
{
public:
  MessagesList();
  double GetDepth(double dNumber, int& iDepth);
  size_t FormatSize(double dNumber,  wxString &wsOutput );
};

#endif  //_WX_MS_H_

// ------------------------------- eof ------------------------------
