/*-----------------------------------------------------------------
 * Name:        wxMsThreadCurlPop3GetMessages.cpp
 * Purpose:     Get new messages for a specified account.
 *              Runs in thread context
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
#include "curl/curl.h"
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

struct MemoryStruct {
  char *memory;
  size_t size;
};

// ------------------------------------------------------------------
size_t MyFrame::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}
// ------------------------------------------------------------------
/**
 * Get all new message data for the specified account, where 'new' is defined
 * as a message not whose UIDL is not found in the current UIDL string array
 * for the account
 */
void MyFrame::ThreadCurlPop3GetNewMessages( Pop3CommandData &ar_Pop3CommandData )
{
  CURL *pCurl;
  CURLcode res = CURLE_OK;
  struct MemoryStruct chunk;
  chunk.memory = (char*) calloc(1, sizeof( char ) );
  chunk.size = 0;

  wxString wsStatResult;    // result of STAT command - number of msg & total size
  wxString wsListResult;    // result of LIST command
  wxString wsUidlResult;    // result of UIDL command

  wxString wsPop3Server = _T("pop3://");
  wsPop3Server += ar_Pop3CommandData.sm_wsPop3ServerUrl.mb_str(wxConvUTF8).data();
#if defined( _DEBUG )
  wxString wsUser = ar_Pop3CommandData.sm_wsUserName;
  wxString wsPassword = ar_Pop3CommandData.sm_wsUserPassword;
  wxString wsServer = wsPop3Server;
  wxString wsCommand =  ar_Pop3CommandData.sm_wasCommands[0];
#endif

  pCurl = curl_easy_init();
  if(pCurl)
  {
    curl_easy_setopt( pCurl,CURLOPT_USERNAME, ar_Pop3CommandData.sm_wsUserName.mb_str(wxConvUTF8).data());
    curl_easy_setopt( pCurl,CURLOPT_PASSWORD, ar_Pop3CommandData.sm_wsUserPassword.mb_str(wxConvUTF8).data() );
    curl_easy_setopt( pCurl, CURLOPT_URL, wsPop3Server.mb_str(wxConvUTF8).data());
    // allow some control over connection timeout - set via INI for now
    curl_easy_setopt( pCurl, CURLOPT_CONNECTTIMEOUT, g_iniPrefs.data[IE_POP3_SERVER_TIMEOUT].dataCurrent.lVal );
    //curl_easy_setopt( pCurl, CURLOPT_VERBOSE, 1);

    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( _T("ThreadCurlPop3GetNewMessages") );
    }
    wxString wsCommand = _T("STAT");    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< STAT
    curl_easy_setopt( pCurl, CURLOPT_HEADERFUNCTION, WriteMemoryCallback);
    /* pass in custom data to the callback */
    curl_easy_setopt( pCurl, CURLOPT_HEADERDATA, &chunk );
    curl_easy_setopt( pCurl, CURLOPT_CUSTOMREQUEST, wsCommand.mb_str(wxConvUTF8).data() );
    // avoid hanging
    curl_easy_setopt( pCurl, CURLOPT_NOBODY, 1L);
    /* Perform the custom request */
    wxLogDebug( _T("GetMessage start for %s, timeout: %ld secs"), wsPop3Server, 
      g_iniPrefs.data[IE_POP3_SERVER_TIMEOUT].dataCurrent.lVal );
    res = curl_easy_perform( pCurl );

    /* Check for errors */
    if (CURLE_OPERATION_TIMEDOUT == res)
    {
      wxString wsT;
      wsT.Printf(  _T("== Timeout for %s"), wsPop3Server );
      wxLogMessage( wsT );
      // clean up memory
      goto cleanup;
    }
    else if ( res != CURLE_OK )
    {
      wxString wsT;
      wsT.Printf( _T("=== GetMsg: STAT: curl_easy_perform() failed: %s"), curl_easy_strerror(res));
      wxLogMessage( wsT );
      // clean up memory
      goto cleanup;
    }
    wsStatResult = chunk.memory;
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( _T("STAT: %lu bytes retrieved: %s"), (long)chunk.size, wsStatResult );
    }
    if(chunk.memory)    // clean up for next command
    {
      free(chunk.memory);
      chunk.memory = (char*) calloc(1, sizeof( char ) );
      chunk.size = 0;
    }
    //  else issue a LIST command   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< LIST
    curl_easy_setopt( pCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt( pCurl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt( pCurl, CURLOPT_NOBODY, 0);
    wsCommand = _T("LIST");
    curl_easy_setopt( pCurl, CURLOPT_CUSTOMREQUEST, wsCommand.mb_str(wxConvUTF8).data() );
    /* Perform the custom request */
    res = curl_easy_perform( pCurl );
    /* Check for errors */
    if(res != CURLE_OK)
    {
      wxString wsT;
      wsT.Printf( _T("GetMsg: LIST: curl_easy_perform() failed: %s"), curl_easy_strerror(res));
      wxLogMessage( wsT );
      wxLogDebug( wsT );
      // clean up memory
      goto cleanup;
    }
    // process the LIST data -- message sizes
    wsListResult = chunk.memory;
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( _T("LIST:  %s"), wsListResult );
    }
    if(chunk.memory)    // clean up for next command
    {
      free(chunk.memory);
      chunk.memory = (char*) calloc(1, sizeof( char ) );
      chunk.size = 0;
    }
    //  else issue a UIDL command     <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    wsCommand = _T("UIDL");
    curl_easy_setopt( pCurl, CURLOPT_CUSTOMREQUEST, wsCommand.mb_str(wxConvUTF8).data() );
    /* Perform the custom request */
    res = curl_easy_perform( pCurl );
    /* Check for errors */
    if(res != CURLE_OK)
    {
      wxString wsT;
      wsT.Printf( _T("GetMsg: UIDL: curl_easy_perform() failed: %s"), curl_easy_strerror(res));
      wxLogMessage( wsT );
      wxLogDebug( wsT );
      // clean up memory
      goto cleanup;
    }
    // process the UIDL data
    wsUidlResult = chunk.memory;
    if(chunk.memory)    // clean up for next command
    {
      free(chunk.memory);
      chunk.memory = (char*) calloc(1, sizeof( char ) );
      chunk.size = 0;
    }
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( _T("UIDLs:  %s\n"), wsUidlResult );
    }
    // does user want us to quit?
    if ( GetThread()->TestDestroy() )
    {
      goto cleanup;
    }
    wxArrayString wasUidlLines;
    wxArrayString wasListLines;
    wxArrayLong   walSizes;
    // this simply cleans out any existing UIDL from the wsUidlResult
    // and wsListResult and leaves any new messages and their UIDL
    // in the arrays so that we can now download their headers
    // Note the arrays wasUidls, wasListLines & walSizes will have
    // the parsed data, minus any lines which do not conform to the
    // specified line format -> see line #365
    ThreadParseUidlData( ar_Pop3CommandData, 
      wsUidlResult,   // current list of UIDLs on server
      wasUidlLines,
      wsListResult,   // current list of sizes
      wasListLines, 
      walSizes, 
      wsStatResult );
    if ( wasUidlLines.GetCount() == 0)
    {
      goto cleanup;
      return; // no new messages, nothing to do
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TOP commands
    // else, while we are still connected to the server, we now
    // need to issue TOP commands to retrieve the headers for
    // any messages still left to be processed
    wxString wsUidl;
    unsigned long ulSize;
    wxString wsHeader;
    std::size_t n = 1;
    std::size_t nNewMessages = wasUidlLines.GetCount();
    while ( wasUidlLines.GetCount() )
    {
      // does user want us to quit?
      if ( GetThread()->TestDestroy() )
      {
        goto cleanup;
      }
      wxString wsT1;
      wxString wsT3 = ar_Pop3CommandData.sm_wsAccountName;
      wsT1.Printf( _("%s: header %ld of %ld"), wsT3, n, nNewMessages );
#if defined( WANT_NEW_PROGRESS )
      // update the progress gauge for user feedback
      GetGaugeProgress()->SetValue( (n * 100)/(nNewMessages) );
#if defined( _MSC_VER )
      // only good for MSW
      // and the taskbar icon's as well
      wxGetApp().m_frame->GetAppProgressInd()->SetValue( (n * 100)/(nNewMessages) );
#endif
#endif
      SetStatusText( wsT1, 0);
      wsUidl = wasUidlLines[0];
      ulSize = walSizes[0];
      unsigned int uiSeq = GetSeqNoFromUidl( wsUidl,  wasUidlLines );
      wxASSERT( uiSeq );  // MUST be positive, > 0
      wsCommand.Printf( _T("TOP %d %ld"),  uiSeq, m_lNTopLines );
      if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
      {
        wxLogMessage( wsCommand );
      }
      curl_easy_setopt( pCurl, CURLOPT_CUSTOMREQUEST,
        wsCommand.mb_str(wxConvUTF8).data() );
      /* Perform the custom request */
      res = curl_easy_perform( pCurl );
      /* Check for errors */
      if(res != CURLE_OK)
      {
        wxString wsT;
        wsT.Printf( _T("GetMsg: %s: curl_easy_perform() failed: %s"),
          wsCommand, curl_easy_strerror(res));
        wxLogMessage( wsT );
        wxLogDebug( wsT );
        // clean up memory
        goto cleanup;
      }
      // does user want us to quit?
      if ( GetThread()->TestDestroy() )
      {
        goto cleanup;
      }
      // else
      // process the header data
      wsHeader = chunk.memory;
      if(chunk.memory)    // clean up for next command
      {
        free(chunk.memory);
        chunk.memory = (char*) calloc(1, sizeof( char ) );
        chunk.size = 0;
      }
      if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
      {
        wxLogMessage( _T("Header: %d\n%s"), uiSeq, wsHeader );
      }
      MyPop3MsgElement *pEl =  new MyPop3MsgElement( wsUidl, ulSize );
      pEl->m_wsUidl = wsHeader;
      pEl->m_bNeedHeader = false;
      pEl->m_wsHeader = wsHeader;
      pEl->m_wsUidl = wsUidl;
      pEl->m_wsAcctName = ar_Pop3CommandData.sm_wsAccountName;
      pEl->m_wsTo = pEl->m_wsAttachments = pEl->m_wsContentSubType = pEl->m_wsContentType =
        pEl->m_wsDateSent = pEl->m_wsMessage = pEl->m_wsPriority = pEl->m_wsSubject =
        pEl->m_wsTo = pEl->m_wsFrom = wxEmptyString;
      pEl->m_bBlacklist = pEl->m_bBounce = pEl->m_bDelete = pEl->m_bHaveMessage = false;
      // post a message to update the mail grid display
      // ends up calling MyFrame::OnUpdateDisplay()
      // it will look after releasing the allocated memory
      wxThreadEvent event( wxEVT_THREAD, POP3_MSG_UPDATE_EVENT );
      event.SetPayload( pEl );
      event.SetInt( n );   // index
//          event.SetExtraLong( 5  );  // not used for now
      event.SetString( _T("TOP ") + ar_Pop3CommandData.sm_wsAccountName );   // command string
      wxQueueEvent( this, event.Clone() );
      wasUidlLines.RemoveAt( 0 );
      wasListLines.RemoveAt( 0 );
      walSizes.RemoveAt( 0 );
      n++;
      if(chunk.memory)
      {
        free(chunk.memory);
        chunk.memory = NULL;
      }
    }
  }
  // Always cleanup
