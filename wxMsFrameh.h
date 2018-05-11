//-------------------------------------------------------------------
// Name:        wxMSFrameh.h
// Purpose:     main frame header for wxMS Mail Screener
// Author:      A. Wiegert
//
// Copyright:
// Licence:     wxWidgets license
//-------------------------------------------------------------------

#ifndef _WX_MS_FRAME_H_
#define _WX_MS_FRAME_H_

// ------------------------------------------------------------------
#include "wxMsPreProcDefsh.h" // needs to be first
// ------------------------------------------------------------------
#include <wx/thread.h>
#include <wx/log.h> 

#if defined( WANT_HELP )
#include "wx/cshelp.h"
#include "wx/html/helpctrl.h"
#endif

#include "wxMsPop3MsgListh.h"

#include "wxMsh.h"
#include "wxMsBaseFrame.h"

#include "mimetic.h"
#include "utils.h"
using namespace std;
using namespace mimetic;
#include "curl/curl.h"

#include <wx/appprogress.h>
#include "wxMsExtendedAddressh.h"
// ------------------------------------------------------------------
// mail grid right click menu and control IDs
enum
{
  wxID_CONTEXT_DELETE_MSG = wxID_HIGHEST,
  wxID_CONTEXT_PREVIEW_MSG,
  wxID_CONTEXT_MARK_DEL_MSG,
  wxID_CONTEXT_MARK_BOUNCE_MSG,
  wxID_CONTEXT_ADD_BLACK_MSG,
  wxID_CONTEXT_ADD_FRIEND_MSG,
  MAIL_CHECK_TIMER_ID,
  MAIL_SERVER_CHECK_TIMER_ID,
  MAIL_DISPLAY_TIMER_ID,          // displays 'Last mail check  .. minutes ago
  POP3_MSG_UPDATE_EVENT,          // this one gets sent from Pop3 Thread
                                  // every TOP command executed
};

// ------------------------------------------------------------------
/**
 * Structure containing all data for executing a POP3 server query.
 * Intended to be filled in by caller, passed on to routine in thread
 * main frame context for execution, which will parse it and return
 * any collected data and the result code in the appropiate members.
 */
struct Pop3CommandData
{
   wxString       sm_wsAccountName;
   wxString       sm_wsPop3ServerUrl;
   wxString       sm_wsUserName;
   wxString       sm_wsUserPassword;
   wxString       sm_wsResultData;
   wxArrayString  sm_wasCommands;   // command strings to execute in one call
   long           sm_lResultCode;
   int            sm_iAccountIndex; // index into account list corresp. to account name
   bool           sm_bSuccess;
   Pop3CommandData() : // default ctor for the struct
      sm_wsAccountName(wxEmptyString),
      sm_wsPop3ServerUrl(wxEmptyString),
      sm_wsUserName(wxEmptyString),
      sm_wsUserPassword(wxEmptyString),
      sm_wsResultData(wxEmptyString),
      sm_lResultCode( -1l ),
      sm_iAccountIndex( - 1 ),
      sm_bSuccess( false )
   {
   };
};

// ------------------------------------------------------------------

class MyFrame : public MyFrameBase, public wxThreadHelper
{
public:
  MyFrame( wxWindow* parent );
  virtual ~MyFrame();

  wxString m_frameProgName;

#if defined( WANT_HELP )
  wxHelpControllerHelpProvider* m_pProvider;
  void OnKeywordSearch(wxCommandEvent & event);
  void OnShowHelpContents(wxCommandEvent & event);
#if defined( WANT_ZIP_HELP )
  wxHtmlHelpController     m_msHtmlHelp;
  wxHtmlHelpController * GetHelpController()
  {
    return &m_msHtmlHelp;
  }
  void OnShowSection_FileMenu(wxCommandEvent & event);
#else
#if defined( _MSC_VER )
  wxCHMHelpController     m_msHtmlHelp;
#else
  wxHtmlHelpController     m_msHtmlHelp;
  wxHtmlHelpController& GetAdvancedHtmlHelpController()
    { return m_msHtmlHelp; }
#endif
  void OnMenuShowHelpContents( wxCommandEvent& event );
  void OnShowSection_FileMenu(wxCommandEvent & event);
  // can't overload the next function
//  void OnDialogContextHelp(wxCommandEvent & event);
  wxHelpController * GetHelpController()
  {
    return &m_msHtmlHelp;
  }
  wxString          m_wsCurHelpFile;
#endif
  bool              m_bFoundHelpFile;
#endif

