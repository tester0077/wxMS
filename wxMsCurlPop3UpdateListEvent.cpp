/*-----------------------------------------------------------------
 * Name:        wxMsCurlPop3UpdateListEvent.cpp
 * Purpose:     Event called when a message list needs updating
 *              with new data from server
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
 * Called after a new message header has been received from the
 * POP3 server or when a row has been deleted.
 * The string passed in will allow us to decide whether we have
 * a new message to be added or a row to be deleted.
 * Use the UIDL to find the proper row to update.
 * Note depending on the start of the string, the passed in string
 * will look quite different:
 * For TOP, it will contain the account name the data corresponds to
 * for DELE it wil have the UIDL prefixed with the proper sequence number
 */
void MyFrame::OnUpdateDisplay( wxThreadEvent& event)
{
//  long        lLong = event.GetExtraLong(); // not used
  MyPop3MsgElement *pEl = event.GetPayload<MyPop3MsgElement *>();
  wxString    wsEventString = event.GetString();  // message account name
  int         iIndex = event.GetInt();  // 0 based index or -1 for cases where there is nothing to do
  if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
  {
    wxLogMessage( _T("===%s: int: %d, String %s"), __FILE__, iIndex,wsEventString );
  }
  if ( wsEventString.StartsWith( _T("TOP ")))  // add to display
  {
    wxASSERT( pEl != NULL);
    wxString wsAccount = wsEventString.AfterFirst( ' ' );
    size_t j = 0;
    {
      wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
      // pEl was allocated in the POP3 handler
      // we need to free the memory here
      m_Pop3MsgList.push_back( *pEl );
      size_t nRows = m_gridMail->GetNumberRows();
      delete pEl;   // the list takes ownership of the data memory
      for ( std::list<MyPop3MsgElement>::iterator it = m_Pop3MsgList.begin();
                  it != m_Pop3MsgList.end(); ++it, j++ )
      {
        if( it->m_wsAcctName.IsSameAs( wsAccount ) )
        {
          Add1Pop3Header2MsgList( nRows, wsAccount );
          AddOneMessageRow( nRows );
          return; // we're done
        }
      }
    }
  }
  else if ( wsEventString.StartsWith( _T("DELE\u00AE")))  //
  {
    long lAccount;
    wxString wsAcctIndex;
    wxString wsAccountName;
    wxASSERT( pEl == NULL);
    // expected string: DELE accountName UIDL
    // I here use the normally obscure Unicode character \u00AE = '®' "registered"
    // a string delimter to avoid placing restrictions on the account names
    if (g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4)
    {
      wxLogMessage(_T("===%s %ld: String %s"), __FILE__, __LINE__, wsEventString);
    }
    wsEventString = wsEventString.AfterFirst(  '\u00AE' );  // strips DELE
    wsAccountName = wsEventString.BeforeFirst( '\u00AE' );
    wsEventString = wsEventString.AfterFirst(  '\u00AE' );  // strip off account name
    wsEventString = wsEventString.AfterFirst(  ' ' );  // strips seq number - still a ' '
    
    size_t j = 0;
    if (g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4)
    {
      wxLogMessage(_T("OnUpdateDisplay 1  %ld:  GetCount: %d, %s"),
        __LINE__, m_Pop3MsgList.size(), wsAccountName);
    }
    std::list<MyPop3MsgElement>::iterator it;
    wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
    {
      // grid rows start @ 0
	    wxString wsAccountName1;
      for ( j = 0, it = m_Pop3MsgList.begin();
                  it != m_Pop3MsgList.end(); ++it, j++ )
      {
        wsAccountName1 = it->m_wsAcctName;
        if (g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4)
        {
          wxLogMessage(_T("OnUpdateDisplay 2  %ld: Deleted Message: %d GetCount:%d, %s - %s"),
            __LINE__, j, m_Pop3MsgList.size(), wsAccountName1, wsAccountName);
        }
        if ( it->m_wsAcctName.IsSameAs( wsAccountName ) )
        {
          
          wxString wsUidlMsgList = it->m_wsUidl.AfterFirst(' ');
          if (g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4)
          {
            wxLogMessage(_T("OnUpdateDisplay 3  %ld: Deleted Message: %d  GetCount:%d, %s - %s"),
              __LINE__, j, m_Pop3MsgList.size(), wsUidlMsgList, wsEventString);
          }
          if ( wsUidlMsgList.IsSameAs( wsEventString ) )
          {
            m_Pop3MsgList.erase( it );
            if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
            {
              wxLogMessage(_T("OnUpdateDisplay 4  %ld: Deleted Message: %d  GetCount:%d, %s - %s"),
                __LINE__, j, m_Pop3MsgList.size(), wsUidlMsgList, wsEventString );
            }
            m_gridMail->DeleteRows( j );
            break;
          }
        }
      }
    }
  }
}

// ------------------------------- eof ------------------------------
