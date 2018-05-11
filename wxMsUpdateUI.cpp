/*-----------------------------------------------------------------
 * Name:         wxMsUpdateUI.cpp
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
void MyFrame::OnUpdateUiMailCheckStart(wxUpdateUIEvent& event)
{
  bool bLocalRun = true;
  // make sure we don't start a new check until
  // the current one is finished   
  {
    wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
    bLocalRun = m_bRunning;
  }
  event.Enable( !bLocalRun );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------
/**
 * Can't make this work - yet.
 * Seemingly mainly because I get all of the headers with one thread call.
 * Needs more work.
 */
void MyFrame::OnUpdateUiMailCheckStop(wxUpdateUIEvent& event)
{
  event.Enable( false /*m_bRunning */);
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------
/**
 * Always enabled - not if we have no messages
 */
void MyFrame::OnUpdateUiProcessMail(wxUpdateUIEvent& event)
{
  if ( m_Pop3MsgList.size() )
    event.Enable( true );
  else
    event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------
/**
 * Disable if no mail app path specified
 */
void MyFrame::OnUpdateUiLaunchMailApp(wxUpdateUIEvent& event)
{
  event.Enable( !g_iniPrefs.data[IE_MAIL_CLIENT_PATH].dataCurrent.wsVal.IsEmpty() );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnUpdateUiFriendsListAddr(wxUpdateUIEvent& event)
{
  event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnUpdateUiFriendsListDom(wxUpdateUIEvent& event)
{
  event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnUpdateUiBlackListAddr(wxUpdateUIEvent& event)
{
  event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnUpdateUiBlackListDom(wxUpdateUIEvent& event)
{
  event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnUpdateUiMarkDelete(wxUpdateUIEvent& event)
{
  if (  m_Pop3MsgList.size() )
    event.Enable( true );
  else
    event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnUpdateUiMarkBounce(wxUpdateUIEvent& event)
{
#if 1
  event.Enable( false );
#else
  if (  m_Pop3MsgList.size() )
    event.Enable( true );
  else
    event.Enable( false );
#endif
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnUpdateUiPreview(wxUpdateUIEvent& event)
{
  if ( m_Pop3MsgList.size() )
    event.Enable( true );
  else
    event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnUpdateUiMarkDeleteAll(wxUpdateUIEvent& event)
{
  if (  m_Pop3MsgList.size() )
    event.Enable( true );
  else
    event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnUpdateUiMarkBounceAll(wxUpdateUIEvent& event)
{
#if 1
  event.Enable( false );
#else
  if (  m_Pop3MsgList.size() )
    event.Enable( true );
  else
    event.Enable( false );
#endif
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnUpdateUiClearList(wxUpdateUIEvent& event)
{
  if (  m_Pop3MsgList.size() )
    event.Enable( true );
  else
    event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnUpdateUiCheckNew(wxUpdateUIEvent& event)
{
  event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnUpdateUiToolsCheckMsgs(wxUpdateUIEvent& event)
{
  event.Enable( true );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnUpdateUiToolsErrors(wxUpdateUIEvent& event)
{
  event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------
void MyFrame::OnUpdateUiLogShow( wxUpdateUIEvent& event)
{
  // set menu items as needed
  event.Check( g_iniPrefs.data[IE_LOG_WIN_WANTED].dataCurrent.bVal );
}

// ------------------------------------------------------------------
void MyFrame::OnUpdateUiLogClear( wxUpdateUIEvent& event)
{
  event.Enable( g_iniPrefs.data[IE_LOG_WIN_WANTED].dataCurrent.bVal );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------
void MyFrame::OnUpdateUIHelpMenu(wxUpdateUIEvent& event)
{
  event.Enable( m_bFoundHelpFile );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------
void MyFrame::OnUpdateUIHelpToolbar(wxUpdateUIEvent& event)
{
  event.Enable( m_bFoundHelpFile );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------
void MyFrame::OnUpdateUiClearAllBounce(wxUpdateUIEvent& event)
{
    event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------
void MyFrame::OnUpdateUiClearAllDelete(wxUpdateUIEvent& event)
{
  if (  m_Pop3MsgList.size() )
    event.Enable( true );
  else
    event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnUpdateUiToolsBlackFriendsList(wxUpdateUIEvent& event)
{
  event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------

void MyFrame::OnUpdateUiToolsStop(wxUpdateUIEvent& event)
{
  event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------------------------------------------
void MyFrame::OnUpdateUiEmailDelete(wxUpdateUIEvent& event)
{
  if (  m_Pop3MsgList.size() )
    event.Enable( true );
  else
    event.Enable( false );
  // don't skip these events!!!
//  event.Skip();
}

// ------------------------------- eof ------------------------------
