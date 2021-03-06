//-------------------------------------------------------------------
// Name:        wxMS.cpp
// Purpose:     main file for wxMS Mail Screener
// Author:      A. Wiegert
//
// Copyright:   (c) A. Wiegert
// Licence:     wxWidgets license
//-------------------------------------------------------------------
//-------------------------------------------------------------------
// Note __VISUALC__ is defined by wxWidgets, not by MSVC IDE
// and thus won't be defined until some wxWidgets headers are included
#if defined( _MSC_VER )
#  if defined ( _DEBUG )
     // this statement NEEDS to go BEFORE all headers
#    define _CRTDBG_MAP_ALLOC
#  endif
#endif
//-------------------------------------------------------------------
#include "wxMsPreProcDefsh.h"   // needs to be first
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// For all others, include the necessary headers (this file is
// usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//-------------------------------------------------------------------
// Header of this .cpp file
//-------------------------------------------------------------------
#include <wx/msgdlg.h>

#include "wxMsh.h"
#if defined (WANT_CMD_LINE)
#include <wx/cmdline.h>
#include <wx/filename.h>
#include "wxMsCmdlineh.h"
#endif

#include "wxMsConfh.h"

#if defined( WANT_EXE_LINK_DATE )
#include <wx/stdpaths.h>
#include "exeDosHdrh.h"
#endif
#if defined( WANT_DBGRPRT )
#include "wx/ffile.h"
#include <wx/busyinfo.h>
#include "wx/debugrpt.h"
#include "wxMsDebugRprth.h"
#endif
#include "wxMsAccounth.h"

// ------------------------------------------------------------------
// Note __VISUALC__ is defined by wxWidgets, not by MSVC IDE
// and thus won't be defined until some wxWidgets headers are included
#if defined( _MSC_VER )
// only good for MSVC
// this block needs to go AFTER all headers
#include <crtdbg.h>
#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif
#endif
// ------------------------------------------------------------------
// globals
// ------------------------------------------------------------------
#if defined( WANT_FILEVERSION )
int giMajorVersion = 0;
int giMinorVersion = 0;
int giBuildNumber = 0;
#endif
#if defined( WANT_SINGLE_INSTANCE )
wxSingleInstanceChecker *gp_checker;
#endif
// ------------------------------------------------------------------
#include <wx/listimpl.cpp>
WX_DEFINE_LIST( MyAccountList );

//-------------------------------------------------------------------
// wxWidgets macro: Declare the application.
//-------------------------------------------------------------------

// Create a new application object: this macro will allow wxWidgets
// to create the application object during program execution (it's
// better than using a static object for many reasons) and also
// declares the accessor function wxGetApp() which will return the
// reference of the right type (i.e. the_app and not wxApp).
IMPLEMENT_APP(MyApp)
MyApp   *theApp;

wxBEGIN_EVENT_TABLE(MyApp, wxApp)
#if defined( WANT_MSVC_INTERNET_TEST )
    EVT_DIALUP_CONNECTED(MyApp::OnConnected)
    EVT_DIALUP_DISCONNECTED(MyApp::OnConnected)
#endif
#if defined( WANT_NEW_STOP )
  EVT_QUERY_END_SESSION(  MyApp::OnAppQueryEndSession )
  EVT_END_SESSION(MyApp::OnAppEndSession)
#endif
wxEND_EVENT_TABLE()

// ------------------------------------------------------------------

MyApp::MyApp()
{
  m_frame = NULL;
  m_pConfig = NULL;
#if defined( WANT_SINGLE_INSTANCE )
  gp_checker = NULL;
#endif
#if defined( WANT_LOG_CHAIN )
  m_pLogChain = NULL;
  m_pAppLog = NULL;
#endif
#if defined( WANT_NEW_STOP )
  m_bShuttingDown = false;
#endif
  m_pFilterConfig = NULL;
  m_pLogWindow = NULL;
  m_bSilentMode = false;
// Note __VISUALC__ is defined by wxWidgets, not by MSVC IDE
// and thus won't be defined until some wxWidgets headers are included
#if defined( _MSC_VER )
  _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
  // with things setup to identify memory leaks
  // there seem to be some leaks I can't find, but this seems to work
  // at least for some ?? adding this code seems to shift the block
  // numbers; I get the same number of leaks - and the same sizes,
  // but the block numer changes
//  _CrtSetBreakAlloc(46028);
#endif
#if defined( WANT_DBGRPRT )
  // user needs to explicitly enable this
  m_uploadReport = false;

  // call this to tell the library to call our OnFatalException()
  wxHandleFatalExceptions();
#endif
}

