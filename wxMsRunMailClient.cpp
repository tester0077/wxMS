/*-----------------------------------------------------------------
 * Name:        wxMsRunMailClient.cpp
 * Purpose:     code to invoke the external mail client - eg: Thunderbird
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
void MyFrame::OnLaunchMailClient(wxCommandEvent& event)
{
  event.Skip();

  if ( !g_iniPrefs.data[IE_MAIL_CLIENT_PATH].dataCurrent.wsVal.IsEmpty() )
  {
    InvokeMailClient( g_iniPrefs.data[IE_MAIL_CLIENT_PATH].dataCurrent.wsVal );
  }
  else
  {
    wxString wsT;
    wsT.Printf( _("No E-Mail client defined!\n") );
    wxMessageBox( wsT, "Error", wxOK );
    return;
  }
}

// ------------------------------------------------------------------
bool MyFrame::InvokeMailClient( wxString wsClientPath )
{
  wxString    wsT;
  wxTextAttr  wtaOld;

  wsT =  _("Invoking mail client ")  + wsClientPath;
  wxLogMessage( wsT );

  wxString wsCmd;
  // adding -mail does not show the GUI??, does TB run??
  wsCmd = wsClientPath;//  + _T(" -mail");

  if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 0 )
  {
    wxLogMessage( _T("wsCmd: ") + wsCmd );
  }
  wxExecute( wsCmd );
  return true;
}


// ------------------------------- eof ------------------------------