  bool            m_bRunning;
  bool            m_bStop;      // set true by stop button on GUI
  bool            m_bCancelled;
#if defined( WANT_LOG_WIN )
  wxLog           *m_pLogOld;
  wxWindow        *m_pLogWindowSave; ///< ptr to log window when not shown
  wxLogTextCtrl   *m_pLogWin;
#endif
#if defined( WANT_LOG_CHAIN )
  void OnDeleteLogFiles( wxCommandEvent& event );
#endif
  struct MemoryStruct {
    char *memory;
    size_t size;
  };
  Pop3CommandData m_Pop3CommandData;
  wxString        m_wsCapabilities;
  wxString        m_wsStat;

  static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
  void ClearPop3CommandData();
  bool ThreadCurlPop3ServerTest( Pop3CommandData &arr_Pop3CommandData );
  void FrameCurlPop3GetNewMessages( Pop3CommandData &ar_Pop3CommandData );
  void ThreadCurlPop3GetNewMessages( Pop3CommandData &ar_Pop3CommandData );
  void ThreadParseUidlData( Pop3CommandData &ar_Pop3CommandData,
    wxString wsUidls,      wxArrayString &ar_wasUidls,
    wxString wsListResult, wxArrayString &ar_wasList, wxArrayLong &ar_walSizes,
    wxString wsStatResult);
  void ThreadParseUidls( wxString a_wsUidlResult, wxArrayString &ar_wasUidls );
  unsigned int GetSeqNoFromUidl( wxString wsUidl, wxArrayString &ar_wasUidls );
  void ThreadGetCurlPop3OneMessageBody( Pop3CommandData &ar_Pop3CommandData );
  void OnUpdateDisplay( wxThreadEvent& event);
  void ThreadCurlPop3DeleteMessage( Pop3CommandData &ar_Pop3CommandData );

  void OnOptionsSetupDlg(wxCommandEvent& WXUNUSED( event ));
  void OnCheck4NewMail(wxCommandEvent& event);
  void Check4NewMail();

  void OnLaunchMailClient(wxCommandEvent& event);
  void OnProcessMail(wxCommandEvent& event);
  bool InvokeMailClient( wxString wsDestPath );
  void OnMailCheckStop(wxCommandEvent& event);
  void ExecutePop3Command( Pop3CommandData &m_Pop3CommandData );
  bool MailServerConnectTest( Pop3CommandData &ar_Pop3CommandData );
  bool CheckConnectivity();

  void printMimeStructure( MimeEntity* pMe, int tabcount = 0, wxTextCtrl *pTextCtrl = NULL );

  void MIMEExtractAll(mimetic::MimeEntity& entity, bool& found_plain_text_body, bool& found_html_body);
  void MIMEExtractBody(mimetic::MimeEntity& entity, wxString& ar_wsOut, bool shall_recode);

  wxString PrintTabs(int c);

  std::list<ExtendedAddress> m_toArray;
  std::list<ExtendedAddress> m_ccArray;
  std::list<ExtendedAddress> m_bccArray;
  bool m_found_plain_text_body;
  bool m_found_html_body;
  bool m_has_html_alternative;
  wxString m_wsTextBody;
  wxString m_wsHtmlBody;
  int      m_iNAttachments;

  // threadhelper stuff
  void RunLibcurlThread();