//-------------------------------------------------------------------
// Public methods
//-------------------------------------------------------------------

MyApp::~MyApp()
{
#if defined( WANT_SINGLE_INSTANCE )
  if ( gp_checker )
  {
    delete gp_checker;
    gp_checker = NULL;
  }
#endif
#if defined( WANT_TASKBAR )
  delete m_pTaskBarIcon;
#endif
#if defined( WANT_MSVC_INTERNET_TEST )
  if ( m_dial )
    delete m_dial;
#endif
  return;
}

// ------------------------------------------------------------------
/**
 * Only get here from red 'x' on main frame or
 * when the system shuts down and sends the  message to OnAppEndSession().
 * Therefore any saving of critical data MUST be done here   !!!!!!!!!!!!!!!!!!!!!!
 * Stopping the threads needs/ought to be done in the main frame dtor
 */
/** More info from an old discussion ( 2008  & msw 2.8.7
 * from: http://wxwidgets.10942.n7.nabble.com/msw-2-8-7-Problem-saving-application-settings-when-system-is-shut-down-td24744.html
 * If this code makes it into the library, maybe it should be mentioned
 * in the docs that the wxApp dtor will not get called if the
 * WM_ENDSESSION is received, so if anyone has important cleanup code in
 * his dtor, he can move the code to wxApp::OnExit(). 
 */
int MyApp::OnExit()
{
  // clean up
  SaveConfig();
  // save filters
  SaveFilterConfig();
  return 0;
}
#if defined( WANT_NEW_STOP )

// ------------------------------------------------------------------
/**
 * Called when the OS is trying to kill the application and it is called
 * before OnAppEndSession()
 * In either case we have 5 seconds to clean up, before the shut down is forced
 * see: https://docs.microsoft.com/en-us/windows/desktop/Shutdown/shutdown-changes-for-windows-vista
 */
void MyApp::OnAppQueryEndSession(wxCloseEvent& event)
{
  // called when the system is shutting down
  // can be tested using 
  // C:\Program Files (x86)\Windows Kits\10\App Certification Kit\rmlogotest.exe"
  // which needs the process ID
  // see also comments in wxCloseEvent handler
  // it seems we can also use 
  // GetLoggingOff () const  
  // Returns true if the user is just logging off or false if the system is shutting down. 
  // This method can only be called for end session and query end session events, it doesn't 
  // make sense for close window event. 
  // if multi-threading, here we can check and stop all threads if any are running.
  // see the thread example in MSVC 2015/examples
  // 
#if defined( WANT_NEW_STOP )
  m_bShuttingDown = true;
#endif

  // all shutdown related work is best done in main frame dtor
	event.Skip();

}

// ------------------------------------------------------------------
void MyApp::OnAppEndSession(wxCloseEvent& event)
{
	event.Skip();
}
#endif
// ------------------------------------------------------------------
int MyApp::OnRun()
{
  int exitcode = wxApp::OnRun();
  return exitcode;
}
// ------------------------------------------------------------------
// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
  theApp = this;
  m_dial = NULL;
#if defined( WANT_LOG_CHAIN )
  m_pLogChain = NULL;
#endif
  // stops the warning dialog from libpng "iCCP: known incorrect sRGB profile
  wxLogNull logNo;
  ::wxInitAllImageHandlers();  // IMPORTANT for wxCrafter
#if !defined( _MSC_VER )
  extern void handler( int sig );
  signal( SIGSEGV, handler );
#endif
  // ------------------------------------------------------------------
  // need to find app name before we use it to make up
  // the working directory path
  wxString msg;
  m_wsAppName = GetAppName();
#if defined ( WANT_STD_PATHS )
  // needed ahead of config
  // for Win 7 at least we have to write the file to the user
  // accessible AppData area because the executable directory
  // is off-limits
  // This means we have to create the data directory wxAPP in
  // the user's Home directory
  // XP:     c:/Documents and Settings/userName/Application data
  // Vista:  c:/Users/UserName/AppData/Roaming
  // Win7    c:/Users/UserName/AppData/Roaming
  // Other info says it would be better to have the in AppData/LocalLow
  // but the Inno installer has no way to set this directory based on
  // OS, so I will simply stay with ..../roaming

  // This code sets up the main config directory, though the variable
  // is not used throughout because it is local to the app.
  // Instead similar code is used to get the same information where needed