cleanup:
  if(chunk.memory)
  {
    free(chunk.memory);
    chunk.memory = NULL;
  }
  curl_easy_cleanup( pCurl );
  return;
}

// ------------------------------------------------------------------
/**
 * Parse the data returned from the interaction with the POP3 server.
 * This routine removes any UIDLs already found in the message list.
 * Any further downloading of data and parsing needs to be done by
 * other routines.
 * Compare the UIDLs in the newly returned list against those in m_Pop3MsgList
 */
void MyFrame::ThreadParseUidlData( Pop3CommandData &ar_Pop3CommandData,
  wxString a_wsUidlResult, wxArrayString &ar_wasUidls,
  wxString a_wsListResult, wxArrayString &ar_wasList,  wxArrayLong &ar_walSizes,
  wxString a_wsStatResult )
{
  wxString  wsT2;
  wxString  wsSeq;
  int       j = 0;
  wxArrayString wasUidls;
  wxArrayString wasList;
  // extract the list of UIDLs
  // wxStringTokenizer seems to have problems with these strings
  // use brute force - this works
  // wxStringTokenizer always seems to make line 2 a blank line
  wxString wsLine;
  wxString wsRest;
  unsigned long ulSeqNo;
  long     lSize;
  wxString wsSeqNo;

  // this code simply splits the lines and adds them to the
  // string array
  wsRest = a_wsUidlResult;
  j = 1;
  while ( !wsRest.IsEmpty() )
  {
    wsLine = wsRest.BeforeFirst( '\r' );
    wsRest = wsRest.AfterFirst( '\n' );
    // split off the sequence number
    wsSeqNo = wsLine.BeforeFirst( ' ' );
    if ( !wsSeqNo.ToULong( &ulSeqNo ) )
      continue; // line does not start with a numeric char, skip it
    ar_wasUidls.Add( wsLine );
    wsLine = wsLine.AfterFirst( ' ' );
    wasUidls.Add( wsLine );
    wxASSERT( ulSeqNo == (unsigned long)j );
    j++;
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( _T("UIDL: %ld - %s"), ulSeqNo, wsLine );
    }
  }
  // do the same for the LIST results
  wsRest = a_wsListResult;
  j = 1;
  while ( !wsRest.IsEmpty() )
  {
    wsLine = wsRest.BeforeFirst( '\r' );
    wsRest = wsRest.AfterFirst( '\n' );
    wsSeqNo = wsLine.BeforeFirst( ' ' );
    if ( !wsSeqNo.ToULong( &ulSeqNo ) )
      continue; // line does not start with a numeric char, skip it
    ar_wasList.Add( wsLine );
    wsLine = wsLine.AfterFirst( ' ' );
    wasList.Add( wsLine );
    wsLine.ToLong( &lSize );
    ar_walSizes.Add( lSize );
    wxASSERT( ulSeqNo == (unsigned long)j );
    j++;
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( _T("LIST: %ld - %s - %ld"), ulSeqNo, wsLine, lSize );
    }
  }

  size_t n = m_Pop3MsgList.size();
  if ( m_Pop3MsgList.size() == 0 )
    return;
  // run throught the list in m_Pop3MsgList and remove any lines in the current
  // display which no longer have a corresponding UIDL on the server.
  // Those have been removed one way or another.
  for (std::list<MyPop3MsgElement>::iterator it = m_Pop3MsgList.begin();
          it != m_Pop3MsgList.end(); ++it )
  {
    bool bFound = false;
    if ( it->m_wsAcctName.IsSameAs( ar_Pop3CommandData.sm_wsAccountName ))
    {
      // scan the incoming message list to see which UIDLs already exist
      // and remove these from both the UIDL and LIST arrays that will be
      // fed back to caller so the new headers can be retrieved
      for ( size_t i = 0; i < ar_wasUidls.GetCount(); i++ )
      {
        if ( it->m_wsUidl.IsSameAs( ar_wasUidls[i] ) )
        {
          bFound = true;  // this one is still on server
        }
      }
      if ( !bFound )
      {
        // enqueue a message to delete tthe line corresponding to this UIDL from the mail grid
        // post a message to update the mail grid display
        // ends up calling MyFrame::OnUpdateDisplay()
        // it will look after releasing the allocated memory
        wxString wsT;
        wxThreadEvent event( wxEVT_THREAD, POP3_MSG_UPDATE_EVENT );
        event.SetPayload( (MyPop3MsgElement *)NULL );
        event.SetInt( n );   // index
//          event.SetExtraLong( 5  );  // not used for now
        wsT.Printf( _T("DELE %s %s"), ar_Pop3CommandData.sm_wsAccountName, it->m_wsUidl );  
        event.SetString( wsT );   // UIDL
        wxQueueEvent( this, event.Clone() );
      }
    }
  }
  // now the items in both of the string arrays ought to correspond
  // since they we obtained in the same POP3 session
