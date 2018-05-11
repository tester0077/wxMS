/*-----------------------------------------------------------------
 * Name:        wxMsOnAbout.cpp
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
// this block needs to AFTER all headers
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif

// ------------------------------------------------------------------

void MyFrame::OnOptionsLogClear( wxCommandEvent& WXUNUSED(event) )
{
#if defined( WANT_LOG_WIN )
  m_textCtrlLog->Clear();
#endif
}

// ------------------------------------------------------------------
// Note the check mark is added/removed automagically
// see wxOrdo for more details

void MyFrame::OnOptionsLogShow( wxCommandEvent&  event )
{
  int iSashPos;
  wxWindow *pWin1;

  bool bShow = m_menuItemLogShow->IsChecked();
  // is log window wanted?
  g_iniPrefs.data[IE_LOG_WIN_WANTED].dataCurrent.bVal = bShow;
  pWin1 = m_splitterMain->GetWindow1();

  if( !bShow )
  {
    m_pLogWindowSave = m_splitterMain->GetWindow2();
    iSashPos = m_splitterMain->GetSashPosition();
    g_iniPrefs.data[IE_SASH_POS].dataCurrent.lVal = iSashPos;
    m_splitterMain->Unsplit();
  }
  else
  {
    m_splitterMain->SplitHorizontally( pWin1, m_pLogWindowSave,
      g_iniPrefs.data[IE_SASH_POS].dataCurrent.lVal );
  }
  wxGetApp().m_frame->Layout();
}

// ------------------------------------------------------------------
#if 0   // not used - yet
void MyFrame::OnMenuItemDeleteLogFiles( wxCommandEvent& WXUNUSED( event ) ) 
{ 
//  bool bRet;
  wxString wsCurLog = wxGetApp().m_wsCurLogFileName;
  if( ::wxFileExists( wsCurLog ) )
  {
    // stop passing messages to file
    wxGetApp().m_pLogChain->PassMessages( false );
    // confirm the 'delete'
    int  iResult = wxMessageBox( 
      _("Do you really want to delete ") + wsCurLog + _("?"),
      _("Confirm"), wxYES_NO | wxICON_EXCLAMATION );
    if( iResult == wxYES )
    {
      // simply close it. 
      // Don't reopen it here; wait for user to ask for it.
      wxGetApp().m_pAppLog->CloseLog();
    }
    // this is needed to pass messages to log window
    wxGetApp().m_pLogChain->PassMessages( true );
  }
  // file does not exist
  else
  {
    wxMessageBox( _("File: ") + wsCurLog + _(" does not exist!"),
      wxEmptyString, wxOK | wxICON_INFORMATION );
  }
}

#include <shlwapi.h>
// ------------------------------------------------------------------
/**
 * Open the current log file in the default editor - read-only
 */
void MyFrame::OnMenuItemOpenLogFiles(wxCommandEvent& WXUNUSED(event))
{
  wxFileName wfnLogFile( wxGetApp().m_wsCurLogFileName );
  if ( !wfnLogFile.FileExists() )
  {
    wxMessageBox( _("Log file: ") + wxGetApp().m_wsCurLogFileName + _(" does not exist!") +
      _("\nYou can enable logging in \"Options->Log\""),
      wxEmptyString, wxOK | wxICON_INFORMATION );
  }

  wxString wsOpen = _T("open");
  ShellExecute(
    NULL,
    wsOpen.wchar_str(),
    wxGetApp().m_wsCurLogFileName.c_str(),
    NULL,
    NULL,
    SW_SHOWNORMAL
  );
  wxLogError( _("Log file open %s: \n"), wxGetApp().m_wsCurLogFileName );
}
#endif
// ------------------------------------------------------------------
bool MyApp::CreateNewLog()
{
#if defined( WANT_LOG_LIMIT )
  wxString wsLog2BigMsg = _("Old log too large; started new log");
  wxFileName wfnLogFile( m_wsCurLogFileName );
  // make the limit just less them the attachment limit EMAIL_ATTACHMENT_LIMIT
  if ( wfnLogFile.Exists() && (wfnLogFile.GetSize() > EMAIL_ATTACHMENT_LIMIT - 10 * 1024 ) )
  {
    ::wxRemoveFile( m_wsCurLogFileName ); // remove file and start over
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
      wxLogInfo( wsLog2BigMsg );
  }
#endif
  m_pAppLog = new MyFileLog( m_wsCurLogFileName );

  if ( m_pAppLog->IsLogFileOK() )
  {
    m_pLogChain = new wxLogChain(m_pAppLog);
  }
  else
  {
    wxString wsMsg;
    wsMsg.Printf(
      _("Could not initialize the application log (file \"%s\"). \n\nContinue anyway?"),
      m_pAppLog->GetLogFileName());
    delete m_pAppLog;
    if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    {
      wxLogInfo( wsMsg );
    }
    if ( wxMessageBox( wsMsg, _("Error"), wxYES_NO | wxICON_ERROR, NULL) != wxYES )
    {
      return false;
    }
  }
  return true;
}

// ------------------------------- eof ------------------------------
