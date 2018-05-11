/*-----------------------------------------------------------------
 * Name:        wxMsGetMessageFilter.cpp
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
#include "wxMsFilterDialogh.h"
#include <wx/colour.h>
#include <wx/tokenzr.h>
// ------------------------------------------------------------------
// Note __VISUALC__ is defined by wxWidgets, not by MSVC IDE  // from Autohotkey-hummer.ahk
// and thus won't be defined until some wxWidgets headers are included
#if defined( _MSC_VER )
// this block needs to go AFTER all headers
#include <crtdbg.h>
#  ifdef _DEBUG
#    define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#    define new DEBUG_NEW
#  endif
#endif
// ------------------------------------------------------------------
/**
 * Apply the filters to a specific message line.
 * Still needs 'bounce' & 'black list'???
 */
void MyFrame::GetMessageFilterStatus( long a_lIndex, std::list<MyPop3MsgElement>::iterator a_it,
    bool& ar_bDelete, wxString& ar_wsStatus, wxString& ar_wsColor )
{
  // set the defaults
  ar_wsStatus = _T("Normal" );
  ar_wsColor= g_iniPrefs.data[IE_STATUS_DEFAULT_COLOR].dataCurrent.wsVal;

  int i = 0;
  wxString wsAddr;
  wxString wsItName;
  wxString wsAction;
  wxString wsSubject = a_it->m_wsSubject;
  ar_bDelete = false;

  bool     bFirstToken;
  wxUnusedVar( bFirstToken );
  // get the 'action' token
  for ( std::vector<MyFilterListEl>::iterator filterIt = wxGetApp().m_FilterList.begin();
    filterIt != wxGetApp().m_FilterList.end(); ++filterIt, i++ )
  {
    wxString wsItName = filterIt->m_wsName;
    wxString wsRuleText = filterIt->m_wsRuleText; // do NOT modify the ruke string
    wsRuleText = wsRuleText.MakeLower();
    wsRuleText = _T("*") + wsRuleText + _T("*");
    wsItName = wsItName.MakeLower();
    bFirstToken = true;
    wxStringTokenizer tokenizer( filterIt->m_wsAction, ",");
    while ( tokenizer. HasMoreTokens() )
    {
      // we should only have one token per action
      wxASSERT_MSG( bFirstToken, _("Too many 'action' tokens: ") + wsItName.MakeUpper() );
      bFirstToken = false;
      wxString token = tokenizer.GetNextToken();
      if ( token.IsSameAs( _T("legit" ) ) )
      {
        if ( filterIt->m_wsRuleSource.Find( _T("'From'") ) != wxNOT_FOUND )
        {
          wsAddr = a_it->m_wsFrom;//adr.GetName();
          // assume we have a quoted string if we have a leading '"'
          if( wsAddr.StartsWith( _T("\"") ) )
          {
            wsAddr = wsAddr.AfterFirst( '"' );
            wsAddr = wsAddr.BeforeLast( '"' );
          }
          wsAddr = wsAddr.MakeLower();
          // make a simple match string
          if( wsAddr.Matches( wsRuleText ) )
          {
            ar_wsColor = filterIt->m_wsColorHex;
            ar_wsStatus = filterIt->m_wsStatus;
            return;
          }
        }
        else if ( filterIt->m_wsRuleSource.Find( _T("'Subject'") ) != wxNOT_FOUND )
        {
          // make a simple match string
          if( wsSubject.MakeLower().Matches( wsRuleText ) )
          {
            ar_wsColor = filterIt->m_wsColorHex;
            ar_wsStatus = filterIt->m_wsStatus;
            return;
          }
        }
        else if ( filterIt->m_wsRuleSource.Find( _T("'To'") ) != wxNOT_FOUND )
        {
          wsAddr = a_it->m_wsTo;
          // make a simple match string
          if( wsAddr.Matches( wsRuleText ) )
          {
            ar_wsColor = filterIt->m_wsColorHex;
            ar_wsStatus = filterIt->m_wsStatus;
            return;
          }
        }
#if 0 // not implemented yet
        else if ( filterIt->m_wsRuleSource.Find( _T("'CC'") ) != wxNOT_FOUND )
        {
#if 0
          if ( ccArray.size() )
            ;
#else
          // make a simple match string
          if( wsAddr.Matches( wsRuleText ) )
          {
            ar_wsColor = filterIt->m_wsColorHex;
            ar_wsStatus = filterIt->m_wsStatus;
            return;
          }
#endif
        }
#endif
      }
      else  //  we need to look at the rest
      {
        if ( token.IsSameAs( _T("delete" ) ) )
        {
          if ( filterIt->m_wsRuleSource.Find( _T("'From'") ) != wxNOT_FOUND )
          {
            wsAddr = a_it->m_wsFrom;
            // assume we have a quoted string if we have a leading '"'
            if( wsAddr.StartsWith( _T("\"") ) )
            {
              wsAddr = wsAddr.AfterFirst( '"' );
              wsAddr = wsAddr.BeforeLast( '"' );
            }
            wsAddr = wsAddr.MakeLower();
            // make a simple match string
            if( wsAddr.MakeLower().Matches( wsRuleText ) )
            {
              ar_wsColor = filterIt->m_wsColorHex;
              ar_wsStatus = filterIt->m_wsStatus;
              ar_bDelete = true;
              return;
            }
          }
          else if ( filterIt->m_wsRuleSource.Find( _T("'Subject'") ) != wxNOT_FOUND )
          {
            // make a simple match string
            if( wsSubject.MakeLower().Matches( wsRuleText ) )
            {
              ar_wsColor = filterIt->m_wsColorHex;
              ar_wsStatus = filterIt->m_wsStatus;
              ar_bDelete = true;
              return;
            }
          }
          else if ( filterIt->m_wsRuleSource.Find( _T("'To'") ) != wxNOT_FOUND )
          {
            wsAddr = a_it->m_wsTo;
#if 0
            if ( toArray.size() )
              ;
#else
            // make a simple match string
            if( wsAddr.Matches( wsRuleText ) )
            {
              ar_wsColor = filterIt->m_wsColorHex;
              ar_wsStatus = filterIt->m_wsStatus;
              ar_bDelete = true;
              return;
            }
#endif
          }
#if 0 // not implemented yet
          else if ( filterIt->m_wsRuleSource.Find( _T("'CC'") ) != wxNOT_FOUND )
          {
            wsAddr = a_it->m_wsTo;
            // make a simple match string
            if( wsAddr.MakeLower().Matches( wsRuleText ) )
            {
              ar_wsColor = filterIt->m_wsColorHex;
              ar_wsStatus = filterIt->m_wsStatus;
              ar_bDelete = true;
              return;
            }
          }
#endif
        }
        else
          continue;
      }
    }
  }
}

// ------------------------------- eof ------------------------------
