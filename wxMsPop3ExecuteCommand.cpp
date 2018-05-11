/*-----------------------------------------------------------------
 * Name:        wxMsPop3ExecuteCommand.cpp
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
// this block needs to go AFTER all headers
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif
// ------------------------------------------------------------------
#if defined( WANT_SOCKET_CODE )
bool MyFrame::ExecutePop3CommandSocket( wxString wsCommand )
{
  wxString wsT;
  wxString wsT1;
  wxString wsT2;
  wxString wsResponse;
  wxString wsRecPrefix  = _T("< ");
  wxString wsSendPrefix = _T("> ");
  const char *buf1;
  unsigned char ucLen;
  buf1 = wsCommand.c_str();
  wsT = wsCommand.BeforeFirst( ' ' );
  if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    wxLogMessage(_("Sending %s to the server ..."), wsT );
  // don't count and send the terminating '\0'
  ucLen  = (unsigned char)(wxStrlen(buf1));
  m_pSock->Write(buf1, ucLen);
  if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
  {
    wsT2 = _T("Sending ") + wsT +_T(":");
    wsT2 += ( m_pSock->Error() ? _("failed !") : _("done") );
    wxLogMessage( wsT2 );
  }
  wsT = buf1;
  wsT1 = wsSendPrefix + wsT.BeforeFirst( '\r');
  wxLogMessage( wsT1 );
  if( ! GetNextLineFromServer( m_pSock, wsResponse,
    g_iniPrefs.data[IE_POP3_SERVER_TIMEOUT].dataCurrent.lVal ) )
  {
    wxString wsT3 = _("Server error for: ");
    wxLogMessage( wsT3 + wsT1 );
    return false;
  }
  if( ! OkResponse( wsResponse ) )
  {
    wxString wsT3 = _("Server error for: ");
    wxLogMessage( wsT3 + wsResponse );
    return false;
  }
  wsT = wsRecPrefix + wsResponse.BeforeFirst( '\r' );
  wxLogMessage( wsT );
  if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
  {
    wsT2 = _T("Received ") + wsT +_T(" reply: ");
    wsT2 += ( m_pSock->Error() ? _("failed !") : _("done") );
    wxLogMessage( wsT2 );
  }
  return m_pSock->Error();
}

// ------------------------------------------------------------------
/**
 * Retrieves the message header as well as however many extra lines
 * set by m_lNTopLines (could be made into an option).
 * The response will comprise the complete header, plus as many
 * lines as requested.
 * The header is separated from the content by a blank line, and
 * the complete response will be concluded by a line
 * with nothing but a ".\r\n" string
 */
bool MyFrame::ExecutePop3AllTopCommand( wxString wsCommand )
{
//  wxBusyCursor wait;
  long jj;
  const char *buf1;
  unsigned char ucLen;
  wxString wsT;
  wxString wsT1;
  wxString wsT2;
  wxString wsResponse;
  wxString wsRecPrefix  = _T("< ");
  wxString wsSendPrefix = _T("> ");

  if ( m_lNMessages )
  {
    wxString wsMsgHeader;
    bool  bEndOfHeader = false;
    for ( jj = 0; jj < m_lNMessages; jj++ )
    {
      for (std::list<MyPop3MsgElement>::iterator it = m_Pop3MsgList.begin();
            it != m_Pop3MsgList.end(); ++it )
      {
        if( it->m_lSequence == jj + 1 )
        {
          if( it->m_bNeedHeader )
          {
            wxString wsT;
            wsT.Printf( _T("TOP %ld %ld\r\n"), jj + 1, m_lNTopLines );
            if ( ! ExecutePop3OneTopCommand( wsT ) )
              it->m_bNeedHeader = true;
            else
              it->m_bNeedHeader = false;
            break;
          }
        }
      }
    }
  }
#if defined( WANT_PROGRESS_DIALOG )
  if ( m_pProgressDialog != NULL)
  {
    m_pProgressDialog->Destroy();
    m_pProgressDialog = NULL;
  }
#endif
  return true;
}

