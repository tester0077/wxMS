/*-----------------------------------------------------------------
 * Name:        wxMsConnectTest.cpp
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets licence
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
#    include <stdlib.h>
#    include <crtdbg.h>
#  endif
#endif
#include "wxMsPreProcDefsh.h"   // MUST be first
// ------------------------------------------------------------------
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
#if defined( _MSC_VER )
// only good for MSVC - see note above re __VISUALC__
// this block needs to AFTER all headers
# include <stdlib.h>
# include <crtdbg.h>
# ifdef _DEBUG
#   ifndef DBG_NEW
#     define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#     define new DBG_NEW
#   endif
# endif
#endif

// ------------------------------------------------------------------
/**
 * Check if any mail servers are available.
 * return false, if noe are available, 
 * true otherwise
 */
bool MyFrame::CheckConnectivity()
{
  wxString wsT;
  // this dialog popping up is a nuisance - good during debugging, though
//  wxBusyInfo wait("Checking Server availability ...");
  wsT = _("Checking Server availability ...");
  SetStatusText( wsT, 4 );
  int iActiveAccounts = 0;
  int iActiveAvailable = 0;
  bool bAnyServersAvailable = false;
  MyAccountList::iterator iter;
  for ( iter = wxGetApp().m_AccountList.begin();
    iter != wxGetApp().m_AccountList.end(); ++iter )
  {
    MyMsAccountEl *pCurrent = *iter;
    if ( pCurrent->m_bEnabled )
    {
      iActiveAccounts++;
      ClearPop3CommandData();     // make sure we use the existing account setup
      m_Pop3CommandData.sm_wsPop3ServerUrl = pCurrent->m_wsPopServerUrl;
      m_Pop3CommandData.sm_wsAccountName = pCurrent->m_wsAcctName;
      m_Pop3CommandData.sm_wsUserName = pCurrent->m_wsUserName;
      if ( pCurrent->m_bSavePassword )
        {
          m_Pop3CommandData.sm_wsUserPassword = pCurrent->m_wsPassword;
        }
      else
      {
        // here we just skip the test; don't bother the user for now
        continue; // try next account
      }
      // try CAPA command
      // MUST be executed by same function which handles TOP & RETR
      // the data is returned the same way
      m_Pop3CommandData.sm_wasCommands.Add( _T("CAPA") );
      if ( MailServerConnectTest( m_Pop3CommandData ) )
      {
        bAnyServersAvailable = true;
        pCurrent->m_bServerAvailable = true;
        iActiveAvailable++;
      }
      else
        pCurrent->m_bServerAvailable = false;
    }
  }
  wsT.Printf( _("%d/%d avail"), iActiveAvailable, iActiveAccounts );
  SetStatusText( wsT, 4 );
  return ( iActiveAvailable > 0);
}

// ------------------------------------------------------------------
/**
 * Check a specific server's availability.
 */
bool MyFrame::MailServerConnectTest( Pop3CommandData &ar_Pop3CommandData )
{
  wxString wsT;
  bool bLocalRun = true;
  wxASSERT( m_Pop3CommandData.sm_wasCommands.GetCount() );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsAccountName.IsEmpty(), m_Pop3CommandData.sm_wsAccountName );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsPop3ServerUrl.IsEmpty(), m_Pop3CommandData.sm_wsPop3ServerUrl );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsUserName.IsEmpty(), m_Pop3CommandData.sm_wsUserName );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsUserPassword.IsEmpty(), m_Pop3CommandData.sm_wsUserPassword );
  RunLibcurlThread();         // handle the rest via threads
  // need to wait to let libcurl download the headers
  while ( bLocalRun )
  {
    {
      wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
      bLocalRun = m_bRunning;
    }
    ::wxYield();
    ::wxMilliSleep( 100 );
  }
  wsT = ar_Pop3CommandData.sm_wsResultData;
  if ( wsT.IsEmpty() )
  {
    return false;
  }
  //else
  return true;
}
// ------------------------------- eof ------------------------------
