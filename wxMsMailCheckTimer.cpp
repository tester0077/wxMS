/*-----------------------------------------------------------------
 * Name:        wxMsMailCheckTimer.cpp
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
 * Timer to handle the mail check, if enabled
 * When started, the timer is set to 1 minute ( 60 * 1000)
 */
void MyFrame::OnMailCheckTimer( wxTimerEvent& event )
{
  // nothing to do if the mail check is not scheduled
  if ( g_iniPrefs.data[IE_SCHEDULE_MAIL_CHECK].dataCurrent.bVal )
  {
    m_iMailTimerTicks++;
    wxString wsT;
    wsT.Printf( _("Last mail check: %d min(s)"), m_iMailTimerTicks );
    if ( m_iMailTimerTicks >= g_iniPrefs.data[IE_MAIL_CHECK_INTERVAL].dataCurrent.lVal )
    {
      Check4NewMail();
      m_tMailCheckTimer.Start(1000 * 60 );
    }
    SetStatusText( wsT, 0 );
  }
  m_iConnectCheckTimerTicks++;
}

// ------------------------------------------------------------------
/**
 * Timer to handle the mail server connection timing check,
 * When started, the timer is set to 1 minute ( 60 * 1000)
 */
void MyFrame::OnMailConnectTimer( wxTimerEvent& event )
{
#if defined( WANT_MAIL_CONNECT_TEST )
  m_iConnectCheckTimerTicks++;
  if ( m_iConnectCheckTimerTicks > g_iniPrefs.data[IE_MAIL_CHECK_INTERVAL].dataCurrent.lVal )
  {
    CheckConnectivity();
  }
#endif
}

// ------------------------------------------------------------------
/**
 * Clear the status field 0 after a suitable time
 */
void MyFrame::OnMailDisplayTimer( wxTimerEvent& event )
{
  SetStatusText( wxEmptyString, 0 );
}

// ------------------------------- eof ------------------------------
