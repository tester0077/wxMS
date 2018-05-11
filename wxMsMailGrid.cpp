/*-----------------------------------------------------------------
 * Name:        wxMsMailGrid.cpp
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
#include "wx/colordlg.h"
#include "wxMsFrameh.h"
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
 * Initialize the 'results' grid.
 */
void MyFrame::InitializeMailGrid()
{
  size_t i = 0;
  m_gridMail->EnableEditing( false );
  m_gridMail->InsertCols( 0, m_wasColLabels.GetCount() );
  m_gridMail->SetRowLabelSize( 1 );
  m_gridMail->EnableGridLines( true );

  m_gridMail->SetSelectionMode( wxGrid::wxGridSelectRows );
  for ( i = 0; i <  m_wasColLabels.GetCount(); i++ )
  {
    m_gridMail->SetColLabelValue( i, m_wasColLabels[i] );
    m_gridMail->SetColSize( i, m_walColWidths[i] );
  }
#if defined( HIDE_COLS_1_2 )
  m_gridMail->HideCol( 1 );   // hide 'Bounce'
  m_gridMail->HideCol( 2 );   // hide 'Blacklist'
#endif
  ResizeMailGrid();
}

// ------------------------------------------------------------------
/**
 * This function is intended to resize the grid when the size of the
 * main frame or any of its components is changed.
 * Also after any of the editable data fields have been changed.
 * It was originally based on hgrid.h by Yuri Borsky, but I had to
 * make significant changes for this code.
 */
void MyFrame::ResizeMailGrid()
{
  int iFullWidth = 0, h;
  int iCols = m_gridMail->GetNumberCols();
  iCols = iCols;  // keep compiler quiet
  wxWindow *pWinParent = m_gridMail->GetGridWindow();
  pWinParent->GetClientSize( &iFullWidth, &h );
  // this is needed during startup, while the grid is being constructed
  if ( iFullWidth < 10 )
    return;
  if( m_wasColLabels.GetCount() <= 0 )
    return;
  m_gridMail->BeginBatch();
  size_t i = 0;
  // get total assigned width
  long lTotalWidth = 0;

  for ( i = 0; i <  m_wasColLabels.GetCount(); i++ )
  {
    if( i <  m_wasColLabels.GetCount() - 1 )
    {
      if ( m_gridMail->IsColShown( i ) )
        lTotalWidth += m_walColWidths[i];
    }
    else
      m_gridMail->SetColSize( i, std::max( 0, iFullWidth - (int)lTotalWidth ) );
  }
  m_gridMail->EndBatch();
}

// ------------------------------------------------------------------
void MyFrame::OnMailGridSize(wxSizeEvent& event)
{
  ResizeMailGrid();
  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnMailGridColSize( wxGridSizeEvent& ev )
{
  const int col = ev.GetRowOrCol();
  m_walColWidths[col] = m_gridMail->GetColSize(col);
//  wxLogMessage("Resized column %d, new width = %d",
//                col, m_gridMail->GetColSize(col));
  ResizeMailGrid();
  ev.Skip();
}

// ------------------------------- eof ------------------------------
