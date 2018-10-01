/*-----------------------------------------------------------------
 * Name:        wxMsCurlPop3ExecuteCmd.cpp
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
#include <wx/tokenzr.h>
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
 * Execute a POP3 command. Called from the thread entry function.
 * Runs in secondary thread.
 * Input is via the function arguments
 * Output via queued events to m_Pop3MsgList
 */
void MyFrame::ExecutePop3Command( Pop3CommandData &ar_Pop3CommandData )
{
  if ( ar_Pop3CommandData.sm_wasCommands[0].StartsWith( _T("CAPA") ) )
  {
    wxASSERT( ar_Pop3CommandData.sm_wasCommands.GetCount() == 1 );
    ThreadCurlPop3ServerTest( m_Pop3CommandData );    // -> wxMsThreadCurlPop3ServerTest.cpp(62)
  } // using the pseudo command new to get new messages
  else if ( ar_Pop3CommandData.sm_wasCommands[0].StartsWith( _T("NEW") ) )
  {
    wxASSERT( ar_Pop3CommandData.sm_wasCommands.GetCount() == 1 );
    ThreadCurlPop3GetNewMessages( m_Pop3CommandData );  // -> wxMsThreadCurlPop3GetMessages.cpp(87)
  }
  else if ( ar_Pop3CommandData.sm_wasCommands[0].StartsWith( _T("RETR") ) )
  {
    wxASSERT( ar_Pop3CommandData.sm_wasCommands.GetCount() == 1 );
    ThreadGetCurlPop3OneMessageBody( m_Pop3CommandData ); // -> wxMsThreadCurlPop3GetMessageBody.cpp(65)
  }
  else if ( ar_Pop3CommandData.sm_wasCommands[0].StartsWith( _T("DELE") ) )
  {
    // we can have more than one delete command per call
    ThreadCurlPop3DeleteMessage( m_Pop3CommandData );   // -> wxMsThreadCurlPop3DeleteMsg.cpp
  }
  SetStatusText( wxEmptyString, 2);
}

// ------------------------------- eof ------------------------------