  void OnStopTask( wxCommandEvent& event );
  void ClearMailGrid();
  void DisplayMailList();
  void AddOneMessageRow( size_t szIndex );
  void Add1Pop3Header2MsgList( size_t szIndex, wxString wsAccount );
  void GetMessageFilterStatus( long lIndex, std::list<MyPop3MsgElement>::iterator it,
    bool& bDelete, wxString& wsStatus, wxString& wsColor );
  size_t FormatSize( double dNumber, wxString &wsOutput );
  double GetDepth( double dNumber, int& iDepth);

  // threadhelper stuff
  wxThread::ExitCode Entry();

  std::list<MyPop3MsgElement> m_Pop3MsgList;     ///< std::list holding all message data
  long                  m_lNTopLines;       ///< number of lines to request in TOP command
  void                  ClearMessages();    ///< clear the message data array

  // mail grid
  void OnMailGridChar(wxKeyEvent& event);
  void InitializeMailGrid();
  void ResizeMailGrid();
  void OnMailGridSize( wxSizeEvent& event);
  void RestoreColWidths();
  void SaveColWidths();
  void RestoreColLabels();
  void SaveColLabels();
  wxArrayLong     m_walColWidths;
  wxArrayString   m_wasColLabels;
  void DeleteMarkedMessages();
  void FrameDeleteMessage( wxString wsAccount, long lIndex, wxString wsUidl );
  void BounceMarkedMessages();
  // mail grid clicking
  void OnMailGridCellLeftClick( wxGridEvent& event);
  void OnMailGridCellChanging( wxGridEvent& event);
  void OnMailGridCellChanged( wxGridEvent& event);
  void OnMailGridEditorShown( wxGridEvent& event);

  // main frame handlers
  void CheckboxHandler( wxCommandEvent& event);
  void OnEmailPreview( wxCommandEvent& event);
  void PreviewMessage( wxString wsAccount, long lIndex );
  void OnEmailDelMessage( wxCommandEvent& event );
  void OnContextPreview( wxCommandEvent& event );
  void OnMarkToBounceSelected( wxCommandEvent& event);
  void OnMarkToBounceAll( wxCommandEvent& event);
  void OnMarkToDeleteSelected( wxCommandEvent& event);
  void OnMarkToDeleteAll( wxCommandEvent& event);
  void OnClearAllDelete( wxCommandEvent& event);
  void OnClearAllBounce( wxCommandEvent& event);

  void OnClearList(wxCommandEvent& event);

  void OnMenuFilters(wxCommandEvent& event);
  long ShowOptions( long lPage );

  void OnMailGridRightClick( wxGridEvent& event);
  int     m_iRightClickRow;
  void ShowContextMenu( const wxPoint& pos );
  void OnContextDelMsg( wxCommandEvent& event );
  bool GetAccountInfo( wxString &wsAccountList );

  void OnContextMarkDelMsg( wxCommandEvent& event );
  void OnContextMarkBounceMsg( wxCommandEvent& event );
  void OnContextAddBlackMsg( wxCommandEvent& event );
  void OnContextAddFriendMsg( wxCommandEvent& event );

