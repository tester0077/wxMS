/*-----------------------------------------------------------------
 * Name:        wxMsContextMenu.cpp
 * Purpose:     Mail grid context menu handlers
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
#include "wxMsh.h"
#include "wxMsFrameh.h"
// ------------------------------------------------------------------
// Note __VISUALC__ is defined by wxWidgets, not by MSVC IDE
// and thus won't be defined until some wxWidgets headers are included
#if defined( _MSC_VER )
// only good for MSVC
// this block needs to AFTER all headers
#include <stdlib.h>
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
 * Get the job tree item info. Note we cannot get the label directly.
 * Likely have to add it as a data item.
 */
void MyFrame::OnMailGridRightClick( wxGridEvent& event)
{
  m_iRightClickRow = event.GetRow();
  wxPoint pos = event.GetPosition();
  ShowContextMenu( pos );
}

// ------------------------------------------------------------------
/**
 * Depending on where the right-click happemns, we show a different
 * menu.
 * Right-click anywhere out side the grid we show this menu:
 *           Clear message list
 *           ------------------------
 *           Mark all for deletion
 *           Unmark all for deletion
 *           ------------------------
 *           Mark all fir bouncing
 *           Unmark all for bouncing
 *
 * Right-click on a message, we show the menu:
 *           Add to black list (-)
 *           Add to friends list  (+)
 *           Add the entire domain to the black list
 *           Add the entire domain to the friends list
 *           ------------------------------------------
 *           Delete message
 *           Bounce message
 *           ------------------------------------------
 *           Mark for deletion
 *           Mark for bouncing
 *           ------------------------------------------
 *           Preview message (Space)
 *           Show header
 */

void MyFrame::ShowContextMenu( const wxPoint& pos )
{
  wxMenu menu;
  wxString wsValue;
  wxString wsPrompt;
  menu.Append(wxID_CONTEXT_PREVIEW_MSG,       _T("Preview message - Alt-Space"));
  menu.Append(wxID_CONTEXT_DELETE_MSG,        _T("Delete msg - DEL"));
  menu.AppendSeparator();
  wsValue = m_gridMail->GetCellValue( m_iRightClickRow, 0 );
  if( wsValue.IsSameAs( _T("1") ) )
  {
    wsPrompt = _("Unmark ");
  }
  else
  {
    wsPrompt = _("Mark ");
  }
  menu.Append(wxID_CONTEXT_MARK_DEL_MSG,      wsPrompt + _T("for deletion"));
  wsValue = m_gridMail->GetCellValue( m_iRightClickRow, 1 );
  if( wsValue.IsSameAs( _T("1") ) )
  {
    wsPrompt = _("Unmark ");
  }
  else
    {
    wsPrompt = _("Mark ");
  }
  menu.Append(wxID_CONTEXT_MARK_BOUNCE_MSG,   wsPrompt + _T("for bouncing"));
#if 0 // skip or now
  menu.AppendSeparator();
  menu.Append(wxID_CONTEXT_ADD_BLACK_MSG,     wxT("Add to 'black' list"));
  menu.Append(wxID_CONTEXT_ADD_FRIEND_MSG,    wxT("Add to 'friend' list"));
#endif
  PopupMenu( &menu, pos.x, pos.y);
}

// ------------------------------------------------------------------
/**
 * Handle the right-click pop-up menu item to preview the currently
 * selected message.
 */
void MyFrame::OnContextPreview( wxCommandEvent&  event  )
{
  int iSelCol = m_gridMail->GetGridCursorCol();
  wxString wsAccount = m_gridMail->GetCellValue( m_iRightClickRow, 9 );
  m_gridMail->SelectRow( m_iRightClickRow );
  m_gridMail->GoToCell( m_iRightClickRow, iSelCol );
  PreviewMessage( wsAccount, (long)m_iRightClickRow  + 1 );
}

// ------------------------------------------------------------------
/**
 * Handle the right-click pop-up menu item to delete the currently
 * selected message.
 */
void MyFrame::OnContextDelMsg( wxCommandEvent& WXUNUSED(event ) )
{
  long lIndex = m_iRightClickRow;
  wxString wsUidl;
  wxString wsAccount = m_gridMail->GetCellValue( lIndex, 8 );
  FrameDeleteMessage( wsAccount, lIndex + 1,  wsUidl );
}

// ------------------------------------------------------------------
/**
 * Handle the right-click pop-up menu item to mark the specific
 * message for deletion
 */
void MyFrame::OnContextMarkDelMsg( wxCommandEvent& WXUNUSED( event ) )
{
  wxString wsValue;
  wsValue = m_gridMail->GetCellValue( m_iRightClickRow, 0 );
  if( wsValue.IsSameAs( _T("1") ) )
  {
    m_gridMail->SetCellValue( m_iRightClickRow, 0, _T("0") );
  }
  else
  {
    m_gridMail->SetCellValue( m_iRightClickRow, 0, _T("1") );
  }
}

// ------------------------------------------------------------------
/**
 * Handle the right-click pop-up menu item to mark the specific
 * message for bouncing
 */
void MyFrame::OnContextMarkBounceMsg( wxCommandEvent& WXUNUSED( event ) )
{
  wxString wsValue;
  wsValue = m_gridMail->GetCellValue( m_iRightClickRow, 1 );
  if( wsValue.IsSameAs( _T("1") ) )
  {
    m_gridMail->SetCellValue( m_iRightClickRow, 1, _T("0") );
  }
  else
  {
    m_gridMail->SetCellValue( m_iRightClickRow, 1, _T("1") );
  }
}

// ------------------------------------------------------------------
/**
 * Handle the right-click pop-up menu item to add the specific
 * message to the 'black' list also removes it from the friends list
 */
void MyFrame::OnContextAddBlackMsg( wxCommandEvent& WXUNUSED( event ) )
{
  m_gridMail->SetCellValue( m_iRightClickRow, 2, _T("1") );
}

  // ------------------------------------------------------------------
/**
 * Handle the right-click pop-up menu item to add the specific
 * message to the 'friends' list, also removes it from the black list
 */
void MyFrame::OnContextAddFriendMsg( wxCommandEvent& WXUNUSED( event ) )
{
  m_gridMail->SetCellValue( m_iRightClickRow, 0, _T("0") );
}

// ------------------------------- eof ------------------------------
