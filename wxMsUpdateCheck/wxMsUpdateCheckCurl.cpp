/*-----------------------------------------------------------------
 * Name:        wxMsCurl.cpp
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
#include "wxMsUpdateCheckCurlh.h"
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
MyUpdateCurlHttpStatusObject::MyUpdateCurlHttpStatusObject ( wxString wsUrl, wxString wsUserAgent )
{
  char buf[100];
  strcpy( buf, (const char*)wsUrl.mb_str(wxConvUTF8) );
  code = CURLE_FAILED_INIT;
  pCurl = curl_easy_init();
  if( pCurl )
  {
    code = curl_easy_setopt( pCurl, CURLOPT_URL, buf );
    code = curl_easy_setopt( pCurl, CURLOPT_WRITEFUNCTION, &MyUpdateCurlHttpStatusObject::curlWriter);
    strcpy( buf, (const char*)wsUserAgent.mb_str(wxConvUTF8) );
    code = curl_easy_setopt( pCurl, CURLOPT_USERAGENT, buf );
    code = curl_easy_setopt( pCurl, CURLOPT_WRITEDATA, &curlBuffer);
    code = curl_easy_perform( pCurl);
  }
};

// ------------------------------------------------------------------

int MyUpdateCurlHttpStatusObject(char *data, size_t size, size_t nmemb, void *buffer)
{
  int result = 0;
  if (buffer != NULL)
  {
    ((std::string *)buffer)->append(data, size * nmemb);
    result = size * nmemb;
  }
  return result;
}

// ------------------------------------------------------------------

int MyUpdateCurlHttpStatusObject::curlWriter(char *data, size_t size, size_t nmemb, void *buffer)
{
  int result = 0;
  if (buffer != NULL)
  {
    ((std::string *)buffer)->append(data, size * nmemb);
    result = size * nmemb;
  }
  return result;
}

// ------------------------------- eof ------------------------------
