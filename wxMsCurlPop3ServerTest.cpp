/*-----------------------------------------------------------------
 * Name:        wxMsCurlPop3ServerTest.cpp
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
#include "wxMsAccountsDialogh.h"
#include "wxMsAccountDetailsh.h"
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
void MyAccountsDetailsDialog::OnMailServerTest(wxCommandEvent& WXUNUSED(event) )
{
  wxString wsT;
  wxString wsResult;
  {
    wxGetApp().m_frame->ClearPop3CommandData();     // make sure we use the existing account setup
    // now get the full message data
    wxGetApp().m_frame->m_Pop3CommandData.sm_wsPop3ServerUrl = m_textCtrlPop3ServerUrl->GetValue();
    wxGetApp().m_frame->m_Pop3CommandData.sm_wsAccountName = m_textCtrlAccountName->GetValue();
    wxGetApp().m_frame->m_Pop3CommandData.sm_wsUserName = m_textCtrlUserName->GetValue();
    wxGetApp().m_frame->m_Pop3CommandData.sm_wsUserPassword = m_textCtrlPassword->GetValue();
    // try CAPA command
    // MUST be executed by same function which handles TOP & RETR
    // the data is returned the same way
    wxGetApp().m_frame->m_Pop3CommandData.sm_wsResultData.Clear();
    wxGetApp().m_frame->m_Pop3CommandData.sm_wasCommands.Add( _T("CAPA") );
  }
  wsT.Printf( _("Testing  %s ..."), m_textCtrlPop3ServerUrl->GetValue() );
  // display  a 'busy' notice
  {
    long lTime = 0l;
    m_wsTestRawCapaString = wxEmptyString;
    m_bTestHasCapa = m_bTestHasUidl = false;
    wxWindowDisabler disableAll;
    wxBusyInfo wait( wsT );
    wxGetApp().m_frame->RunLibcurlThread();         // handle the rest via threads
    // need to wait to let libcurl download the headers
    while ( wxGetApp().m_frame->m_bRunning )
    {
      ::wxYield();
      ::wxMilliSleep( 100 );
      lTime++;
    }
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( _T("CAPA: ") + wxGetApp().m_frame->m_Pop3CommandData.sm_wsResultData );
    }
    wsT = wxGetApp().m_frame->m_Pop3CommandData.sm_wsResultData;
    if ( wsT.StartsWith( _T("Error: ") ) )
    {
      m_staticTextTestResult->SetLabelText( wsT );
      return;
    }
    else if ( wsT.IsEmpty() )
    {
      wsResult = _("Failed!");
      if ( lTime > 20l )
        wsResult += _(" - Timeout");
      m_staticTextTestResult->SetLabelText( wsResult );
      return;
    }
    else 
    {
      wsResult = _(" Success");
      m_staticTextTestResult->SetLabelText( wsResult );
      m_wsTestRawCapaString = wxGetApp().m_frame->m_Pop3CommandData.sm_wsResultData;
      m_bTestHasCapa = m_wsTestRawCapaString.IsEmpty();
      m_bTestHasUidl = m_wsTestRawCapaString.MakeUpper().Find( _T("UIDL"));
    }
  }
}

// ------------------------------- eof ------------------------------
