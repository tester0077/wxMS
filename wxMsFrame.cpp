//-------------------------------------------------------------------
// Name:        wxMsFrame .cpp
// Purpose:     main frame file for wxMS Mail Screener
// Author:      A. Wiegert
//
// Copyright:
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
#include "wxMsPreProcDefsh.h"   // needs to be first
// ------------------------------------------------------------------
//-------------------------------------------------------------------
// Standard wxWidgets headers
//-------------------------------------------------------------------
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
#include <wx/event.h>
#include <wx/msgdlg.h>

#include "wxMsFrameh.h"
#if defined( WANT_FILEVERSION )
#include "fileVerh.h"
#endif
#if defined( WANT_TASKBAR )
#include "rc/smile.xpm"
#endif
#include <list>
#include "curl/curl.h"

// ------------------------------------------------------------------
// Note __VISUALC__ is defined by wxWidgets, not by MSVC IDE
// and thus won't be defined until some wxWidgets headers are included
#if defined( _MSC_VER )
// this block needs to AFTER all headers
#include <crtdbg.h>
  #ifdef _DEBUG
    #define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
    #define new DEBUG_NEW
  #endif
#endif
// ------------------------------------------------------------------
extern MyApp *theApp;

//-------------------------------------------------------------------

// The event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
// The reason why the menuitems and tools are given the same name in the
// XRC file, is that both a tool (a toolbar item) and a menuitem are designed
// to fire the same kind of event (an EVT_MENU) and thus I give them the same
// ID name to help new users emphasize this point which is often overlooked
// when starting out with wxWidgets.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(wxID_EXIT,                     MyFrame::OnExit )
  EVT_GRID_COL_SIZE(                      MyFrame::OnMailGridColSize )
  // mail grid context menu handlers
  EVT_MENU( wxID_CONTEXT_DELETE_MSG,      MyFrame::OnContextDelMsg )
  EVT_MENU( wxID_CONTEXT_PREVIEW_MSG,     MyFrame::OnContextPreview )
  EVT_MENU( wxID_CONTEXT_MARK_DEL_MSG,    MyFrame::OnContextMarkDelMsg )
  EVT_MENU( wxID_CONTEXT_MARK_BOUNCE_MSG, MyFrame::OnContextMarkBounceMsg )
  EVT_MENU( wxID_CONTEXT_ADD_BLACK_MSG,   MyFrame::OnContextAddBlackMsg )
  EVT_MENU( wxID_CONTEXT_ADD_FRIEND_MSG,  MyFrame::OnContextAddFriendMsg )
  // for checkboxes in mailgrid
  EVT_CHECKBOX( wxID_ANY,                 MyFrame::CheckboxHandler)
  EVT_TIMER( MAIL_CHECK_TIMER_ID,         MyFrame::OnMailCheckTimer)
  EVT_TIMER( MAIL_SERVER_CHECK_TIMER_ID,  MyFrame::OnMailConnectTimer)
  EVT_TIMER( MAIL_DISPLAY_TIMER_ID,       MyFrame::OnMailDisplayTimer)
  EVT_IDLE(                               MyFrame::OnIdle)
END_EVENT_TABLE()

