// Name:        charsetconv.cpp
// Purpose:     This class implements the rfc2231 norm
// Author:      Brice André
// Created:     2010/12/12
// RCS-ID:      $Id: mycomp.cpp 505 2007-03-31 10:31:46Z frm $
// Copyright:   (c) 2010 Brice André
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

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

// includes
#ifndef WX_PRECOMP
   // here goes the #include <wx/abc.h> directives for those
   // files which are not included by wxprec.h
#include "wx/wx.h"
#endif

#include "wx/encconv.h"
#include "wx/fontmap.h"

#include "charsetconv.h"
#include "wxMsConfh.h"
// ------------------------------------------------------------------
// Note __VISUALC__ is defined by wxWidgets, not by MSVC IDE
// and thus won't be defined until some wxWidgets headers are included
#if defined( _MSC_VER )
// only good for MSVC
// this block needs to go AFTER all headers
#include <crtdbg.h>
#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif
#endif
extern iniPrefs_t g_iniPrefs;
// ------------------------------------------------------------------
/*!
 * This function computes the beginning of an UTF-8 stream by pointing after the BOM, if any
 * It seems that some configs of wxWidgets do not like the presence of BOM in an UTF-8 stream...
 */
static const char* AfterBOM(const char* stream)
{
  const unsigned char* ptr = (const unsigned char*)stream;
  if ((ptr[0] == 0xEF) &&
      (ptr[1] == 0xBB) &&
      (ptr[2] == 0xBF))
  {
    /* BOM coded on 3 bytes */
    return stream + 3;
  }
  else if ((ptr[0] == 0xF0) &&
          (ptr[1] == 0x8F) &&
          (ptr[2] == 0xBB) &&
          (ptr[3] == 0xBF))
  {
    /* BOM coded on 4 bytes */
    return stream + 4;
  }
  else
  {
    /* No BOM... */
    return stream;
  }
}

// ------------------------------------------------------------------

