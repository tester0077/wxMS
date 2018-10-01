/*-----------------------------------------------------------------
 * Name:        wxMsCheckPopMail.cpp
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets license
 *---------------------------------------------------------------- */

/*----------------------------------------------------------------
 * Standard wxWidgets headers
 *---------------------------------------------------------------- */
// Note __VISUALC__ is defined by wxWidgets, not by MSVC IDE
// and thus won't be defined until some wxWidgets headers are included
#if defined( _MSC_VER )
#  if defined ( _DEBUG )
     // this statement NEEDS to go BEFORE all headers
#    define _CRTDBG_MAP_ALLOC
#  endif
#endif
#include "wxMsPreProcDefsh.h"   // needs to be first
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

/* For all others, include the necessary headers
 * (this file is usually all you need because it
 * includes almost all "standard" wxWidgets headers) */
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
// ------------------------------------------------------------------
#include "wxMsFrameh.h"
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
/**
 * We check all enabled accounts.
 * Simply add all message to the list, which includes a column for the account name.
 * But we will need a list of account details
 */
void MyFrame::OnCheck4NewMail(wxCommandEvent& WXUNUSED(event)  )
{
  Check4NewMail();
}

// ------------------------------------------------------------------
/**
 * Split out so I can also call it from the 'check for mail' timer.
 */
void MyFrame::Check4NewMail()
{
  wxString wsT;
#if defined( _MSC_VER )
  if( !wxGetApp().IsInternetAvailable() )
  {
    wsT.Printf( _("Internet Connection is unavailable!\n") );
    wxMessageBox( wsT, "Error", wxOK );
    wxLogMessage( wsT );
    return;
  }
#endif
#if defined( WANT_SEMAPHORE )
  // make sure we're not in the middle of a current call
  wxGetApp().m_semAllDone.Wait();
#endif
  // clear the display - this seems to be the only way to have
  // this app behave on the slower machine Black
  ClearMessages();
  DisplayMailList();

  // then proceed & check for mail
  bool bAnyAccountEnabled = false;
  MyAccountList::iterator iter;
  m_sz2Delete = 0;
  SetStatusText( wxEmptyString, 1 ); // clear 'X Delete / Y Total'

  for ( iter = wxGetApp().m_AccountList.begin();
    iter != wxGetApp().m_AccountList.end(); ++iter )
  {
    MyMsAccountEl *pCurrent = *iter;
    // cant use this test for accounts which do not save the password
    if ( pCurrent->m_bEnabled /*&& pCurrent->m_bServerAvailable*/ )
    {
      // MUST check if the account is available
      // if not, run one more test, if it succeeds, check mail
      // else advise user
      if ( ! pCurrent->m_bServerAvailable )
      {
        if ( ! CheckConnectivity() )
        {
          // see FS#2158 prevent multiple instances of this pop-up
          // being shown over time & allow the deletion of the current one
          // when the server becomes available again
          wsT.Printf( _("POP3 server: \"%s\" for account: \"%s\" is off-line"), 
            pCurrent->m_wsPopServerUrl, pCurrent->m_wsAcctName );
          wxWindow *pWin0 = FindWindowByLabel(  _("Notice"), this );
          wxWindow *pWin1 = FindWindowByLabel(  _("Notice") );
          wxWindow *pWin2 = FindWindowByName(  _("Notice"), this );
          SetStatusText( wsT, 0 );
          wxMessageBox( wsT, _("Notice"), wxOK );
          continue;
        }
      }
      // get any new messages from this account's POP3 server
      m_Pop3CommandData.sm_wsPop3ServerUrl = pCurrent->m_wsPopServerUrl;
      m_Pop3CommandData.sm_wsAccountName = pCurrent->m_wsAcctName;
      m_Pop3CommandData.sm_wsUserName = pCurrent->m_wsUserName;
      if ( pCurrent->m_bSavePassword )
      {
        m_Pop3CommandData.sm_wsUserPassword = pCurrent->m_wsPassword;
      }
      else
      {
        // we need to prompt the user for the password
        wxMsPasswordPromptDlg dlg( this );
        dlg.GetStaticTextPwServer()->SetLabelText( pCurrent->m_wsPopServerUrl );
        dlg.GetStaticTextPwUserName()->SetLabelText( pCurrent->m_wsUserName );
        if ( dlg.ShowModal() == wxID_OK )
        {
          m_Pop3CommandData.sm_wsUserPassword = dlg.GetTextCtrlPassword()->GetLineText( 0 );
        }
        else
          continue; // try next account
      }
      // some feedback for user when connections are slow
      // status text will be overwritten when data is returned
      wsT.Printf( _("Waiting on %s"), pCurrent->m_wsPopServerUrl );
      SetStatusText( wsT, 0 );
      FrameCurlPop3GetNewMessages( m_Pop3CommandData );
      // will return when data is available
    }
  }
  SetStatusText( wxEmptyString, 0 );
  if( ! m_bCancelled )
  {
    wsT.Printf( _(" %ld Delete/%ld Total"), m_sz2Delete, m_Pop3MsgList.size() );
  }
  else
  {
    wsT = _("Mail check cancelled!");
  }
  SetStatusText( wsT, 1 );
  m_gridMail->SelectRow( 0 );
  // reset the mail check timer
  m_iMailTimerTicks = 0;
  m_iConnectCheckTimerTicks = 0;
  if ( g_iniPrefs.data[IE_SCHEDULE_MAIL_CHECK].dataCurrent.bVal )
  {
    m_tMailCheckTimer.Start(1000 * 60 );
  }
  m_tMailServerConnectCheckTimer.Start(1000 * 60 );
}

