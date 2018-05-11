/*-----------------------------------------------------------------
 * Name:        wxMsCurlPop3GetMessages.cpp
 * Purpose:     Get messages from POP3 servers
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
#include "wxMsAccountsDialogh.h"
#include "wxMsAccountDetailsh.h"
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
/**
 * Get new messages from the specific POP3 account.
 * Use the contents of the UIDL array of strings to get the
 * necesary information for any new messages from the POP3
 * server accounts
 */
void MyFrame::FrameCurlPop3GetNewMessages( Pop3CommandData & ar_Pop3CommandData)
{
  ar_Pop3CommandData.sm_wsResultData.Clear();
  ar_Pop3CommandData.sm_wasCommands.Clear();
  ar_Pop3CommandData.sm_wasCommands.Add( _T("NEW") );   // use the pseudo command NEW to

  bool bLocalRun = true;
  wxASSERT( m_Pop3CommandData.sm_wasCommands.GetCount() );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsAccountName.IsEmpty(), m_Pop3CommandData.sm_wsAccountName );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsPop3ServerUrl.IsEmpty(), m_Pop3CommandData.sm_wsPop3ServerUrl );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsUserName.IsEmpty(), m_Pop3CommandData.sm_wsUserName );
  wxASSERT_MSG( !m_Pop3CommandData.sm_wsUserPassword.IsEmpty(), m_Pop3CommandData.sm_wsUserPassword );
  RunLibcurlThread();         // handle the rest via threads

  while ( bLocalRun )
  {
    {
      wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
      bLocalRun = m_bRunning;
    }
#if defined( _MSC_VER )
    ::wxYield();  // needed for Windows - else it hangs in this loop
#endif
    ::wxMilliSleep( 10 );
  }
}

// ------------------------------- eof ------------------------------
