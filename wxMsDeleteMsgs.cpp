/*-----------------------------------------------------------------
 * Name:        wxMsDeleteMsgs.cpp
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
 * Toggle the 'delete' check box
 */
void MyFrame::OnMarkToDeleteSelected(wxCommandEvent& event)
{
  // Get selected row
  wxString wsValue;
  wxArrayInt waiRows = m_gridMail->GetSelectedRows();
  if ( (waiRows.GetCount() > 1) || ( waiRows.GetCount() == 0 )  )
    return;
  unsigned long lIndex = waiRows[0];
  wsValue = m_gridMail->GetCellValue( lIndex, 0 );
  if( wsValue.IsSameAs( _T("1") ) )
  {
    m_gridMail->SetCellValue( lIndex, 0, _T("0") );
  }
  else
  {
    m_gridMail->SetCellValue( lIndex, 0, _T("1") );
  }
}

// ------------------------------------------------------------------
void MyFrame::OnMarkDeleteAll(wxCommandEvent& WXUNUSED(event) )
{
  m_gridMail->BeginBatch();
  int i = 0;

  for ( i = 0; i <  m_gridMail->GetNumberRows(); i++ )
  {
    m_gridMail->SetCellValue( i, 0, _T("1") );
  }
  m_gridMail->EndBatch();
}

// ------------------------------------------------------------------
void MyFrame::OnClearAllDelete( wxCommandEvent& event)
{
  m_gridMail->BeginBatch();
  int i = 0;

  for ( i = 0; i <  m_gridMail->GetNumberRows(); i++ )
  {
    m_gridMail->SetCellValue( i, 0, _T("0") );
  }
  m_gridMail->EndBatch();
}

// ------------------------------------------------------------------
/**
 * Delete all messages marked for deletion.
 * The best way to avoid issues because removing messages may change
 * the sequence numbers, is to start at the 'bottom; in the list and
 * work our way up.
 */