#if defined( _MSC_VER )
  m_wsConfigDir = wxStandardPaths::Get().GetUserConfigDir() +
    _T("\\") + m_wsAppName;
#else
  // needed to handle Linux help files *.zip/*.htb
  wxFileSystem::AddHandler(new wxZipFSHandler);
  /**
   * Setup default directory stuff for Linux
    * as per https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
    * summary at:
    https://stackoverflow.com/questions/1024114/location-of-ini-config-files-in-linux-unix

    $XDG_CONFIG_HOME (default: "$HOME/.config"): user-specific configuration files.
   *
   in my case $HOME -> /home/arnold -> /home/$USER/.wxms
   * see also wxMsConfig.cpp where the actual config file is created/used
   */
  m_wsConfigDir = wxStandardPaths::Get().GetUserConfigDir() +
    _T("/") + m_wsAppName;
#endif

  // MUST ensure the config directory exists, just in case user
  // or someone else deletes it; otherwise app cannot function
  // Sometimes fails because a file with the same name exists,
  // offer to delete the file and start over
  bool firstTry = true;
tryAgain:
  if (!::wxDirExists(m_wsConfigDir))
  {
    // for Linux we just want current user permissions
    // Note the config file permissions need to be set if and when
    // the file is created in wxMsConfig
    if (::wxMkDir( m_wsConfigDir, wxPOSIX_USER_READ | wxPOSIX_USER_WRITE ) != 0)
    {
      wxFileName wfnConfig(m_wsConfigDir);
      if (firstTry && wfnConfig.FileExists())
      {
        wxString wsT;
        wsT.Printf(_T("Can't create directory %s\nA file with the name '%s' existsts\n") +
          _("Delete the file and create the directory?"),
          wfnConfig.GetFullPath(), wfnConfig.GetName());
        int iResponse = wxMessageBox(wsT, _T("Confirm"), wxICON_QUESTION | wxYES_NO);
        if (iResponse == wxYES)
        {
          firstTry = false;
          ::wxRemoveFile(m_wsConfigDir);
          goto tryAgain;
        }
        else
          return false;
      }
      else
      {
        wxMessageBox(_T("Can't create: ") + m_wsConfigDir
          + _("\nCannot continue - Exiting") + _("\nHint: Perhaps a file by the name ")
          + m_wsAppName + _(" existsts in ") + wxStandardPaths::Get().GetUserConfigDir(),
          _T("Error"), wxOK);
        return false;
      }
    }
  }
#endif
#if defined( WANT_SINGLE_INSTANCE )
  const wxString name = wxString::Format(_T("%s-%s"), m_wsAppName, wxGetUserId() );
  gp_checker = new wxSingleInstanceChecker(name);
  if ( gp_checker->IsAnotherRunning() )
  {
    wxLogError( _("Another program instance is already running, aborting.") );
    // does not show either, because the debugger termiates as well in this case
    wxLogDebug( _("Another program instance is already running, aborting.") );
    wxBell();
    wxString wsMsg;
    wsMsg.Printf(  _("Another program instance of %s-%s is already running, aborting."),
      m_wsAppName,  wxGetUserId() );
    wxMessageBox( wsMsg, _("Error"), wxOK );
    delete gp_checker; // OnExit() won't be called if we return false
    gp_checker = NULL;
    return false;
  }
#endif

// ------------------------------------------------------------------
#if defined ( WANT_CMD_LINE )
    // Parse command line
    wxString cmdFilename;
    wxCmdLineParser cmdParser(g_cmdLineDesc, argc, argv);
    int res;
    {
      wxLogNull log;
      // Pass false to suppress auto Usage() message
      res = cmdParser.Parse(false);
    }
    // Check if the user asked for command-line help
    if (res == -1 || res > 0
      || cmdParser.Found( _T("h") ) || cmdParser.Found( _T("?") ) )
    {
      cmdParser.Usage();
      return false;
    }
    // Check if the user asked for the version
    // when runing, this displays a dialog box with
    // the message
    if (cmdParser.Found( _T("v")))
    {
#ifndef __WXMSW__
      wxLog::SetActiveTarget(new wxLogStderr);
#endif
#if defined( WANT_LOG_VERSION_HEADER )
      wxString msg;
      wxString date(wxString::FromAscii(__DATE__));
      msg.Printf(
        _T("%s, 2007 Version %d.%d - Build: %d, %s"),
        m_frame->m_frameProgName,
        giMajorVersion, giMinorVersion, giBuildNumber,
        (const wxChar*) date);
      wxLogMessage(msg);
#endif
      return false;
    }
    // Check for debug level
    long debugLevel = 0;
    if (cmdParser.Found( _T("d"), & debugLevel))
    {
    }
    // Check for a project filename
    if (cmdParser.GetParamCount() > 0)
    {
      cmdFilename = cmdParser.GetParam(0);
      // Under Windows when invoking via a document
      // in Explorer, we are passed the short form.
      // So normalize and make the long form.
      wxFileName fName(cmdFilename);
      fName.Normalize(wxPATH_NORM_LONG|wxPATH_NORM_DOTS|
      wxPATH_NORM_TILDE|wxPATH_NORM_ABSOLUTE);
      cmdFilename = fName.GetFullPath();
    }
