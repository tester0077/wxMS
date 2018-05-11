/*-----------------------------------------------------------------
 * Name:        wxMsOptionsSelectMailClient.cpp
 * Purpose:     Allow user to select the e-mail client
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
#include "wxMsh.h"
#include "wxMsOptionsDialogh.h"
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
// ------------------------------------------------------------------/**
/* 
 * Handle the selection of an email client path.
 */
void wxMsOptionsDialog::OnPickEmailClient(wxCommandEvent& WXUNUSED( event) )
{
  wxString wsCaption = _("Choose the E-Mail client");
#if defined( _MSC_VER )
  wxString wsWildCard = _T("EXE files (*.exe)|*.exe|All files(*.*)|*.*");
#else
  wxString wsWildCard = _T("All files(*.*)|*.*");
#endif
  wxString wsDefaultDir;
  wxString wsDefaultFileName;
  wxFileName wfnEmailClient;

  if ( ! m_iniPrefs.data[IE_MAIL_CLIENT_PATH].dataCurrent.wsVal.IsEmpty() )
  {
    wfnEmailClient.Assign( wfnEmailClient );
    wsDefaultFileName = wfnEmailClient.GetFullName();
    wsDefaultDir = wfnEmailClient.GetFullPath();
  }
  wxString wsFileName;//( g_iniPrefs.data[IE_MAIL_CLIENT_PATH].dataCurrent.wsVal );
  wxFileDialog dlg( this, wsCaption, wsDefaultDir, wsDefaultFileName, wsWildCard, wxFD_OPEN );
  if ( dlg.ShowModal() == wxID_OK )
  {
    wsFileName = dlg.GetPath();
    m_textCtrlEmailClient->AppendText( wsFileName );
  }
}

// ------------------------------- eof ------------------------------
