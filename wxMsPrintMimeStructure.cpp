/*-----------------------------------------------------------------
 * Name:        wxMsPrintMimeStructure.cpp
 * Purpose:
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

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::istringstream
#include <fstream>
#include <iterator>
#include <streambuf>
#include "mimetic.h"
#include "utils.h"
using namespace std;
using namespace mimetic;
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
wxString MyFrame::PrintTabs(int c)
{
  wxString wsT;
  while(c--)
  {
    wsT += _T("   ");
  }
  return wsT;
}

/**
 * Print out the mime structure.
 * If a ptr to wxTextCtrl is supplied, append the text to that window
 * else do not send anything except to the log window if verbosity is
 * set high enough
 */
void MyFrame::printMimeStructure( MimeEntity* pMe, int a_iTabcount, wxTextCtrl *a_pTextCtrl )
{
  wxString wsT;
  wxString wsT1;
  wxString wsT2;
  wxString wsOutput;
  {
    Header& h = pMe->header();
    ContentType ct = h.contentType();
    wsT += PrintTabs(a_iTabcount);
    wsT1 = ct.type();
    wsT2 = ct.subtype();
    wsOutput.Printf( _T("%s%s/%s"), wsT, wsT1, wsT2 );
    if ( a_pTextCtrl )
      a_pTextCtrl->AppendText( wsOutput + _T("\n") );
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( wsOutput );
    }
    ContentType::ParamList::iterator bit, eit;
    bit = ct.paramList().begin();
    eit = ct.paramList().end();
    for(; bit != eit; ++bit)
    {
      wsT = PrintTabs(a_iTabcount);
      wsT1 = bit->name();
      wsT2 = bit->value();
      wsOutput.Printf( _T("%sparam: %s = %s"), wsT, wsT1, wsT2 );
      if ( a_pTextCtrl )
        a_pTextCtrl->AppendText( wsOutput + _T("\n") );
      if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
      {
        wxLogMessage( wsOutput );
      }
    }
    if(h.hasField(ContentTransferEncoding::label))
    {
      wsT = PrintTabs(a_iTabcount);
      wsT1 = h.contentTransferEncoding().mechanism();
      wsOutput.Printf( _T("%sencoding: %s"), wsT, wsT1 );
      if ( a_pTextCtrl )
        a_pTextCtrl->AppendText( wsOutput + _T("\n") );
      if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
      {
        wxLogMessage( wsOutput );
      }
    }
    if(h.hasField(ContentDisposition::label))
    {
      wsT = PrintTabs(a_iTabcount);
      const ContentDisposition cd = h.contentDisposition();
      wsT1 = cd.type();
      wsOutput.Printf(_T("%sdisposition: %s"), wsT, wsT1 );
      if ( a_pTextCtrl )
        a_pTextCtrl->AppendText( wsOutput + _T("\n") );
      if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
      {
        wxLogMessage( wsOutput);
      }
      ContentDisposition::ParamList::const_iterator bit, eit;
      bit = cd.paramList().begin();
      eit = cd.paramList().end();
      for(; bit != eit; ++bit)
      {
        wsT = PrintTabs(a_iTabcount);
        wsT1 = bit->name();
        wsT2 = bit->value();
        wsOutput.Printf( _T("%sparam: %s = %s"), wsT, wsT1, wsT2 );
        if ( a_pTextCtrl )
          a_pTextCtrl->AppendText( wsOutput + _T("\n") );
        if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
        {
          ;//wxLogMessage( wsOutput );
        }
      }
    }
    Header::iterator hbit, heit;
    hbit = pMe->header().begin();
    heit = pMe->header().end();
    for(; hbit != heit; ++hbit)
    {
      wsT = PrintTabs(a_iTabcount);
      wsT1 = hbit->name();
      wsT2 = hbit->value();
      wsOutput.Printf( _T("%sh: %s = %s"), wsT, wsT1, wsT2 );
      if ( a_pTextCtrl )
        a_pTextCtrl->AppendText( wsOutput + _T("\n") );
      if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
      {
        ;//wxLogMessage( wsOutput );
      }
    }
    if(pMe->body().preamble().length())
    {
      wsT = PrintTabs(a_iTabcount);
      wsT1 = pMe->body().preamble();
      wsT.Replace( _T("%"), _T("%%"), true );
      wsT1.Replace( _T("%"), _T("%%"), true );
      wsOutput.Printf( _T("%spreamble: %s"), wsT, wsT1 );
      if ( a_pTextCtrl )
        a_pTextCtrl->AppendText( wsOutput + _T("\n") );
      if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
      {
        wxLogMessage( wsOutput );
      }
    }
    if(pMe->body().epilogue().length())
    {
      wsT = PrintTabs(a_iTabcount);
      wsT1 = pMe->body().epilogue();
      wsOutput.Printf( _T("%sepilogue: %s"), wsT, wsT1 );
      if ( a_pTextCtrl )
        a_pTextCtrl->AppendText( wsOutput + _T("\n") );
      if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
      {
        wxLogMessage( wsOutput );
      }
    }
    wsT = PrintTabs(a_iTabcount);
    wsOutput.Printf( _T("%spart size: %ld"), wsT, pMe->size() );
    if ( a_pTextCtrl )
      a_pTextCtrl->AppendText( wsOutput  + _T("\n") );
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( wsOutput );
    }
    wsT = PrintTabs(a_iTabcount);

    wsOutput.Printf( _T("%sbody size: %ld"), wsT, pMe->body().length() );
    if ( a_pTextCtrl )
      a_pTextCtrl->AppendText( wsOutput + _T("\n") );
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( wsOutput );
    }
    // issue a separator line
    if ( a_pTextCtrl )
      a_pTextCtrl->AppendText( _("---- MIME body parts separator ---\n") );
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( _("---- MIME body parts separator ---") );
    }
  }
  MimeEntityList::iterator mbit = pMe->body().parts().begin(),
                  meit = pMe->body().parts().end();
  for(;mbit!=meit;++mbit)
  {
    // add one 'tab' in spacesV
    printMimeStructure(*mbit, 3 + a_iTabcount, a_pTextCtrl );
  }
}


// ------------------------------- eof ------------------------------