#endif

  // Make an instance of your derived frame. Passing NULL (the default value
  // of MyFrame's constructor is NULL) as the frame doesn't have a parent
  // since it is the main application window.
  m_frame = new MyFrame( NULL );
#if defined( WANT_VERSION_IN_TITLE )
  // Set up the main frame title with version & build info
  // Note: the base frame MUST have the main frame title set to:
  // "%s - %s - Version %d.%d - Build: %d"
  wxString wsFrameTitle = m_frame->GetLabel();
  wxString wsT;
  wsT.Printf( wsFrameTitle, m_wsAppName, _("Mail Screener" ),
    giMajorVersion, giMinorVersion, giBuildNumber );
  m_frame->SetLabel( wsT );
#else
   m_frame->SetTitle( m_wsAppName +_T(" - Mail Screener" ) );
#endif

  CreateConfig(); //create this first - for .INI file
  RestoreConfig();
  if( g_iniPrefs.data[IE_OPT_LAST_CONF_FILE].dataCurrent.wsVal.IsEmpty() )
    g_iniPrefs.data[IE_OPT_LAST_CONF_FILE].dataCurrent.wsVal = m_wsConfigDir;
#if defined( WANT_SCALING )
  // shrink app frame at startup is new reso;ution is smaller than old
  int x, y;
  wxDisplaySize( &x, &y );
  if ( x < g_iniPrefs.data[IE_RESOLUTION_X].dataCurrent.lVal )
  {
    g_iniPrefs.data[IE_FRAME_W].dataCurrent.lVal *= 
      (x / g_iniPrefs.data[IE_RESOLUTION_X].dataCurrent.lVal);
  }
  if ( y < g_iniPrefs.data[IE_RESOLUTION_Y].dataCurrent.lVal )
  {
    g_iniPrefs.data[IE_FRAME_W].dataCurrent.lVal *= 
      (y / g_iniPrefs.data[IE_RESOLUTION_Y].dataCurrent.lVal);
  }
#endif
#if defined( WANT_MONITOR_CHECK )
  const size_t count = wxDisplay::GetCount();
  int  iWin = wxDisplay::GetFromWindow( m_frame );
  // sets both size & position
  m_frame->SetSize( g_iniPrefs.data[IE_FRAME_X].dataCurrent.lVal, 
    g_iniPrefs.data[IE_FRAME_Y].dataCurrent.lVal, 
    g_iniPrefs.data[IE_FRAME_W].dataCurrent.lVal,
    g_iniPrefs.data[IE_FRAME_H].dataCurrent.lVal );
  iWin = wxDisplay::GetFromWindow( m_frame );
  if ( iWin == wxNOT_FOUND )    // the coordinates are on a different screen
  {
    // repositon it to the primary screen, whichever that ends up as.
    // in this way, I expect that the user can then carry on, resizing
    // and repositioning as needed
    m_frame->SetSize( 10, 10, 
    g_iniPrefs.data[IE_FRAME_W].dataCurrent.lVal,
    g_iniPrefs.data[IE_FRAME_H].dataCurrent.lVal );
  }
#else
  m_frame->SetSize( g_iniPrefs.data[IE_FRAME_X].dataCurrent.lVal,
    g_iniPrefs.data[IE_FRAME_Y].dataCurrent.lVal,
    g_iniPrefs.data[IE_FRAME_W].dataCurrent.lVal,
    g_iniPrefs.data[IE_FRAME_H].dataCurrent.lVal );
#endif
  RestoreAccounts();
  // see if any accounts are active/enabled
  m_frame->m_bAnyAccountsActive = m_frame->GetAccountInfo();
  m_frame->RestoreColWidths();
  m_frame->RestoreColLabels();
  m_frame->InitializeMailGrid();
  m_frame->m_lNTopLines = g_iniPrefs.data[IE_OPT_MAX_TOP_LINES].dataCurrent.lVal;
  // Restore filters
  CreateFilterConfig();
  RestoreFilterConfig();
