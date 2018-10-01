/*-----------------------------------------------------------------
 * Name:        wxMsPop3Thead.cpp
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
// ==================================================================
/**
 * This routine is called for all conversations with the POP3 server.
 * At the start it only was intended to handle the first call after
 * startup, but later the code was adapted so it could be used for
 * the subsequent handling of other commands, such as DELE & RETR.
 * Input is via MyFrame variable m_Pop3CommandData, .....
 * Output via m_Pop3MsgList
 */
void MyFrame::RunLibcurlThread()
{
  wxASSERT( m_Pop3CommandData.sm_wasCommands.GetCount() );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsAccountName.IsEmpty(), m_Pop3CommandData.sm_wsAccountName );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsPop3ServerUrl.IsEmpty(), m_Pop3CommandData.sm_wsPop3ServerUrl );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsUserName.IsEmpty(), m_Pop3CommandData.sm_wsUserName );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsUserPassword.IsEmpty(), m_Pop3CommandData.sm_wsUserPassword );

  wxASSERT( m_Pop3CommandData.sm_wsResultData.IsEmpty() );
  if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
  {
    wxLogMessage( _T("RunLibcurlThread"));
  }
  // we want to start a long task, but we don't want our GUI to block
  // while it's executed, so we use a thread to do it.
  if (CreateThread(wxTHREAD_DETACHED) != wxTHREAD_NO_ERROR)
  {
    wxLogError("Could not create the worker thread!");
    m_bRunning = false;
    return;
  }
  // go!
  if (GetThread()->Run() != wxTHREAD_NO_ERROR)
  {
    wxLogError("Could not run the worker thread!");
    m_bRunning = false;
    return;
  }
  m_bRunning = true;
  // thread is not running yet, no need for crit sect
  m_bCancelled = false;
}

// ------------------------------------------------------------------
/**
 * Job/Task entry point
 */
wxThread::ExitCode MyFrame::Entry()
{
  // IMPORTANT:
  // this function gets executed in the secondary thread context!
  // here we do our long task, periodically calling TestDestroy():
  if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
  {
    wxLogMessage( _T("\n\nMyFrame::Entry"));
  }
  while (!GetThread()->TestDestroy() && !Cancelled()
#if defined( WANT_NEW_STOP )
    &&  !wxGetApp().m_bShuttingDown 
#endif
    )
  {
    // since this Entry() is implemented in MyFrame context we don't
    // need any pointer to access the m_data, m_processedData, m_dataCS
    // variables... very nice!                            
    ExecutePop3Command( m_Pop3CommandData );  // <<<<<<<<<<<<<<<<<<<<<< start work
    //                                  continue work at: wxMsCurlPop3ExecuteCmd.cpp
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( _T("ExecutePop3Command"));
    }
    // VERY IMPORTANT: do not call any GUI function inside this
    //                 function; rather use wxQueueEvent():
    // we're done, alert main thread and display the results
    // we used pointer 'this' assuming it's safe; see OnClose()
    break;
  }
  {
#if 1
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogMessage( _T("MyFrame::ExecutePop3Command Entry Done"));
    }
    // ensure no one reads m_data while we write it
    {
      wxCriticalSectionLocker lock( m_CS_Pop3MsgList );
      m_bRunning = false;
    }
    SetStatusText( _T(""), 1);
    // clear the progress gauge
    GetGaugeProgress()->SetValue( 0 );
#if defined( _MSC_VER )
    // and the taskbar icon's as well
    GetAppProgressInd()->SetValue( 0 );
#endif
#else
    // we could also queue a 'done' event
    // see the wxThreadHelper class example
    // or possibly wxBup
#endif
  }
#if defined( WANT_SEMAPHORE )
  // we only need one call to signal completion
  // TestDestroy() returned true (which means the main thread asked us
  // to terminate as soon as possible) or we ended the long task...
  if ( wxGetApp().m_bShuttingDown )
  {
    wxGetApp().m_bShuttingDown = false;
    // signal the main frame that we're done
    wxGetApp().m_semAllDone.Post();
  }
#endif
  return (wxThread::ExitCode)0;
}

// ------------------------------- eof ------------------------------
