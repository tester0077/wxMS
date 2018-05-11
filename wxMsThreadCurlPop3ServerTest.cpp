/*-----------------------------------------------------------------
 * Name:        wxMsThreadCurlPop3ServerTest.cpp
 * Purpose:     Code for the thread function to test connectivity to a
 *              POP3 server
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
 * Can handle a POP3 server test, uses CAPA command
 * Result to be parsed bt caller.
 * The data is returned in Pop3CommandData.sm_wsResultData
 * This function runs in secondary thread.
 */
bool MyFrame::ThreadCurlPop3ServerTest( Pop3CommandData &ar_Pop3CommandData )
{
  CURL *pCurl;
  CURLcode res = CURLE_OK;
  struct MemoryStruct chunk;

  wxASSERT( ar_Pop3CommandData.sm_wasCommands.GetCount() );
  chunk.memory = (char*) malloc(1);
  chunk.size = 0;
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
    curl_easy_setopt( pCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt( pCurl, CURLOPT_WRITEDATA, (void *)&chunk);

    //some servers needs this validation
//    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    /* Set the command */
    curl_easy_setopt( pCurl, CURLOPT_CUSTOMREQUEST, ar_Pop3CommandData.sm_wasCommands[0].mb_str(wxConvUTF8).data());
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
    else if ( res != CURLE_OK )
    {
      wxString wsT;
      wxLogMessage( _T("%s: curl_easy_perform() failed: %s"),
        ar_Pop3CommandData.sm_wasCommands[0], curl_easy_strerror(res));
      wxLogDebug( _T("%s: curl_easy_perform() failed: %s"),
        ar_Pop3CommandData.sm_wasCommands[0], curl_easy_strerror(res));
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
        wxLogMessage( ar_Pop3CommandData.sm_wasCommands[0] );
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
  // clean up memory
cleanup:
  if(chunk.memory)
  {
      free(chunk.memory);
      chunk.memory = NULL;
  }
  // Always cleanup
  curl_easy_cleanup( pCurl );
  return true;
}

// ------------------------------- eof ------------------------------