#if defined( WANT_LOG_WIN ) // want log window etc
  if( !g_iniPrefs.data[IE_LOG_WIN_WANTED].dataCurrent.bVal )
  {
    m_frame->m_pLogWindowSave = m_frame->GetSplitterMain()->GetWindow2();
    m_frame->GetSplitterMain()->Unsplit();
  }
  else
  {
    m_frame->GetSplitterMain()->SetSashPosition(
      g_iniPrefs.data[IE_SASH_POS].dataCurrent.lVal );
  }
#endif
#if defined( WANT_LOG_CHAIN )
  g_iniPrefs.data[IE_LOG_DIR_PATH].dataDefault.wsVal =
    g_iniPrefs.data[IE_LOG_DIR_PATH].dataCurrent.wsVal = m_wsConfigDir;
  m_wsCurLogFileName = g_iniPrefs.data[IE_LOG_DIR_PATH].dataCurrent.wsVal + _T("/") + WS_LOG_FILE_NAME;
  if ( g_iniPrefs.data[IE_LOG_FILE_WANTED].dataCurrent.bVal )
  {
    // log size limit & rotation is handled by called routine
    CreateNewLog();
  }
#endif    // WANT_LOG_CHAIN
  // Show the frame as it's created initially hidden.
  m_frame->Show(true);
  m_frame->GetGaugeProgress()->SetValue( 0 );
  
//  ::wxYield();  // causes crash
  // adding this delay here does not show tool bar icons
  m_frame->Refresh();
  ::wxMilliSleep( 500 ); // give GUI a chance to settle before we check mail
#if defined( WANT_MSVC_INTERNET_TEST )
  // Init dial up manager
  m_dial = wxDialUpManager::Create();
  if ( !m_dial->IsOk() )
  {
      wxLogError(wxT("Initialization of internet connectivity test failed."));
#if wxUSE_LOG
      wxLog::GetActiveTarget()->Flush();
#endif // wxUSE_LOG
      // do it here, OnExit() won't be called
      delete m_dial;
      m_dial = NULL;
      // we'll keep going even it it fails, though there is little point
//      return false;
  }
#else
  if( m_frame->CheckConnectivity())
  {
    if( g_iniPrefs.data[IE_OPT_AUTO_UPDATE_CHECK].dataCurrent.bVal )
      m_frame->Check4Update( false );
    // check & download mail at start up?
    if( g_iniPrefs.data[IE_CHECK_MAIL_STARTUP].dataCurrent.bVal )
    {
      // check default account as left by user last time
      m_frame->Check4NewMail();
    }
  }
#endif
#if defined( _MSC_VER )
#if !defined( WANT_STARTUP_IN_IDLE )
  // if this is used the tool bar will NOT show until after the mail check returns.
  if( IsInternetAvailable() )
  {
    if( g_iniPrefs.data[IE_OPT_AUTO_UPDATE_CHECK].dataCurrent.bVal )
      m_frame->Check4Update( false );
    // check & download mail at start up?
    if( g_iniPrefs.data[IE_CHECK_MAIL_STARTUP].dataCurrent.bVal )
    {
      // check default account as left by user last time
//      ::wxYield();
//      m_frame->Refresh();
//      ::wxSleep( 10 );
      m_frame->Check4NewMail();
    }
  }
  else
  {
    wsT.Printf( _("Internet Connection is unavailable!\n") );
    wxMessageBox( wsT, "Error", wxOK );
    //wxLogMessage( wsT );  // does not show
  }
#endif
#endif
  // start the mail check timer if user wants it so
  if ( g_iniPrefs.data[IE_SCHEDULE_MAIL_CHECK].dataCurrent.bVal )
  {
    m_frame->GetMailTimerPtr()->Start( 60 * 1000 );
  }
  // Return true to tell program to continue (false would terminate).
  return true;
}

// ------------------------------------------------------------------
void MyApp::OnConnected(wxDialUpEvent& event)
{
  const wxChar *msg;
  if ( event.IsOwnEvent() )
  {
    msg = event.IsConnectedEvent() ? wxT("Successfully connected")
                                    : wxT("Dialing failed");
    wxLogStatus(wxEmptyString);
  }
  else
  {
    msg = event.IsConnectedEvent() ? wxT("Just connected!")
                                    : wxT("Disconnected");
  }
  wxLogMessage(msg);
}

// ------------------------------- eof ------------------------------