restart:
  for (std::list<MyPop3MsgElement>::iterator it = m_Pop3MsgList.begin();
          it != m_Pop3MsgList.end(); ++it, j++ )
  {
    if ( it->m_wsAcctName.IsSameAs( ar_Pop3CommandData.sm_wsAccountName ))
    {
      // scan the incoming message list to see which UIDLs already exist
      // and remove these from both the UIDL and LIST arrays that will be
      // fed back to caller so the new headers can be retrieved
      for ( size_t i = 0; i < ar_wasUidls.GetCount(); i++ )
      {
        if ( it->m_wsUidl.IsSameAs( ar_wasUidls[i] ) )
        {
          // we have found a match
          // the message with this UIDL is present in the list
          // probably from a previous scan, so remove this
          // item from both the UIDL & LIST array
          wxString wsT;
          wsT.Printf( _T("%s - %d: MisMatched sizes: %ld - %ld"),
            __FILE__, __LINE__,  it->m_lSize, ar_walSizes[i] );
          wxASSERT_MSG( it->m_lSize  == ar_walSizes[i], wsT );
          ar_wasUidls.RemoveAt( i );
          ar_wasList.RemoveAt( i );
          ar_walSizes.RemoveAt( i );
          goto restart; // we need to start over again
        }
      }
    }
  }
}