// ------------------------------------------------------------------
bool MyFrame::ExecutePop3OneTopCommand( wxString wsCommand )
{
//  wxBusyCursor wait;
  long jj;
  const char *buf1;
  unsigned char ucLen;
  wxString wsT;
  wxString wsT1;
  wxString wsT2;
  wxString wsResponse;
  wxString wsRecPrefix  = _T("< ");
  wxString wsSendPrefix = _T("> ");
  wxString wsMsgHeader;
  bool  bEndOfHeader = false;

  wsT = wsCommand.AfterFirst( ' ' );
  wsT = wsT.BeforeFirst( ' ' );
  wsT.ToLong( &jj );
  jj--;
  if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    wxLogMessage(_("Sending TOP to the server ..."));
  buf1 = wsCommand.c_str();
  // don't count and send the terminating '\0'
  ucLen  = (unsigned char)(wxStrlen(buf1));
  m_pSock->Write(buf1, ucLen);
  if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
  {
    wsT2 =  _T("Sending TOP: ") + ( m_pSock->Error() ? _("failed !") : _("done") );
    wxLogMessage( wsT2 );
  }
  wsT1 = wsSendPrefix + wsCommand.BeforeFirst( '\r');
  wxLogMessage( wsT1 );
  wsT1.Printf( _T("%s/%ld"), wsT1, m_lNMessages );
  SetStatusText( wsT1, 2);
  if( ! GetNextLineFromServer( m_pSock, wsResponse,
    g_iniPrefs.data[IE_POP3_SERVER_TIMEOUT].dataCurrent.lVal ) )
  {
    wxString wsT3 = _("Server error for: ");
    wxLogMessage( wsT3 + wsCommand );
    return false;
  }
  if( ! OkResponse( wsResponse ) )
  {
    wxString wsT3 = _("Server Response error: ");
    wxLogMessage( wsT3 + wsResponse );
    return false;
  }
  wsT = wsRecPrefix + wsResponse.BeforeFirst( '\r' );
  wxLogMessage( wsT );
  if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
  {
    wsT2 = _T("Received TOP reply: ") + ( m_pSock->Error() ? _("failed !") : _("done") );
    wxLogMessage( wsT2 );
  }
  long lLinesExpected = m_lNTopLines + 1;
  bEndOfHeader = false;
  wxString wsMessageTop;
  while( true )
  {
    GetNextLineFromServer( m_pSock, wsResponse,
      g_iniPrefs.data[IE_POP3_SERVER_TIMEOUT].dataCurrent.lVal );
    if (! wsResponse.IsSameAs(_T(".\r\n")) )
      wsMessageTop += wsResponse;

    // strip the newline for a better. more compact list
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      // need to convert all single '%' to "%%" for logging
      wxString wsFix = wsResponse;;
      size_t n = wsFix.Replace( "%", "%%" );
      wxLogMessage( wsFix.BeforeFirst( '\r' ) );
    }
    if ( wsResponse.IsSameAs(_T(".\r\n")) )
    {
#if defined( WANT_WX_EMAIL )
      // End of server reply - interpret/decode the message header
      wxEmailMessage* pMsgHeader =
        new wxEmailMessage( wsMessageTop, jj, false );

      {
        // ensure no one the message data while we write it
        wxCriticalSectionLocker lock(m_dataCS);
        for (std::list<MyPop3MsgElement>::iterator it = m_Pop3MsgList.begin();
            it != m_Pop3MsgList.end(); ++it )
        {
          if( it->m_lSequence == jj + 1 )
          {
            wxASSERT( it->m_pEmailMessage == NULL );
            it->m_pEmailMessage = pMsgHeader;
            it->m_bHaveMessage = bEndOfHeader;
            it->m_wsHeader = wsMsgHeader;
            it->m_bHaveMessage = false;
            break;
          }
        }
        m_lMsgIndex = jj;
      }
#endif
#if defined( WANT_PROGRESS_DIALOG )
      /* Check if we need to create progress dialog */
      if (m_pProgressDialog == NULL)
      {
        m_pProgressDialog = new wxProgressDialog(
          _T("wxMS : Retrieving messages"),
          _T("Please wait while the messages are retrieved from the server"),
          m_lNMessages );
      }
      /* Update the progress dialog status */
      m_pProgressDialog->Update(jj);
#endif
      wxQueueEvent(GetEventHandler(), new wxThreadEvent());
      break;
    }
    else
    {
      // Append content to line
      if ( wsMsgHeader != _T(""))
      {
        wsMsgHeader += _T("\r\n");
      }
      if (wsResponse.StartsWith(_T("..")))
      {
        wsMsgHeader += wsResponse.AfterFirst('.');
      }
      else
      {
        wsMsgHeader += wsResponse;
      }
      if( wsResponse.BeforeFirst( '\r' ).IsSameAs( wxEmptyString) )
      {
        bEndOfHeader = true;
      }
      if ( bEndOfHeader )
        lLinesExpected--;
    }
  }
  return true;
}

