/*-----------------------------------------------------------------
 * Name:        wxMsOptions.cpp
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
#include <wx/dir.h>
#include <wx/treebase.h>
#if WANT_XPM
#include "..\wxMain\rc\platform.xpm"
#endif
#include "wxMsh.h"
#include "wxMsFrameh.h"
#include "wxMsOptionsDialogh.h"
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
// -------------------------------------------------------------

/**
 * Called when user selects the Options->Setup item on the main menu
 * or toolbar.
 * Allows the user to specify new options or modify existing ones.
 * The changes in the dialog are all saved in a local copy  of the preferences
 * and saved to disk only if the user dismissed the dialog by clicking OK.
 * This allows the user to change his mind by dismissing the dialog by
 * clicking 'Cancel' to retain the old data.
 *
 * \param [in] - command event
 * \return     - nothing direct; all info is stored in the local
 *               temporary .INI structure - which is copied to the
 *               global .INI structure only if and when the user clicks 'OK".
 *               The global .INI data is saved on exit.
 */
void MyFrame::OnOptionsSetupDlg(wxCommandEvent& WXUNUSED( event ))
{
  g_iniPrefs.data[IE_LAST_OPTION_TAB].dataCurrent.lVal =
    ShowOptions( g_iniPrefs.data[IE_LAST_OPTION_TAB].dataCurrent.lVal );
  DisplayMailList();
}

// ------------------------------------------------------------------
/**
 * Display the options dialog and move to selected notebook page;
 * return the page selected on exit.
 */
long MyFrame::ShowOptions( long lPage )
{
  wxMsOptionsDialog dlg( this );

  std::vector<MyFilterListEl> filterList;
  filterList = wxGetApp().m_FilterList;

  dlg.m_wsDlgAppName = wxGetApp().GetAppName();
  wxGetApp().MakeConfigCopy( g_iniPrefs, dlg.m_iniPrefs );
  dlg.GetNotebookOptions()->SetSelection( lPage );
  dlg.SetSize( wxDefaultCoord, wxDefaultCoord,
    g_iniPrefs.data[IE_OPT_DLG_W].dataCurrent.lVal,
    g_iniPrefs.data[IE_OPT_DLG_H].dataCurrent.lVal );
  if ( dlg.ShowModal() == wxID_OK )
  {
    // all info MUST be saved here or else aborting the dialog
    // will have bad side effects as some of the INI data will
    // have been changed but not everything
    wxGetApp().MakeConfigCopy( dlg.m_iniPrefs, g_iniPrefs );
    /* Need to also verify that we have a writable file path & name
     * for log and that the log file is available for writing.
     * I don't yet understand the ins and outs of this business,
     * so I will leave it at this for now.
     */
#if defined( WANT_LOG_CHAIN )
    // see if a log file is wanted - we can use the global .INI here
    if ( g_iniPrefs.data[IE_LOG_FILE_WANTED].dataCurrent.bVal )
    {
      wxASSERT( !g_iniPrefs.data[IE_LOG_DIR_PATH].dataCurrent.wsVal.IsEmpty() );
    }
    // make sure the log chain has been set up
    if ( wxGetApp().m_pLogChain )
    {
      wxGetApp().m_pLogChain->PassMessages(
        g_iniPrefs.data[IE_LOG_FILE_WANTED].dataCurrent.bVal );
    }
#endif
    m_lNTopLines = g_iniPrefs.data[IE_OPT_MAX_TOP_LINES].dataCurrent.lVal;
    // start the mail check timer if user wants it so
    if ( g_iniPrefs.data[IE_SCHEDULE_MAIL_CHECK].dataCurrent.bVal )
    {
      GetMailTimerPtr()->Start( 60 * 1000 ); 
    }
  }
  else
  {
    // restore the original filter list
    wxGetApp().m_FilterList = filterList;
  }
  // save the dialog size
  int w, h;
  dlg.GetSize( &w, &h );
  g_iniPrefs.data[IE_OPT_DLG_W].dataCurrent.lVal = w;
  g_iniPrefs.data[IE_OPT_DLG_H].dataCurrent.lVal = h;
  return dlg.GetNotebookOptions()->GetSelection();
}

// ------------------------------------------------------------------
void MyFrame::OnMenuFilters(wxCommandEvent& WXUNUSED(event))
{
  ShowOptions( 2 );
  DisplayMailList();
}

// ------------------------------- eof ------------------------------