// ------------------------------------------------------------------
/**
 * This fumnction will also return false if there is missing information
 * for any account - may need refining for multiple valid/enabled accounts  //todo
 */
bool MyFrame::GetAccountInfo()
{
  wxString wsT;
  int iActiveAccounts = 0;
  bool bAnyAccountEnabled = false;
  MyAccountList::iterator iter;
  for ( iter = wxGetApp().m_AccountList.begin();
    iter != wxGetApp().m_AccountList.end(); ++iter )
  {
    MyMsAccountEl *pCurrent = *iter;
    if ( pCurrent->m_bEnabled )
    {
      // need to verify we have all the information we need
      // just in case the user edited the data outside the app
      if ( !pCurrent->m_wsAcctKind.IsSameAs( _T("POP3")) )
      {
        wxMessageBox( _("We can only handle POP3 mail servers at present!"),
          _("Notice"), wxOK );
        return false;
      }
      if ( pCurrent->m_wsAcctName.IsEmpty() )
      {
        wxMessageBox( _("Account Name cannot be empty!"),
          _("Notice"), wxOK );
        return false;
      }
      if ( pCurrent->m_wsPopServerUrl.IsEmpty() )
      {
        wsT.Printf( _("POP3 server URL for %s cannot be empty!"), pCurrent->m_wsAcctName );
        wxMessageBox( wsT, _("Notice"), wxOK );
        return false;
      }
      if ( pCurrent->m_wsPassword.IsEmpty() )
      {
        wsT.Printf( _("POP3 user password for %s cannot be empty!"), pCurrent->m_wsAcctName );
        wxMessageBox( wsT, _("Notice"), wxOK );
        return false;
      }
      if ( pCurrent->m_wsUserName.IsEmpty() )
      {
        wsT.Printf( _("POP3 user name for %s cannot be empty!"), pCurrent->m_wsAcctName );
        wxMessageBox( wsT, _("Notice"), wxOK );
        return false;
      }
      iActiveAccounts++;
      bAnyAccountEnabled = true;
    }
  }
  // Modify the tool bar tip to show the status
  wxToolBarToolBase* pTool = m_toolBarMain->FindById( wxID_CHECK_MAIL );
  if( !bAnyAccountEnabled )
  {
    pTool->SetLongHelp( _("No accounts enabled!") );
    pTool->SetShortHelp( _("No accounts enabled!") );
  }
  else
  {
    pTool->SetLongHelp( _("Check Selected Mail Account(s) (F5)") );
    pTool->SetShortHelp( _("Check Mail") );
  }
  return bAnyAccountEnabled;
}

// ------------------------------- eof ------------------------------