// ------------------------------------------------------------------
/**
 * Pass in a UIDL string of the form
 * "1 41112.S1DlDulSXMPlpsZday1pZQG4GqxGXNGQEOqRS8JhJOE=" or
 * without the sequence number prefix
 * "41112.S1DlDulSXMPlpsZday1pZQG4GqxGXNGQEOqRS8JhJOE="
 * and return the seqence number as found in the line with the
 * same UIDL in the list of server UIDLs
 * Note: 0 is NOT a valid sequence number
 *** from RFC 1939
    The unique-id of a message is an arbitrary server-determined
    string, consisting of one to 70 characters in the range 0x21
    to 0x7E, which uniquely identifies a message within a
    maildrop and which persists across sessions.  This
    persistence is required even if a session ends without
    entering the UPDATE state.  The server should never reuse an
    unique-id in a given maildrop, for as long as the entity
    using the unique-id exists.
  *
  * My note: char range 0x21 - 0x7e excludes spaces
  * My test server from Pablo Software adheres to this restriction,
  * even though it used the subject line as UIDL, but replaces all
  * spaces with dashes
 */
unsigned int MyFrame::GetSeqNoFromUidl( wxString a_wsUidl, wxArrayString &ar_wasUidls )
{
  unsigned long ulSeq = 0;
  wxString wsSeq;
  wxString wsServerUidl;
  if ( a_wsUidl.Find( ' ' ) != wxNOT_FOUND )
    a_wsUidl = a_wsUidl.AfterFirst( ' ' );
  for ( unsigned int i = 0; i < ar_wasUidls.GetCount();  i++ )
  {
    wsServerUidl = ar_wasUidls[i].AfterFirst( ' ' );
    if ( wsServerUidl.IsSameAs( a_wsUidl ) )
    {
      wsSeq = ar_wasUidls[i].BeforeFirst( ' ' );
      bool bRet = wsSeq.ToULong( &ulSeq );
      wxASSERT( bRet );
      wxUnusedVar( bRet );  // keep linux compiler happy in release mode
      wxASSERT( ulSeq );  // MUST be positive, > 0
      return (unsigned int)ulSeq;
    }
  }
  // seems that if we get here we have a problem ????
  // throwing the assertion here will be closer to the real issue
  // than waiting til we return
  wxASSERT( ulSeq );  // MUST be positive, > 0
  return (unsigned int)ulSeq;
}

// ------------------------------- eof ------------------------------
