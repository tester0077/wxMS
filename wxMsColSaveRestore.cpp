/*-----------------------------------------------------------------
 * Name:        wxMsColSaveRestore.cpp
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
#include "wxMsFrameh.h"
#include "wxMsConfh.h"
#include <wx/tokenzr.h>
#include <wx/dynarray.h>
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
/**
 * Col width are save in the INI file as a string of integers
 * separated with '|'
 */
void MyFrame::RestoreColWidths()
{
  long        lWidth;
  wxString    token;

  m_walColWidths.Clear();

  wxStringTokenizer tokenizer(
    g_iniPrefs.data[IE_MAIL_GRID_COL_WIDTHS].dataCurrent.wsVal, "|");
  while ( tokenizer.HasMoreTokens() )
  {
    wxString token = tokenizer.GetNextToken();
    // process token here
    token.ToLong( &lWidth ) ;
    m_walColWidths.Add(lWidth );
  }
}

// ------------------------------------------------------------------

void MyFrame::SaveColWidths()
{
  int i;
  wxString wsT;
  g_iniPrefs.data[IE_MAIL_GRID_COL_WIDTHS].dataCurrent.wsVal =
    wxEmptyString;

  wxGridCellCoords topLeft( 0, 0 ), topRight( 0, m_gridMail->GetNumberCols() );
  wxGridSizesInfo sizeinfo = m_gridMail->GetColSizes();
  for ( i = topLeft.GetCol(); i <= topRight.GetCol() - 1; i++ )
  {
    wsT.Printf( _T("%d"), sizeinfo.GetSize( i ) );
    g_iniPrefs.data[IE_MAIL_GRID_COL_WIDTHS].dataCurrent.wsVal +=
       wsT + _T("|");
  }
  // strip last '|'
  g_iniPrefs.data[IE_MAIL_GRID_COL_WIDTHS].dataCurrent.wsVal.Truncate(
    g_iniPrefs.data[IE_MAIL_GRID_COL_WIDTHS].dataCurrent.wsVal.Length() - 1 );
}

// ------------------------------------------------------------------
/**
 * Col labels are save in the INI file as a string of strings
 * separated with '|'
 */
void MyFrame::RestoreColLabels()
{
  wxString        token;

  m_wasColLabels.Clear();

  wxStringTokenizer tokenizer(
    g_iniPrefs.data[IE_MAIL_GRID_COL_LABELS].dataCurrent.wsVal, "|");
  while ( tokenizer.HasMoreTokens() )
  {
    wxString token = tokenizer.GetNextToken();
    // process token here
    m_wasColLabels.Add( token );
  }
}

// ------------------------------------------------------------------

void MyFrame::SaveColLabels()
{
  int i;
  wxString wsT;
  g_iniPrefs.data[IE_MAIL_GRID_COL_LABELS].dataCurrent.wsVal =
    wxEmptyString;
  for ( i = 0; i < m_gridMail->GetNumberCols(); i++ )
  {
    g_iniPrefs.data[IE_MAIL_GRID_COL_LABELS].dataCurrent.wsVal +=
       m_gridMail->GetColLabelValue( i ) + _T("|");
  }
  // strip last '|'
  g_iniPrefs.data[IE_MAIL_GRID_COL_LABELS].dataCurrent.wsVal.Truncate(
    g_iniPrefs.data[IE_MAIL_GRID_COL_LABELS].dataCurrent.wsVal.Length() - 1 );
}

// ------------------------------- eof ------------------------------
