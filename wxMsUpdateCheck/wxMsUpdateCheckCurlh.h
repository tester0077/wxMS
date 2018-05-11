/*-----------------------------------------------------------------
 * Name:        wxMsCurlh.h
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets license
 *-------------------------------------------------------------- */
#ifndef _WX_MS_CURL_H
#define _WX_MS_CURL_H
#include "wxMsPreProcDefsh.h" // needs to be first
// avoids linker warnings & errors for curl_easy... functions
// though I haven't been able to figure out how
#include "curl/curl.h"
// ------------------------------------------------------------------
class MyUpdateCurlHttpStatusObject
{
  public:
  MyUpdateCurlHttpStatusObject(  wxString url, wxString wsUserAgent = _T("wxMs")  );
  ~MyUpdateCurlHttpStatusObject() { curl_easy_cleanup( pCurl ); };

  static int curlWriter(char *data, size_t size, size_t nmemb, void *buffer);

  std::string getData () { return curlBuffer; }
  CURLcode GetReturnCode() { return code; };
  wxString GetErrorString() { return curl_easy_strerror( code ); };

protected:
  CURL * pCurl;
  std::string curlBuffer;
  CURLcode code;
  wxString wsErrorString;
};

#endif  // _WX_MS__CURL_H
// ------------------------------- eof ---------------------------
