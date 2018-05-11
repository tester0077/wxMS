////////////////////////////////////////////////////////////////////////////
// Name:        rfc2231.cpp
// Purpose:     This class implements the rfc2047 norm
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

// For compilers that support precompilation, includes "wx.h".
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

#include "wx/regex.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "rfc2047.h"
#include "charsetconv.h"
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

wxString wxRfc2047::Decode(const wxString& encoded_str)
{
  /* Initialise decoded string */
  wxString decoded_str = encoded_str;

  /* Remove all white spaces between encoded parts, if any */
  {
    wxRegEx pattern(
    _T("(=\\077[[:alnum:].\\055_]+\\077[qQbB]\\077[^\\077]*\\077=)[[:blank:]]+\
    (=\\077[[:alnum:].\\055_]+\\077[qQbB]\\077[^\\077]*\\077=)"), wxRE_ADVANCED);
    pattern.ReplaceAll(&decoded_str, _T("\\1\\2"));
  }

  /* Perform Q decoding */
  {
    /* Check if we have pattern */
    wxRegEx pattern(
      _T("=\\077([[:alnum:].\\055_]+)\\077[qQ]\\077([^\\077]*)\\077="),
      wxRE_ADVANCED);
    while ((pattern.Matches(decoded_str)) &&
            (pattern.GetMatchCount() == 3))
    {
      /* Extract the encoded string */
      wxString str_content = pattern.GetMatch(decoded_str, 2);

      /* Replace all spaces */
      str_content.Replace(_T("_"), _T(" "));

      /* Handle all =xx paterns */
      int index;
      while (((index = str_content.Find(_T("="))) != wxNOT_FOUND) &&
            (index < int(str_content.length()-2)))
      {
        unsigned long val_long;
        str_content.Mid(index+1, 2).ToULong(&val_long, 16);
        char val[2];
        *((unsigned char*)val) = (unsigned char)val_long;
        val[1] = 0;

        str_content = str_content.Mid(0,index) << wxString(val, wxConvLocal) <<
          str_content.Mid(index+3);
      }

      /* Convert to local charset, if necessary */
      str_content = wxCharsetConverter::ConvertCharset(str_content,
        pattern.GetMatch(decoded_str, 1));

      /* Recode string before replacement */
      str_content.Replace(_T("\\"), _T("\\\\"));
      str_content.Replace(_T("&"), _T("\\&"));

      /* Replace in result */
      pattern.ReplaceFirst(&decoded_str, str_content);
    }
  }

  /* Perform B decoding */
  {
    /* Check if we have pattern */
    wxRegEx pattern(_T("=\\077([[:alnum:].\\055_]+)\\077[bB]\\077([^\\077]*)\\077="),
      wxRE_ADVANCED);
    // it seems we can have multiple 'B' strings - need to decode & concatenate them all
    while ((pattern.Matches(decoded_str)) &&
            (pattern.GetMatchCount() == 3))
    {
      /* Extract the encoded string */
      wxString str_content = pattern.GetMatch(decoded_str, 2);

      /* Perform a base64 decoding of the string */
      std::vector<unsigned char> buffer;
      std::string std_string = (const char*)str_content.mb_str(wxConvLocal);
      mimetic::Base64::Decoder b64;
      mimetic::decode(std_string.begin(),
                      std_string.end(),
                      b64,
                      std::back_inserter(buffer));

      /* Flush content in a string */
      str_content = _T("");
#if !defined( _MSC_VER )
      unsigned int uiSize = buffer.size() - 1; // move calc'n outside of loop
      char *pBuffer = (char *)calloc( uiSize + 5, sizeof( char) );
      for (unsigned char* p = &buffer[0], i = 0; p <= &buffer[uiSize]; p++, i++)
      {
        pBuffer[i] = *p;
      }
      //  str_content = wxString::FromUTF8( pBuf ); // does not work
      wxString wsT1( pBuffer, wxConvUTF8 );
      free( pBuffer );
      str_content = wsT1;

      /* Convert to local charset, if necessary */
      wxString wsConvert = pattern.GetMatch(decoded_str, 1);
      str_content = wxCharsetConverter::ConvertCharset(str_content,
        pattern.GetMatch(decoded_str, 1));
#else
      for (unsigned char* p = &buffer[0]; p <= &buffer[buffer.size()-1]; p++)
      {
        // complains because of hi bit set in some UTF-8 chars
        // stops the rest from working - works OK on Windows/MSVC 2015
        str_content.Append(*p, 1);
      }
      wxString wsConvert = pattern.GetMatch(decoded_str, 1);
      /* Convert to local charset, if necessary */
      str_content = wxCharsetConverter::ConvertCharset(str_content,
        pattern.GetMatch(decoded_str, 1));
#endif
      /* Recode string before replacement */
      str_content.Replace(_T("\\"), _T("\\\\"));
      str_content.Replace(_T("&"), _T("\\&"));
      /* Replace in result */
      pattern.ReplaceFirst(&decoded_str, str_content);
    }
  }
  /* Return the decoded string */
  return decoded_str;
}

// ------------------------------- eof ------------------------------