// ------------------------------------------------------------------
MyFrame::MyFrame( wxWindow* parent )
  : MyFrameBase( parent ), m_tMailCheckTimer(this, MAIL_CHECK_TIMER_ID ),
  m_tMailDisplayTimer(this, MAIL_DISPLAY_TIMER_ID ),
  m_tMailServerConnectCheckTimer( this, MAIL_SERVER_CHECK_TIMER_ID )
{
  m_iRightClickRow = 0;
  m_iWheelRotation = 0;
  m_wasColLabels.Clear();
  m_wasColLabels.Empty();
  m_walColWidths.Clear();
  m_walColWidths.Empty();
  m_bStop = false;

#if defined( _MSC_VER )
  m_pAppProgress = NULL;
  m_pAppProgress = new  wxAppProgressIndicator(this);
  if( m_pAppProgress->IsAvailable() )
  {
    m_pAppProgress->SetRange( 100 );
    m_bHaveAppProgress = true;
  }
  else
  {
    m_pAppProgress = NULL;
    m_bHaveAppProgress = false;
  }
#endif
  // 1 minute interval
  m_iMailTimerTicks = 0;
  m_iConnectCheckTimerTicks = 0;
  m_tMailCheckTimer.Start(1000 * 60 );
  m_bRunning = m_bCancelled = false;
  m_lNTopLines = g_iniPrefs.data[IE_OPT_MAX_TOP_LINES].dataCurrent.lVal;

  // Update the mail grid display
  // It is also possible to use event tables, but dynamic binding is simpler.
  Bind( wxEVT_THREAD, &MyFrame::OnUpdateDisplay, this );
  m_Pop3MsgList.clear();

#if defined( WANT_FILEVERSION )
//#include "fileVerh.h"
    wxFileVersion fVer;
    // open the executable's version info
    bool bErr = fVer.Open();
    if( bErr == false )
    {
      wxString wsT;
      wsT.Printf( _("Cannot find the .EXE file version, exiting !") );
      wxMessageBox( wsT, _("Error") );
      //return;
    }
    wxString wsFV = fVer.GetFixedFileVersion();
    // and   close the
//    fVer.Close();
    giMajorVersion = fVer.GetMajorVersion();
    giMinorVersion =  fVer.GetMinorVersion();
    giBuildNumber  = fVer.GetBuildNumber();
    //  fVer.GetSpecialBuild();
#endif
#if defined( WANT_HELP )
#if defined( WANT_ZIP_HELP )
  //wxFileSystem::AddHandler(new wxZipFSHandler);
  //m_pProvider = new wxHelpControllerHelpProvider;
  //wxHelpProvider::Set( m_pProvider );
  m_pProvider->SetHelpController(GetHelpController());
  if( !GetHelpController()->AddBook(wxFileName(_T("wx??Help.zip"))))
  {
    wxLogError( _("Cannot initialize the Help system."));
  }
#else
#if defined( _MSC_VER )
  // help file now is in the %USER%/AppData/Roaming   for Vista
  wxString wsCurPath = wxStandardPaths::Get().GetResourcesDir();
  wxFileName wfnHelpFile;
//  wxString wsTT = wxGetApp().m_wsConfigDir;
  wfnHelpFile.AssignDir( wxGetApp().m_wsConfigDir );
  // and construct the help file name
  wxString wsHelpFile = wxGetApp().m_wsAppName;
  wfnHelpFile.SetName( wsHelpFile + _T("Help") );
  wfnHelpFile.SetExt( _T("chm") );
  wsHelpFile = m_wsCurHelpFile = wfnHelpFile.GetFullPath();
  if( ! ::wxFileExists( m_wsCurHelpFile ) )
  {
    wxLogDebug( _("Cannot find the Help file %s."), m_wsCurHelpFile);
    m_bFoundHelpFile  = false;
    m_wsCurHelpFile = wxEmptyString;
  }
  else
    m_bFoundHelpFile  = true;
  m_pProvider = new wxHelpControllerHelpProvider;
  wxHelpProvider::Set( m_pProvider );
  m_pProvider->SetHelpController(GetHelpController());
  // Note: this code only checks if the basic MS Help DLLs are available
  // not whether or not the help file can be found
  if( !GetHelpController()->Initialize( m_wsCurHelpFile ) )
  {
    wxLogError( _("Cannot initialize the MS HTML Help system."));
//    m_bFoundHelpFile = false;
  }
#else
  // under Linux
  wxFileName wfnHelpFile;
  m_msHtmlHelp.UseConfig(wxConfig::Get());
  m_msHtmlHelp.SetTempDir(wxT("."));

  wfnHelpFile.AssignDir( wxGetApp().m_wsConfigDir + _T("/Help") );
  // and construct the help file name
  wxString wsHelpFile;
  wfnHelpFile.SetName( _T("wxMS-HB") );
  // also works if ZIP handler is ininitialized
  wfnHelpFile.SetExt( _T("htb") );

  wsHelpFile = m_wsCurHelpFile = wfnHelpFile.GetFullPath();
  if( ! ::wxFileExists( m_wsCurHelpFile ) )
  {
    wxLogDebug( _("Cannot find the Help file %s."), m_wsCurHelpFile);
    m_bFoundHelpFile  = false;
    m_wsCurHelpFile = wxEmptyString;
  }
  else
  {
    m_bFoundHelpFile  = true;
    m_msHtmlHelp.AddBook( m_wsCurHelpFile );
  }
#endif
#endif
#endif

#if defined( WANT_LOG_WIN )
  // set up Log Window
  // send text to it via something like:
  // wxLogStatus(_T("Caret is at (%d, %d)"), m_xCaret, m_yCaret);
  m_pLogWin = new wxLogTextCtrl( m_textCtrlLog );
  m_pLogOld = wxLog::SetActiveTarget( m_pLogWin );
#endif
  GetGaugeProgress()->SetValue( 0 );

#if defined( WANT_DBGRPRT )
#ifndef _DEBUG
  // remove the crash menu item if in release mode
  int iCrashId = m_menubarMain->FindMenu( _T("Crash") );
  wxASSERT( iCrashId != wxNOT_FOUND );

  wxString wsMenu;
  for ( size_t i = 0; i < m_menubarMain->GetMenuCount(); i ++ )
  {
    wsMenu = m_menubarMain->GetMenuLabelText( i );
    if ( wsMenu.IsSameAs( _T("Crash") ) )
      m_menubarMain->Remove( i );
  }
#endif
#endif
}

