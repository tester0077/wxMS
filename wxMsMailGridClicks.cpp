/*-----------------------------------------------------------------
 * Name:        wxMsMailGridClicks.cpp
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
// ------------------------------------------------------------------
/**
 * Was intended to check operation of the grid check boxes, but it is not
 * called every click
 */
void MyFrame::CheckboxHandler( wxCommandEvent& event)
{
  // last test returned 1 -> select rows, even though it does NOT really
  // seem that way
//  long i = m_gridMail->GetSelectionMode();
  event.Skip();
}

// ------------------------------------------------------------------
void MyFrame::OnMailGridCellLeftClick(wxGridEvent& event)
{
  wxString wsValue;
  int iCol = event.GetCol();
  int iRow = event.GetRow();
  bool bCurrent = false;
#ifdef _DEBUG
  if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
  {
    wxLogMessage( _T("left click: row: %d, col: %d"), iRow, iCol );
  }
#endif
  m_gridMail->SelectRow( iRow );
  if( (iCol >= 0) &&( iCol <= 2) )
  {
#ifdef _DEBUG
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( _T(" show editor: row: %d, col: %d"), iRow, iCol );
    }
#endif
    m_gridMail->EnableCellEditControl();
    m_gridMail->ShowCellEditControl();
    wsValue = m_gridMail->GetCellValue( iRow, iCol );
    if( wsValue.IsSameAs( _T("1") ) ) // toggle the value
    {
      m_gridMail->SetCellValue( iRow, iCol, _T("") );
      bCurrent = false;
    }
    else
    {
      m_gridMail->SetCellValue( iRow, iCol, _T("1") );
      bCurrent = true;
    }
    m_gridMail->DisableCellEditControl();
    bool bDone = false;
    // also update the message list data
    for (std::list<MyPop3MsgElement>::iterator it = m_Pop3MsgList.begin();
            it != m_Pop3MsgList.end(); ++it )
    {
 //::     if( it->m_lSequence == iRow + 1 )
      {
        bDone =  true;
        switch( iCol )
        {
        case 0:
          it->m_bDelete = bCurrent;
          break;
        case 1:
          it->m_bBounce = bCurrent;
          break;
        case 2:
          it->m_bBlacklist = bCurrent;
          break;
        default:
          wsValue.Printf( _("Bad row/col: %d,%d"), iRow, iCol );
          wxLogMessage( wsValue );
          wxASSERT_MSG( true, wsValue );
          break;
        }
      }
      if ( bDone )
        break;  // exit the loop
    }
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( _T("selected: row: %d"), iRow );
    }
  }
}

// ------------------------------------------------------------------

void MyFrame::OnMailGridCellChanging(wxGridEvent& event)
{
#ifdef _DEBUG
  int iCol = event.GetCol();
  int iRow = event.GetRow();
  wxLogMessage( _T("cell changing: row: %d, col: %d"), iRow, iCol );
#endif
  event.Skip();
}

// ------------------------------------------------------------------
void MyFrame::OnMailGridCellChanged(wxGridEvent& event)
{
#ifdef _DEBUG
  int iCol = event.GetCol();
  int iRow = event.GetRow();
  wxLogMessage( _T("cell changed: row: %d, col: %d"), iRow, iCol );
#endif
  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnMailGridEditorShown(wxGridEvent& event)
{
#ifdef _DEBUG
  int iCol = event.GetCol();
  int iRow = event.GetRow();
  wxLogMessage( _T("editor shown: row: %d, col: %d"), iRow, iCol );
#endif
  event.Skip();
}

// ------------------------------------------------------------------
void MyFrame::OnMailGridMouseWheel(wxMouseEvent& event)
{
//  bool goingdown = event.GetWheelRotation() < 0; // Which way are we scrolling
  m_iWheelRotation += event.GetWheelRotation();
  int iLines = m_iWheelRotation / event.GetWheelDelta();
  m_iWheelRotation -= iLines * event.GetWheelDelta();
  wxArrayInt waiRows = m_gridMail->GetSelectedRows();
  if ( waiRows.GetCount() ) // nothing to do if no rows are selected
  {
    int iRow;
    if( iLines > 0 )
    {
      iRow = waiRows[0] - 1;
      m_gridMail->SelectRow( waiRows[0] - 1 );
    }
    else
    {
      iRow = waiRows[0] + 1;
      m_gridMail->SelectRow( waiRows[0] + 1 );
    }
    m_gridMail->MakeCellVisible( iRow, 0 );
  }
  event.Skip();
}

// ------------------------------- eof ------------------------------
