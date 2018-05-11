/*-----------------------------------------------------------------
 * Name:        wxMsMailGridChar.cpp
 * Purpose:     Handle key presses in the mail grid
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
void MyFrame::OnMailGridChar(wxKeyEvent& event)
{
  long keycode = event.GetKeyCode();
  int iNRows = m_gridMail->GetNumberRows();
  int iNCols = m_gridMail->GetNumberCols();
  int iSelRow = m_gridMail->GetGridCursorRow();
  int iSelCol = m_gridMail->GetGridCursorCol();
  switch ( keycode )
  {
  case WXK_DOWN:
    if( iSelRow < iNRows )
    {
      m_gridMail->MoveCursorDown( false );
    }
    m_gridMail->SelectRow( iSelRow );
    m_gridMail->GoToCell( iSelRow, iSelCol );
    break;
  case WXK_UP:
    if( iSelRow > 0 )
    {
      m_gridMail->MoveCursorUp( false );
    }
    m_gridMail->SelectRow( iSelRow );
    m_gridMail->GoToCell( iSelRow, iSelCol );
    break;
  case WXK_RIGHT:
    if( iSelCol < iNCols )
    {
      m_gridMail->MoveCursorRight( false );
    }
    m_gridMail->SelectRow( iSelRow );
    m_gridMail->GoToCell( iSelRow, iSelCol );
    break;
  case WXK_LEFT:
    if( iSelCol > 0 )
    {
      m_gridMail->MoveCursorLeft( false );
    }
    m_gridMail->SelectRow( iSelRow );
    m_gridMail->GoToCell( iSelRow, iSelCol );
    break;
  default:
    event.Skip();
    break;
  }
}

// ------------------------------- eof ------------------------------
