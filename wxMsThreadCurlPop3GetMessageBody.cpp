/*-----------------------------------------------------------------
 * Name:        wxMsThreadCurlPop3GetMessageBody.cpp
 * Purpose:     get the message body for previewing
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
// ------------------------------------------------------------------
/**
 * Handles the command to retrieve the message body for preview.
 * Instead of the message sequence number, we pass in the UIDL
 * and then get the UIDLS of all messages on the server to find
 * the corresponding sequence number needed for the actual RETR
 * command.
 * Result will need to be parsed bt caller.
 * The data is returned in Pop3CommandData.sm_wsResultData
 */
void MyFrame::ThreadGetCurlPop3OneMessageBody( Pop3CommandData &ar_Pop3CommandData )
{
  CURL *pCurl;
  CURLcode res = CURLE_OK;
  struct MemoryStruct chunk;
  wxString wsUidlResult;    // result of UIDL command

  wxASSERT( ar_Pop3CommandData.sm_wasCommands.GetCount() );
  chunk.memory = (char*) malloc(1);
  chunk.size = 0;
  wxString wsPop3Server = _T("pop3://");
  wsPop3Server += ar_Pop3CommandData.sm_wsPop3ServerUrl.mb_str(wxConvUTF8).data();
#if defined( _DEBUG )
  wxString wsUser = ar_Pop3CommandData.sm_wsUserName;
  wxString wsPassword = ar_Pop3CommandData.sm_wsUserPassword;
  wxString wsServer = wsPop3Server;

#endif
  wxString wsCommand =  ar_Pop3CommandData.sm_wasCommands[0];
  wxString wsUidl = wsCommand.AfterFirst( ' ' );
  pCurl = curl_easy_init();
  if(pCurl)
  {
    curl_easy_setopt( pCurl,CURLOPT_USERNAME, ar_Pop3CommandData.sm_wsUserName.mb_str(wxConvUTF8).data());
    curl_easy_setopt( pCurl,CURLOPT_PASSWORD, ar_Pop3CommandData.sm_wsUserPassword.mb_str(wxConvUTF8).data() );
    curl_easy_setopt( pCurl, CURLOPT_URL, wsPop3Server.mb_str(wxConvUTF8).data());
    curl_easy_setopt( pCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt( pCurl, CURLOPT_WRITEDATA, (void *)&chunk);
    // allow some control over connection timeout - set via INI for now
    curl_easy_setopt( pCurl, CURLOPT_CONNECTTIMEOUT, g_iniPrefs.data[IE_POP3_SERVER_TIMEOUT].dataCurrent.lVal );
    //some servers needs this validation
//    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    //  else issue a UIDL command     <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    wsCommand = _T("UIDL");
    curl_easy_setopt( pCurl, CURLOPT_CUSTOMREQUEST, wsCommand.mb_str(wxConvUTF8).data() );
    /* Perform the custom request */
    res = curl_easy_perform( pCurl );
    /* Check for errors */
    if (CURLE_OPERATION_TIMEDOUT == res)
    {
      wxString wsT;
      wsT.Printf(  _T("Timeout for %s"), wsPop3Server );
      wxLogMessage( wsT );
      SetStatusText( wsT, 1 );
      // clean up memory
      goto cleanup;
    }
    else if( res != CURLE_OK )
    {
      wxString wsT;
      wsT.Printf( _T("GetMsgBody: UIDL: curl_easy_perform() failed: %s"), curl_easy_strerror(res));
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
      wxLogMessage( _T("GetMsgBody UIDLs:  %s\n"), wsUidlResult );
    }
    // ------Get the  sequence number we need
    wxArrayString wasUidlLines;
    // split the long result into individual UIDLs
    ThreadParseUidls( wsUidlResult, wasUidlLines );

    unsigned int uiSeq = GetSeqNoFromUidl( wsUidl, wasUidlLines );
    wxASSERT( uiSeq );  // MUST be positive, > 0
    // ------------------------------------------------------------------
    // Set the command with the proper message number
    wsCommand.Printf( _T("RETR %d"), uiSeq );
    curl_easy_setopt( pCurl, CURLOPT_CUSTOMREQUEST, wsCommand.mb_str(wxConvUTF8).data());
    /* Perform the custom request */
    res = curl_easy_perform( pCurl );

    /* Check for errors */
    if(res != CURLE_OK)
    {
      wxString wsT;
      wxLogMessage( _T("GetMsgBody: %s: curl_easy_perform() failed: %s"),
        wsCommand, curl_easy_strerror(res));
      wxLogDebug( _T("GetMsgBody: %s: curl_easy_perform() failed: %s"),
        wsCommand, curl_easy_strerror(res));
      wsT = curl_easy_strerror(res);
      ar_Pop3CommandData.sm_wsResultData = _T("Error: ") + wsT;
      // clean up memory
      goto cleanup;
    }
    else
    {
      // seems we need to make sure the data is copied out of chunk.memory
      // to be saved before the memory is freed
      wxString wsTemp = chunk.memory;
      ar_Pop3CommandData.sm_wsResultData = wsTemp;
      if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
      {
        wxLogMessage( wsCommand );
        wxLogMessage( _T("%lu bytes retrieved"), (long)chunk.size );
        wxLogMessage( _T("%s"),chunk.memory );
      }
    }
    // clean up memory
    if(chunk.memory)
    {
        free(chunk.memory);
        chunk.memory = NULL;
    }
  }
cleanup:
  // clean up memory
  if(chunk.memory)
  {
      free(chunk.memory);
      chunk.memory = NULL;
  }
  // Always cleanup
  curl_easy_cleanup( pCurl );
  return;
}

// ==================================================================

/**
 * Parse the long string of UIDLs returned from the interaction with the
 * POP3 server.
 * This routine splits the UIDLs and adds them to the string array for
 * further processing
 */
void MyFrame::ThreadParseUidls( wxString a_wsUidlResult, wxArrayString &ar_wasUidls )
{
  wxString  wsSeq;
  int       j = 0;
  // extract the list of UIDLs
  // wxStringTokenizer seems to have problems with these strings
  // use brute force - this works
  // wxStringTokenizer always seems to make line 2 a blank line
  wxString wsLine;
  wxString wsRest;
  unsigned long ulSeqNo;
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
    wxASSERT( ulSeqNo == (unsigned long)j );
    j++;
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( _T("ThreadParseUidls UIDL: %ld - %s"), ulSeqNo, wsLine );
    }
  }
}

// ------------------------------- eof ------------------------------