  // OnUpdate handlers
  void OnUpdateUiMailCheckStart( wxUpdateUIEvent& event);
  void OnUpdateUiMailCheckStop( wxUpdateUIEvent& event);
  void OnUpdateUiProcessMail( wxUpdateUIEvent& event);
  void OnUpdateUiLaunchMailApp( wxUpdateUIEvent& event);
  void OnUpdateUiFriendsListAddr( wxUpdateUIEvent& event);
  void OnUpdateUiFriendsListDom( wxUpdateUIEvent& event);
  void OnUpdateUiBlackListAddr( wxUpdateUIEvent& event);
  void OnUpdateUiBlackListDom( wxUpdateUIEvent& event);
  void OnUpdateUiMarkDelete( wxUpdateUIEvent& event);
  void OnUpdateUiMarkBounce( wxUpdateUIEvent& event);
  void OnUpdateUiPreview( wxUpdateUIEvent& event);
  void OnUpdateUiMarkDeleteAll( wxUpdateUIEvent& event);
  void OnUpdateUiMarkBounceAll( wxUpdateUIEvent& event);
  void OnUpdateUiClearList( wxUpdateUIEvent& event);
  void OnUpdateUiLogShow( wxUpdateUIEvent& event);
  void OnUpdateUiLogClear( wxUpdateUIEvent& event);
  void OnUpdateUiCheckNew( wxUpdateUIEvent& event);
  void OnUpdateUiToolsCheckMsgs( wxUpdateUIEvent& event);
  void OnUpdateUiToolsAccounts( wxUpdateUIEvent& event);
  void OnUpdateUiToolsBlackFriends( wxUpdateUIEvent& event);
  void OnUpdateUiToolsErrors( wxUpdateUIEvent& event);
  void OnUpdateUIHelpMenu( wxUpdateUIEvent& event);
  void OnUpdateUIHelpToolbar( wxUpdateUIEvent& event);
  void OnCheck4Update( wxCommandEvent& event);
  void OnUpdateUiClearAllBounce(wxUpdateUIEvent& event);
  void OnUpdateUiClearAllDelete(wxUpdateUIEvent& event);
  void OnUpdateUiToolsBlackFriendsList(wxUpdateUIEvent& event);
  void OnUpdateUiToolsStop(wxUpdateUIEvent& event);
  void OnUpdateUiEmailDelete(wxUpdateUIEvent& event);
  bool Check4Update();
  void OnLists(wxCommandEvent& event);    // friends & black list handler

  // mailcheck timer
  void OnMailCheckTimer( wxTimerEvent& event );
  // Clear the status field 0 after a suitable time
  void OnMailDisplayTimer( wxTimerEvent& event ); 
  void OnMailConnectTimer( wxTimerEvent& event );

  int                     m_iWheelRotation;
#if defined( WANT_MSVC_INTERNET_TEST )
  void OnIdle(wxIdleEvent& event);
#endif
#if defined( _MSC_VER )
  // Note: this is intended to display progress in main task bar icon for this application
  bool                    m_bHaveAppProgress;
  wxAppProgressIndicator *m_pAppProgress;
  wxAppProgressIndicator* GetAppProgressInd() { return m_pAppProgress; }
#endif
  wxTimer *GetMailTimerPtr()
  {
    return &m_tMailCheckTimer;
  }
#if defined( WANT_DBGRPRT )
  void OnCauseException( wxCommandEvent& event );
#endif

private:  //////////////////////////////////////////////////////
  // Event handlers - these functions should _not_ be virtual)
  void OnExit( wxCommandEvent& event);
  void OnClose(wxCloseEvent& event );
  void OnQuit( wxCommandEvent& event );
  void OnAbout( wxCommandEvent &event );
  void OnOptionsLogClear( wxCommandEvent& event );
  void OnOptionsLogShow( wxCommandEvent& event );
  void OnMenuAccounts( wxCommandEvent& event );
  void OnMailGridColSize( wxGridSizeEvent& ev );
  void OnMailGridMouseWheel(wxMouseEvent& event);

  // the data for the Entry() routine,
  // intended to protect the message list m_Pop3MsgList
  // and m_bRunning
  wxCriticalSection     m_CS_Pop3MsgList;

  // mail check timer
  wxTimer m_tMailCheckTimer;
  int     m_iMailTimerTicks;
  wxTimer m_tMailDisplayTimer;
  wxTimer m_tMailServerConnectCheckTimer;
  int     m_iConnectCheckTimerTicks;

  // Any class wishing to process wxWidgets events must use this macro
  DECLARE_EVENT_TABLE()
};

// ------------------------------------------------------------------
//class wxMsBasePasswordPromptDlg;
class wxMsPasswordPromptDlg : public wxMsBasePasswordPromptDlg
{
public:
  wxMsPasswordPromptDlg( wxWindow* parent);// { m_parent = parent; };
  wxWindow* m_parent;
};
#endif    // _WX_MS_FRAME_H_
// ------------------------------- eof ------------------------------
