/*-----------------------------------------------------------------
 * Name:        wxMsBounceMsgs.cpp
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
 * Toggle the 'bounce' check box
 */
void MyFrame::OnMarkToBounceSelected(wxCommandEvent& event)
{
  // Get selected row
  wxString wsValue;
  wxArrayInt waiRows = m_gridMail->GetSelectedRows();
  if ( (waiRows.GetCount() > 1) || ( waiRows.GetCount() == 0 )  )
    return;
  unsigned long lIndex = waiRows[0];
  wsValue = m_gridMail->GetCellValue( lIndex, 1 );
  if( wsValue.IsSameAs( _T("1") ) )
  {
    m_gridMail->SetCellValue( lIndex, 1, _T("0") );
  }
  else
  {
    m_gridMail->SetCellValue( lIndex, 1, _T("1") );
  }
}

// ------------------------------------------------------------------
void MyFrame::OnMarkToBounceAll(wxCommandEvent& event)
{
  m_gridMail->BeginBatch();
  int i = 0;

  for ( i = 0; i <  m_gridMail->GetNumberRows(); i++ )
  {
    m_gridMail->SetCellValue( i, 1, _T("1") );
  }
  m_gridMail->EndBatch();
}

// ------------------------------------------------------------------
void MyFrame::OnClearAllBounce( wxCommandEvent& event)
{
  m_gridMail->BeginBatch();
  int i = 0;

  for ( i = 0; i <  m_gridMail->GetNumberRows(); i++ )
  {
    m_gridMail->SetCellValue( i, 1, _T("0") );
  }
  m_gridMail->EndBatch();
}

// ------------------------------------------------------------------
void MyFrame::BounceMarkedMessages()
{
  m_gridMail->BeginBatch();
  int i = 0;
  wxString wsValue;
  int    iDeletes = 0;

  for ( i = 0; i <  m_gridMail->GetNumberRows(); i++ )
  {
    wsValue = m_gridMail->GetCellValue( i, 1 );
    if( wsValue.IsSameAs( _T("1") ) )
    {
      iDeletes++;
      // now bounce each one
    }
  }
  m_gridMail->EndBatch();
  wxLogMessage( _("%d messages bounced"), iDeletes );
}

// ------------------------------- eof ------------------------------