void MyFrame::DeleteMarkedMessages()
{
  int           iDeletes = 0; // total number of messages deleted
  size_t        iRow = 0;
  wxString      wsValue;
  wxString      wsT;
  wxString      wsAccount;        // curent account name
  wxArrayInt    waiRows2Delete;   // mail grid rows to be deleted when done
  wxArrayString wasUidls2Delete;
  wxArrayString wasAccounts;
  wxString      wsUidl;
  bool bFound = false;

  // Clear all of the command structure
  ClearPop3CommandData();
  MyAccountList::iterator iter;
  for ( iter = wxGetApp().m_AccountList.begin();
    iter != wxGetApp().m_AccountList.end(); ++iter )
  {
    // collect all accounts with messages marked for deletion
    MyMsAccountEl *pCurrent = *iter;
    if ( pCurrent->m_bEnabled )
    {
      wsAccount = wxEmptyString;
      // MUST start at bottom of grid, because, as messages removed,
      // the sequenc numbers within the grid would change
      for ( iRow = m_gridMail->GetNumberRows(); iRow > 0; iRow-- )
      {
        // get account name when we find an enabled account
        wsAccount = m_gridMail->GetCellValue( iRow - 1, 9 );
        if ( pCurrent->m_wsAcctName.IsSameAs( wsAccount ) )
        {
          wsValue = m_gridMail->GetCellValue( iRow - 1, 0 );
          if( wsValue.IsSameAs( _T("1") ) )
          {
            wasAccounts.Add( wsAccount );
            break;  // once we found one message for this account
            // we can move on to next account
          }
        }
      }
    }
  }
  if ( wasAccounts.size() == 0 )
    return; // nothing to do
  // otherwise handle all accounts with messages marked for deletion
  for ( size_t i = 0; i < wasAccounts.size(); i++)
  {
    for ( iter = wxGetApp().m_AccountList.begin();
      iter != wxGetApp().m_AccountList.end(); ++iter )
    {
      // collect all accounts with messages marked for deletion
      MyMsAccountEl *pCurrent = *iter;
      if ( pCurrent->m_bEnabled )
      {
        if ( wasAccounts[i].IsSameAs( pCurrent->m_wsAcctName ) )
        {
          for ( iRow = m_gridMail->GetNumberRows(); iRow > 0; iRow-- )
          {
            // get account name when we find an enabled account
            wsAccount = m_gridMail->GetCellValue( iRow - 1, 9 );
            if ( pCurrent->m_wsAcctName.IsSameAs( wsAccount ) )
            {
              // scan the message list
              wsValue = m_gridMail->GetCellValue( iRow - 1, 0 );
              if( wsValue.IsSameAs( _T("1") ) )
              {
                size_t j;
                std::list<MyPop3MsgElement>::iterator it;
                // find the corrsponding UIDL
                for ( j = 0, it = m_Pop3MsgList.begin(); it != m_Pop3MsgList.end(); ++it, j++ )
                {
#if defined( _DEBUG )
                  wxString wsTT = it->m_wsUidl;
#endif
                  if( j == iRow - 1 )
                  {
                    wasUidls2Delete.Add( it->m_wsUidl );
                    wsUidl = it->m_wsUidl;
                    waiRows2Delete.Add( iRow - 1 );
                    iDeletes++;
                    //continue; // search rest of the grid
                  }
                }
              }
            }
          }
          // now setup the DELE commands for the entries in wasUidls2Delete
          // we cannnot use FrameDeleteMessage() because we want to delete
          // all marked messages in one POP3 session
          MyAccountList::iterator iter;
          int j;
          for ( j = 0, iter = wxGetApp().m_AccountList.begin();
            iter != wxGetApp().m_AccountList.end(); ++iter, j++)
          {
            MyMsAccountEl *pCurrent = *iter;
            if ( pCurrent->m_wsAcctName.IsSameAs( wasAccounts[i] ) )
            {
              wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
              m_Pop3CommandData.sm_wsAccountName = pCurrent->m_wsAcctName;
              m_Pop3CommandData.sm_wsPop3ServerUrl = pCurrent->m_wsPopServerUrl;
              m_Pop3CommandData.sm_wsUserName = pCurrent->m_wsUserName;
              m_Pop3CommandData.sm_wsUserPassword = pCurrent->m_wsPassword;
              m_Pop3CommandData.sm_iAccountIndex = j;
              bFound = true;
              break;    // no need to check further
            }
            else
              continue; // try next account
          }
          if ( !bFound )  // can't proceed
          {
            wxMessageBox( _("Problems processing deletes for account:\n") + wsAccount,
              _("Error") );
            return;
          }
          for ( size_t i = 0; i < wasUidls2Delete.GetCount(); i++ )
          {
            wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
            // at this point, the UIDL string still carries the proper prefix
            wsT.Printf( _T("DELE %s"), wasUidls2Delete[i] );
            m_Pop3CommandData.sm_wasCommands.Add( wsT );
          }
          bool bLocalRun = true;
          {
            wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
            m_bRunning = true;
          }
          wxASSERT( m_Pop3CommandData.sm_wasCommands.GetCount() );
          wxASSERT_MSG( !m_Pop3CommandData.sm_wsAccountName.IsEmpty(), m_Pop3CommandData.sm_wsAccountName );
          wxASSERT_MSG( !m_Pop3CommandData.sm_wsPop3ServerUrl.IsEmpty(), m_Pop3CommandData.sm_wsPop3ServerUrl );
          wxASSERT_MSG( !m_Pop3CommandData.sm_wsUserName.IsEmpty(), m_Pop3CommandData.sm_wsUserName );
          wxASSERT_MSG( !m_Pop3CommandData.sm_wsUserPassword.IsEmpty(), m_Pop3CommandData.sm_wsUserPassword );
          RunLibcurlThread();
          // wait until the messages have been deleted 
          while ( bLocalRun )
          {
            {
              wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
              bLocalRun = m_bRunning;
            }
            {
#if defined( _MSC_VER )
              ::wxYield();
#endif
              ::wxMilliSleep( 10 );
#if defined( _DEBUG )
              wxLogMessage(_T("3 - %s - Delete - Line %ld"), __FILE__, __LINE__ );
#endif
            }
          }
        }
      }
    }
  }
  // independent of verbosity
  wxLogMessage( _("%d message(s) deleted"), iDeletes );
}

// ------------------------------------------------------------------
/**
 * Called from the mail grid context menu to delete a single message,
 * as well as when deleting all messages marked for deletion.
 */
