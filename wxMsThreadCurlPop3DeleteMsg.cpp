/*-----------------------------------------------------------------
 * Name:        wxMsThreadCurlPop3DeleteMsg.cpp
 * Purpose:     Code to delete one or more messages given the 
 *              message's UIDL
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
void MyFrame::ThreadCurlPop3DeleteMessage( Pop3CommandData &ar_Pop3CommandData )
{
  CURL *pCurl;
  CURLcode res = CURLE_OK;
  struct MemoryStruct chunk;
  chunk.memory = (char*) calloc(1, sizeof( char ) );
  chunk.size = 0;

  wxString wsUidlResult;    // result of UIDL command
  wxString wsUidl;
  wxString wsAccount;       // needed as feedback for updating the mail grid
  wsAccount = ar_Pop3CommandData.sm_wsAccountName;
  wxString wsPop3Server = _T("pop3://");
  wsPop3Server += ar_Pop3CommandData.sm_wsPop3ServerUrl.mb_str(wxConvUTF8).data();
  wxString wsCommand =  ar_Pop3CommandData.sm_wasCommands[0];
#if defined( _DEBUG )
  wxString wsUser = ar_Pop3CommandData.sm_wsUserName;
  wxString wsPassword = ar_Pop3CommandData.sm_wsUserPassword;
  wxString wsServer = wsPop3Server;
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
    //  issue a UIDL command     <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    wsCommand = _T("UIDL");
    curl_easy_setopt( pCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt( pCurl, CURLOPT_WRITEDATA, (void *)&chunk);
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
      wsT.Printf( _T("ThreadCurlPop3DeleteMessage: UIDL: curl_easy_perform() failed: %s"), curl_easy_strerror(res));
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
      wxLogMessage( _T("ThreadCurlPop3DeleteMessage UIDLs:\n  %s"), wsUidlResult );
    }
    // does user want us to quit?
    if ( GetThread()->TestDestroy() || Cancelled() )
    {
      goto cleanup;
    }
    //   parse the UIDLs into a string array
    wxArrayString wasUidlLines;
    // split the long result into individual UIDLs
    ThreadParseUidls( wsUidlResult, wasUidlLines, _T("Delete") );
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< DELE commands
    // else, while we are still connected to the server, we now 
    // need to issue DELE commands to delete the messages corresponding
    // to the UIDLs passed in
    // Do not perform a transfer as DELE returns no data
    curl_easy_setopt( pCurl, CURLOPT_NOBODY, 1L);

    size_t i;
    for ( i = 0; i < ar_Pop3CommandData.sm_wasCommands.GetCount(); i++ )
    {
      wsCommand =  ar_Pop3CommandData.sm_wasCommands[i];
      // ------Get the  sequence number we need
      // this time the string looks like
      // "DELE nn zxcvbnm12345  ....." all chars' except spaces in UIDL
      wsUidl = wsCommand.AfterFirst( ' ' );
      // "nn zxcvbnm12345  ....." 
      long lSeq = GetSeqNoFromUidl( wsUidl, wasUidlLines );
#if 1
      if ( lSeq <= 0 )
      {
        // if a message we think exists on the server has been deleted
        // possibly by another user loggong in or for any reason
        // by the server itself, we will get to this point.
        // All we can do is to carry on,
        // BUT, we need to clean up the user display, 
        // so wepost a message to update the mail grid display
        // which ends up calling MyFrame::OnUpdateDisplay()
        wxThreadEvent event( wxEVT_THREAD, POP3_MSG_UPDATE_EVENT );
        wxString wsT;
        event.SetPayload( (MyPop3MsgElement *)NULL );
        event.SetInt( lSeq - 1 );   // index
  //      event.SetExtraLong( 5  );  // not used for now
        wsT.Printf( _T("DELE\u00AE%s\u00AE%s"), ar_Pop3CommandData.sm_wsAccountName,wsUidl );  
        event.SetString( wsT );   // command string
        wxQueueEvent( this, event.Clone() );
        continue;
      }
#if 1
#else
      if ( lSeq <= 0 )
      {
        wxString wsT;
        wxBell(); wxBell(); wxBell();
        wxLogMessage(
          _T("!!! %s %ld: GetSeqNoFromUidl failed:\n    %s"), __FILE__, __LINE__, wsUidl );
        continue;
      }
#endif
#endif
      // Set the command with the proper message number
      wsCommand.Printf( _T("DELE %d"), lSeq );
      curl_easy_setopt( pCurl, CURLOPT_CUSTOMREQUEST, wsCommand.mb_str(wxConvUTF8).data());
      /* Perform the custom request */
      res = curl_easy_perform( pCurl );
      /* Check for errors */
      if(res != CURLE_OK)
      {
        wxString wsT;
        wxLogMessage( _T("ThreadCurlPop3DeleteMessage: %s: curl_easy_perform() failed: %s"),
          wsCommand, curl_easy_strerror(res));
        wxLogDebug( _T("ThreadCurlPop3DeleteMessage: %s: curl_easy_perform() failed: %s"),
          wsCommand, curl_easy_strerror(res));
        wsT = curl_easy_strerror(res);
        ar_Pop3CommandData.sm_wsResultData = _T("Error: ") + wsT;
        // clean up memory
        goto cleanup;
      }
      // post a message to update the mail grid display
      // ends up calling MyFrame::OnUpdateDisplay()
      wxThreadEvent event( wxEVT_THREAD, POP3_MSG_UPDATE_EVENT );
      wxString wsT;
      event.SetPayload( (MyPop3MsgElement *)NULL );
      event.SetInt( lSeq - 1 );   // index
//      event.SetExtraLong( 5  );  // not used for now
      wsT.Printf( _T("DELE\u00AE%s\u00AE%s"), ar_Pop3CommandData.sm_wsAccountName,wsUidl );  
      event.SetString( wsT );   // command string
      wxQueueEvent( this, event.Clone() );
#if defined( WANT_UIDL_ASSERT )
      // seems needed to give slower machines time to update the GUI
      // and still lets us delete a number of messages in one call
      // there are other ways to to this, but .....
      wxThread::Sleep( 50 );
#endif
    }
    // does user want us to quit?
    if ( GetThread()->TestDestroy() || Cancelled() )
      return;
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

// ------------------------------- eof ------------------------------