// ------------------------------------------------------------------
void MyFrame::GetPop3MessageContent( wxString a_wsAccount, long lIndex,
  wxString & wsContent )
{
  m_wsPop3Server = m_wsUser = m_wsPassword = wxEmptyString;
  MyAccountList::iterator iter;
  for ( iter = wxGetApp().m_AccountList.begin();
    iter != wxGetApp().m_AccountList.end(); ++iter )
  {
    MyMsAccountEl *pCurrent = *iter;
    if ( a_wsAccount.IsSameAs(pCurrent->m_wsAcctName ) )
    {
      m_wsPop3Server = pCurrent->m_wsPopServerUrl;
      m_wsUser = pCurrent->m_wsUserName;
      m_wsPassword = pCurrent->m_wsPassword;
      break;
    }
  }
  if ( m_wsPop3Server.IsEmpty() || m_wsUser.IsEmpty() )
  {
    wxLogMessage(_T("Empty Server URL or empty user name for account: %s"), a_wsAccount );
    wxBell();
    return;
  }
  m_wasCommands.Clear();
  wxString wsT;
  wsT.Printf( _T("RETR %ld\r\n"), lIndex );
  m_bRunning = false;
  m_wasCommands.Add( wsT );
  // Set busy info
//  wxBusyInfo pBusy_info( _T("Please wait while the content\nof the message is downloaded") );
  RunLibcurlThread();          // handle the mail check via threads
  while ( m_bRunning )
  {
//    ::wxYield();
    ::wxMilliSleep( 10 );
  }
  std::list<MyPop3MsgElement>::iterator it;
  {
    // ensure no one modifies the message data while we
    // get ready to display it ???
    wxCriticalSectionLocker lock(m_dataCS);
    for (it = m_Pop3MsgList.begin(); it != m_Pop3MsgList.end(); ++it )
    {
      if( it->m_lSequence == lIndex )
      {
        wsContent = it->m_wsMessage;
        break;
      }
    }
  }
}

// ------------------------------------------------------------------
bool MyFrame::ExecutePop3RetrCommand( wxString wsCommand )
{
//  wxBusyCursor wait;
  long jj;
  const char *buf1;
  unsigned char ucLen;
  wxString wsT;
  wxString wsT1;
  wxString wsT2;
  wxString wsResponse;
  wxString wsRecPrefix  = _T("< ");
  wxString wsSendPrefix = _T("> ");
  bool  bEndOfHeader = false;
  long lMsgSize = 0;
  long lIndex;

  wxString wsIndex = wsCommand.AfterFirst( ' ' );
  wsIndex.ToLong( &lIndex );
  if ( m_lNMessages )
  {
    wxString wsMessage;
    bEndOfHeader = false;
    {
      // ensure no one modifies the message data while we
      // get ready to display it ???
      wxCriticalSectionLocker lock(m_dataCS);
      for (std::list<MyPop3MsgElement>::iterator it = m_Pop3MsgList.begin();
            it != m_Pop3MsgList.end(); ++it )
      {
        if( it->m_lSequence == lIndex )
        {
          lMsgSize = it->m_lSize;
          break;
        }
      }
    }
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
      wxLogMessage(_("Sending RETR to the server ..."));
    wxString wsT = wsCommand.AfterFirst( ' ');
    wsT.ToLong( &jj );
    // adjust for array base of 0
    jj--;
    buf1 = wsCommand.c_str();
    // don't count and send the terminating '\0'
    ucLen  = (unsigned char)(wxStrlen(buf1));
    m_pSock->Write(buf1, ucLen);
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wsT2 =  _T("Sending RETR: ") + ( m_pSock->Error() ? _("failed !") : _("done") );
      wxLogMessage( wsT2 );
    }
    wsT1 = wsSendPrefix + wsCommand.BeforeFirst( '\r');
    wxLogMessage( wsT1 );
    wsT1.Printf( _T("%s/%ld"), wsT1, m_lNMessages );
    SetStatusText( wsT1, 2);
    if( ! GetNextLineFromServer( m_pSock, wsResponse,
      g_iniPrefs.data[IE_POP3_SERVER_TIMEOUT].dataCurrent.lVal ) )
    {
      wxString wsT3 = _("Server returned: ");
      wxLogMessage( wsT3 + wsT1 );
      return false;
    }
    if( ! OkResponse( wsResponse ) )
    {
      wxString wsT3 = _("Server Response error: ");
      wxLogMessage( wsT3 + wsResponse );
      return false;
    }
    wsT = wsRecPrefix + wsResponse.BeforeFirst( '\r' );
    wxLogMessage( wsT );
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wsT2 = _T("Received RETR reply: ") + ( m_pSock->Error() ? _("failed !") : _("done") );
      wxLogMessage( wsT2 );
    }
    bool bRet = GetMessage( wsMessage, lMsgSize );
#if defined( WANT_WX_EMAIL )
    // strip off the last line with nothing but '.\r\n'
    wsMessage = wsMessage.BeforeLast( '.' );
    /* End of message -> convert it
     * Get the corresponding message */
    wxEmailMessage* message =
      new wxEmailMessage( wsMessage, jj, true );
    {
      // ensure no one reads m_data while we refresh the message array
      wxCriticalSectionLocker lock(m_dataCS);
      for (std::list<MyPop3MsgElement>::iterator it = m_Pop3MsgList.begin();
            it != m_Pop3MsgList.end(); ++it )
        {
          if( it->m_lSequence == jj + 1 )
          {
            if ( it->m_pEmailMessage )
              delete it->m_pEmailMessage;
            it->m_pEmailMessage = message;
            it->m_bHaveMessage = bEndOfHeader;
            it->m_wsMessage = wsMessage;
            it->m_bHaveMessage = true;
            break;
          }
        }
      m_lMsgIndex = jj;
    }
#endif
  }
  SetStatusText( _T(""), 2);
  return true;
}
#endif
// ------------------------------- eof ------------------------------