void MyFrame::FrameDeleteMessage( wxString a_wsAccount, long a_lIndex, wxString a_wsUidl )
{
  // clear the command structure
  ClearPop3CommandData();
  long j = 0;
  bool bFound = false;

  MyAccountList::iterator iter;
  for ( j = 0, iter = wxGetApp().m_AccountList.begin();
    iter != wxGetApp().m_AccountList.end(); ++iter, j++ )
  {
    MyMsAccountEl *pCurrent = *iter;
    if ( pCurrent->m_wsAcctName.IsSameAs( a_wsAccount ) )
    {
      wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
      m_Pop3CommandData.sm_wsPop3ServerUrl = pCurrent->m_wsPopServerUrl;
      m_Pop3CommandData.sm_wsUserName = pCurrent->m_wsUserName;
      m_Pop3CommandData.sm_wsUserPassword = pCurrent->m_wsPassword;
      m_Pop3CommandData.sm_wsAccountName = a_wsAccount;
      bFound = true;
      break;
    }
  }
  if ( !bFound )  // can't proceed
    return;
  // delete the highlighted message
  // pass in command + UIDL; the called routine will
  // sort out the correspondence between message number & UIDL
  wxString wsT =  a_wsUidl;
  wsT = wsT.AfterFirst( ' ' ); // strip the current sequence number
  {
    wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
    m_Pop3CommandData.sm_wasCommands.Add(  _T("DELE ") + wsT );
  }
  wxASSERT( m_Pop3CommandData.sm_wasCommands.GetCount() );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsAccountName.IsEmpty(), m_Pop3CommandData.sm_wsAccountName );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsPop3ServerUrl.IsEmpty(), m_Pop3CommandData.sm_wsPop3ServerUrl );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsUserName.IsEmpty(), m_Pop3CommandData.sm_wsUserName );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsUserPassword.IsEmpty(), m_Pop3CommandData.sm_wsUserPassword );
  RunLibcurlThread();            // handle the mail delete via threads
  bool bLocalRun = true;
  // make sure we don't start a new check until
  // the current one is finished
  while ( bLocalRun )
  {
    {
      wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
      bLocalRun = m_bRunning;
    }
    ::wxMilliSleep( 10 );
    ::wxYield();  // wait for the job to finish
  }
  // need to remove the message from the mail list
  // before we refresh the display

  std::list<MyPop3MsgElement>::iterator it;
  for (j = 1, it = m_Pop3MsgList.begin();
              it != m_Pop3MsgList.end(); ++it, j++ )
  {
    if( j == a_lIndex )
    {
      m_Pop3MsgList.erase( it );
      if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
      {
        wxLogMessage(_T("Deleted Message: %ld - Line 183 GetCount:%d"),
          j, m_Pop3MsgList.size() );
      }
      break;
    }
  }
}

// ------------------------------------------------------------------
/**
 * Called when user selects a message and presses 'DEL' or wants to
 * delete a message via the main menu: Email -> Delete message.
 */
void MyFrame::OnEmailDelMessage( wxCommandEvent& WXUNUSED(event ) )
{
  wxString wsT;
  long j = 0;
  // Get selected row
  wxArrayInt waiRows = m_gridMail->GetSelectedRows();
  if ( (waiRows.GetCount() > 1) || ( waiRows.GetCount() == 0 )  )
    return;
  long lIndex = waiRows[0];
  wxString wsAccount = m_gridMail->GetCellValue( lIndex, 9 );
  wxString wsSubject = m_gridMail->GetCellValue( lIndex, 6 );
  if( wsAccount.IsEmpty() )
  {
    wsT.Printf( _("The account name in row %d is missing!"), lIndex );
    wxMessageBox( wsT, "Notice", wxOK );
    return;
  }
  // when POP3 server has UIDL command, we nned to get the UIDL related to
  // the chosen message and pass it to the libcurl handler so that
  // it can be all processed in one POP3 transaction.
  bool bFound = false;
  std::list<MyPop3MsgElement>::iterator it;
  // Display the message dialog
  for (j = 1, it = m_Pop3MsgList.begin(); it != m_Pop3MsgList.end(); ++it, j++ )
  {
    if( j == lIndex + 1)
    {
      if ( it->m_wsAcctName.IsSameAs( wsAccount ) )
      {
        FrameDeleteMessage( wsAccount, lIndex + 1, it->m_wsUidl );
        bFound = true;
        return;
      }
    }
  }
  if ( !bFound )
    wxLogMessage( _("Could not delete message in row %d"), lIndex + 1 );
}

// ------------------------------- eof ------------------------------