//-----------------------------------------------------------------------------
// Private methods
//-----------------------------------------------------------------------------
/**
 * We need to call
 * Destroy();
 * instead of
 * Close(true);  // so the main frame dtor will be called
 * when the application is closed from the File menu or with Alt-F4
 * This function is NOT called when the user closes the application
 * using the red 'x' on the system menu.
 * That action calls the main frame dtor directly.
 */
void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
  Destroy();
}

// ------------------------------------------------------------------
/**
 * If CanVeto() returns false, the system is shutting down.
 * and we MUST exit.
 */
void MyFrame::OnClose( wxCloseEvent& event )
{
  if ( event.CanVeto() )
  {
    if ( GetThread() &&      // DoStartALongTask() may have not been called
       GetThread()->IsRunning() )
    {
       GetThread()->Wait();
    }
  }
  Destroy();
}

//-----------------------------------------------------------------------------

MyFrame::~MyFrame()
{
  // save the frame position
  int x, y, w, h;

  GetSize(&w, &h);
  g_iniPrefs.data[IE_FRAME_W].dataCurrent.lVal = w;
  g_iniPrefs.data[IE_FRAME_H].dataCurrent.lVal = h;

  GetPosition(&x, &y);
  g_iniPrefs.data[IE_FRAME_X].dataCurrent.lVal = x;
  g_iniPrefs.data[IE_FRAME_Y].dataCurrent.lVal = y;

  g_iniPrefs.data[IE_SASH_POS].dataCurrent.lVal =
    m_splitterMain->GetSashPosition();

  wxGetApp().SaveAccounts();
  SaveColWidths();
  SaveColLabels();
  ClearMessages();
  curl_global_cleanup();
#if defined( _MSC_VER )
  if ( m_pAppProgress )
    delete m_pAppProgress;
#endif
}

//-----------------------------------------------------------------------------

void MyFrame::OnQuit( wxCommandEvent &WXUNUSED(event) )
{
  Close( true );
}

// ------------------------------------------------------------------
/**
 * Clear the message data array.
 */
void  MyFrame::ClearMessages()
{
  {
    // ensure noone modifies the message data while we
    // get ready to display it ???
    wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
    m_Pop3MsgList.clear();
  }
}

// ------------------------------------------------------------------
void MyFrame::OnClearList(wxCommandEvent& event)
{
  wxUnusedVar(event);
  ClearMessages();
  DisplayMailList();
}

// ------------------------------------------------------------------
#if defined( WANT_MSVC_INTERNET_TEST )
/* This arrangement displays the status once at start up &
 * not again until it changes.
 */
void MyFrame::OnIdle(wxIdleEvent& WXUNUSED(event))
{
#if defined( WANT_MSVC_INTERNET_TEST )
  static int s_isOnline = -1; // not true nor false

  bool isOnline = wxGetApp().GetDialer()->IsOnline();
  if ( s_isOnline != (int)isOnline )
  {
    s_isOnline = isOnline;
#if wxUSE_STATUSBAR
#if defined( _MSC_VER )
    SetStatusText(isOnline ? wxT("Online") : wxT("Offline"), 4 );
#endif    // WANT_MSVC_INTERNET_TEST
#endif    // wxUSE_STATUSBAR
  }
#endif    // WANT_MSVC_INTERNET_TEST
}
#endif    // WANT_MSVC_INTERNET_TEST

// ------------------------------------------------------------------

void MyFrame::ClearPop3CommandData()
{
  wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
  m_Pop3CommandData.sm_wsResultData =
    m_Pop3CommandData.sm_wsAccountName =
    m_Pop3CommandData.sm_wsPop3ServerUrl =
    m_Pop3CommandData.sm_wsUserName =
    m_Pop3CommandData.sm_wsUserPassword = wxEmptyString;
  m_Pop3CommandData.sm_bSuccess = false;
  m_Pop3CommandData.sm_lResultCode = -1l;
  m_Pop3CommandData.sm_wasCommands.Clear();
}


// ------------------------------------------------------------------
// Constructor
wxMsPasswordPromptDlg::wxMsPasswordPromptDlg( wxWindow* parent) :
  wxMsBasePasswordPromptDlg( parent )
{
  m_parent = parent;
};

// ------------------------------- eof ------------------------------
