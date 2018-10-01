/*-----------------------------------------------------------------
 * Name:        wxMsOnProcessMail.cpp
 * Purpose:     Handler to process mail: delete, bounce messages,
 *              then, if any messages are left, invoke the mail client.
 *
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
 * This function needs to
 *  - bounce all messages marked for bouncing
 *  - flag all blacklisted messages
 *  - delete all messages marked for deletion
 *  - then invoke the mail client - if options is set
 *    but only if there are 'new messages' !
 */
void MyFrame::OnProcessMail(wxCommandEvent& event)
{
#if defined( WANT_SEMAPHORE )
  // make sure we're not in the middle of a current call to get mail
  wxGetApp().m_semAllDone.Wait();
#endif
#if defined( _DEBUG )
  wxLogMessage(_T("1 - %s - ProcessMail - Line %ld"), __FILE__, __LINE__ );
#endif
  bool bLocalRun = true;

  DeleteMarkedMessages();
#if defined( _DEBUG )
  wxLogMessage(_T("2 - %s - ProcessMail - Line %ld"), __FILE__, __LINE__ );
#endif
  // make sure we don't kick off the mail client until
  // the messages have been deleted 
  while ( bLocalRun )
  {
    {
      wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
      bLocalRun = m_bRunning;
    }
    {
#if defined( _MSC_VER )
      ::wxYield();
#endif
      ::wxMilliSleep( 10 );
#if defined( _DEBUG )
      wxLogMessage(_T("3 - %s - ProcessMail - Line %ld"), __FILE__, __LINE__ );
#endif
    }
  }
#if defined( _DEBUG )
  wxLogMessage(_T("4 - %s - ProcessMail - Line %ld"), __FILE__, __LINE__ );
#endif
  // this does not work on Black; we seem to get here before
  // the deleted message(s) is/atre removed from the list
  // clear the display - this seems to be the only way to have
  // this app behave on the slower machine Black
//   ClearMessages();
  DisplayMailList();
#if defined( _DEBUG )
  wxLogMessage(_T("5 - %s - ProcessMail - Line %ld"), __FILE__, __LINE__ );
#endif
  if (  m_Pop3MsgList.size() )  //no need to call client if there are no messages to download
  {

    if( g_iniPrefs.data[IE_LAUNCH_MAIL_CLIENT].dataCurrent.bVal )
    {
      wxString wsT = _T("C:\\Program Files (x86)\\Mozilla Thunderbird\\thunderbird.exe");
      InvokeMailClient( wsT );
    }
  }
  event.Skip();
}

// ------------------------------------------------------------------
/**
 * Note this thread is detached - can't wait on it.
 * We set the status here and then need to test in every loop inside the thread.
 */

void MyFrame::OnMailCheckStop(wxCommandEvent& WXUNUSED(event) )
{
  {
    wxCriticalSectionLocker lock(m_csCancelled);
    m_bCancelled = true;
  }
}

// ------------------------------- eof ------------------------------