wxString myCharsetConverter::ConvertCharset(const wxString& ar_wsInStr, const wxString& ar_wsCharSet )
{
  wxString wsContent = ar_wsInStr; // make this the default
  wxString wsFaceName;
#if 0
  // ok, now get the corresponding encoding
  // see comments below
  wxFontEncoding fontenc = wxFontMapper::Get()->CharsetToEncoding( ar_wsCharSet );

  if ( fontenc == wxFONTENCODING_SYSTEM )
  {
    wxLogError(wxT("Charset '%s' is unsupported."), ar_wsCharSet );
    // may not be the best way to handle this, but ??????
    return ar_wsInStr;
  }
#endif

  // for some unknown - to wxWidgets - charsets, this code sometimes throws up a dialog
  // asking the user to select any one of the available char set encodings, but does not
  // wait for the user to make a choice - possibly because we are running in a secondary
  // thread.
  // in addition, once the error has come about, it is NOT reset for any subsequent
  // strings and the same error dialog is displayed - as often as there are remaining 
  // messages - until the last one is handled. Only then can the error dialogs be dismissed.
  // Evidently it has something to do with the default parameter interactive = true,
  // but it does not seem to be true in all cases
  wxFontEncoding text_encoding = wxFontMapper::Get()->CharsetToEncoding( ar_wsCharSet );
  wxFontEncoding system_encoding = wxLocale::GetSystemEncoding();
  if ( text_encoding == wxFONTENCODING_UTF8 )
  {
    wsContent = wxString::FromUTF8(AfterBOM(ar_wsInStr.mb_str(wxConvLocal)));
  }
  else if( text_encoding == wxFONTENCODING_DEFAULT )
  {
    wsContent = ar_wsInStr;
  }
  else if (system_encoding != text_encoding)
  {
    wxEncodingConverter converter;
//    wxASSERT_MSG( (text_encoding != wxFONTENCODING_UNICODE) && (system_encoding != wxFONTENCODING_UNICODE), 
//      _("Should not use wxFONTENCODING_UNICODE")  );
    bool can_convert = converter.Init(text_encoding, system_encoding);
    if (can_convert) 
    {
      wsContent = converter.Convert(ar_wsInStr);
    }
    else
    {
      /* What can we do ?? */

      // for now, use the MS Windows specific code
      // // see https://docs.microsoft.com/en-us/windows/desktop/intl/code-page-identifiers
      // for translation between the code received from the decoder and the MS code page identifier
      // 1b 24 42 36 62 30 52 1b 28 4a 20 76 69 61 20 63 75 72 6c 2d 6c 69 62 72 61 72 79 00 - old sample data
      if( ar_wsCharSet.IsSameAs( _T("ISO-2022-JP" )) )
      {
        int l = ar_wsInStr.Len();
        char ISO2022JP_DATA[128] = {0};
        int i = 0;
        for ( i = 0; i < l; i++ )
        {
          ISO2022JP_DATA[i] = ar_wsInStr.GetChar(i);
        }
#if 1
        TCHAR destbuffer[256];
        memset(destbuffer, 0, 256*sizeof(TCHAR) );

        ::MultiByteToWideChar (
                                50222,              // code page for ISO-2022-JP
                                0,                  // flags
                                ISO2022JP_DATA,     // input string
                                -1,                 // its length (NUL-terminated)
                                destbuffer,         // output string
                                512                 // size of output buffer
                              );
        wsContent = wxString(destbuffer) ;
        if( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 3)
          wxLogMessage( _T("%s: %s"), ar_wsCharSet, wsContent );
#else
        // these don't work with wxWidgets - yet
        wxCSConv conv("ISO-2022-JP");
        bool bOk = conv.IsOk();
        wxASSERT( bOk );
        wxString s1( ISO2022JP_DATA, conv);
        wxString s2( ISO2022JP_DATA, wxCSConv("ISO-2022-JP"));
        wxString s3( (const wchar_t *)ar_wsInStr, wxCSConv("ISO-2022-JP"));
        wxString s4( (const char *)ar_wsInStr, wxCSConv("ISO-2022-JP"));
        wxLogMessage( _T("%s: %s, %s, %s, %s"), ar_wsCharSet, s1, s2, s3, s4 );
#endif
      }
      else //if( ar_wsCharSet.IsSameAs( _T("GB2312" )) ||  ar_wsCharSet.IsSameAs( _T("GBK" )) )
      {
#if 0
        int l = ar_wsInStr.Len();
        char ISO2022JP_DATA[128] = {0};
        int i = 0;
        for ( i = 0; i < l; i++ )
        {
          ISO2022JP_DATA[i] = ar_wsInStr.GetChar(i);
        }
        TCHAR destbuffer[256];
        memset(destbuffer, 0, 256*sizeof(TCHAR) );

        ::MultiByteToWideChar (
                                936,              // code page for ISO-2022-JP
                                0,                  // flags
                                ISO2022JP_DATA,     // input string
                                -1,                 // its length (NUL-terminated)
                                destbuffer,         // output string
                                512                 // size of output buffer
                              );
        wsContent = wxString(destbuffer);
#endif
        wxCSConv conv( ar_wsCharSet );
        bool bOk = conv.IsOk();
        wxASSERT( bOk );
        wxString s1( (const char *)ar_wsInStr.c_str(), conv);
        wsContent = s1;
        if( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 3)
          wxLogMessage( _T("%s: %s"), ar_wsCharSet, s1 );
        if ( wsContent.Len() == 0 )
          wxLogError( _("Can't decode charset '%s'  ."), ar_wsCharSet );
#if 1
#else
        wxString s2( ISO2022JP_DATA, wxCSConv("GB2312"));
        wxString s3( (const wchar_t *)ar_wsInStr, wxCSConv("GB2312"));
        wxString s4( (const char *)ar_wsInStr, wxCSConv("GB2312"));
//        if( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 3)
          wxLogMessage( _T("%s: %s, %s, %s"), ar_wsCharSet,  s2, s3, s4 );

#endif
      }
//      else
//      {
//        // at least log the error return the raw bytes
//          wxLogError( _("Can't decode charset '%s'  ."), ar_wsCharSet );
//      }
    }
  }
  return wsContent;
}

// ------------------------------- eof ------------------------------
