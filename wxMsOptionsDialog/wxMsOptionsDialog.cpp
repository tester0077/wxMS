/*-----------------------------------------------------------------
 * Name:        wxMsOptionsDialog.cpp
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
// -----------------------------------------------------------------
#include "wxMsh.h"
#include "wxMsOptionsDialogh.h"
#include "wxMsFilterDialogh.h"
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
// ------------------------------------------------------------------
// Note: Check box update code is in the file for each page

// Constructor
wxMsOptionsDialog::wxMsOptionsDialog( wxWindow* parent ) :
    MyDialogOptionsBase( parent  )
{
  m_pParent = parent;
}

// ------------------------------------------------------------------

bool wxMsOptionsDialog::TransferDataToWindow()
{
  wxString wsT;
  bool b;
  // 'General' tab

  // launch mail client after processing mail
  b = m_iniPrefs.data[IE_LAUNCH_MAIL_CLIENT].dataCurrent.bVal;
  m_checkBoxOptLaunchMailClient->SetValue( b );

  // e-mail client path
  m_textCtrlEmailClient->AppendText( 
    m_iniPrefs.data[IE_MAIL_CLIENT_PATH].dataCurrent.wsVal );

  // check for new mail at startup
  b = m_iniPrefs.data[IE_CHECK_MAIL_STARTUP].dataCurrent.bVal;
  m_checkBoxOptCheckMailAtStart->SetValue( b );

  // play sound when new mail arrives
  b = m_iniPrefs.data[IE_SOUND_4_NEW_MAIL].dataCurrent.bVal;
  m_checkBoxOptPlaySound->SetValue( b );

  // schedule mail check
  b = m_iniPrefs.data[IE_SCHEDULE_MAIL_CHECK].dataCurrent.bVal;
  m_checkBoxScheduleMailCheck->SetValue( b );

  // schedule mail check interval - minutes
  m_spinCtrlMailCheckInterval->SetValue(
    m_iniPrefs.data[IE_MAIL_CHECK_INTERVAL].dataCurrent.lVal );

  // schedule mail srerver connection check
  b = m_iniPrefs.data[IE_SCHEDULE_SERVER_CHECK].dataCurrent.bVal;
  m_checkBoxScheduleMailCheck->SetValue( b );

  // schedule mail server connection check interval - minutes
  m_spinCtrlMailServerConectCheckInterval->SetValue(
    m_iniPrefs.data[IE_SERVER_CHECK_INTERVAL].dataCurrent.lVal );

  // check for updates at startup
  m_checkBoxAutoUpdateCheck->SetValue(
    m_iniPrefs.data[IE_OPT_AUTO_UPDATE_CHECK].dataCurrent.bVal );

  // number of message lines to get with TOP request
  m_spinCtrlMaxTopLines->SetValue(
    m_iniPrefs.data[IE_OPT_MAX_TOP_LINES].dataCurrent.lVal );

  // ------------------------------------------------------------------
  // same for the 'Log' tab
  b =  m_iniPrefs.data[IE_LOG_FILE_WANTED].dataCurrent.bVal;
  m_cbOptLogToFile->SetValue( b );
  if ( b )
  {
    b = m_iniPrefs.data[IE_USE_LOG_DEF_DIR].dataCurrent.bVal;
    m_cbOptLogUseDefaultPath->SetValue( b );
    if ( b )
    {
       m_btnOptLogSelLogFilesDir->Enable( false );
       m_tcOptLogFilesDestDir->Disable();
    }
    else
    {
       m_btnOptLogSelLogFilesDir->Enable();
       m_tcOptLogFilesDestDir->Enable();
    }
  }
  else
  {
    m_cbOptLogUseDefaultPath->Disable();
    m_btnOptLogSelLogFilesDir->Enable( false );
    m_tcOptLogFilesDestDir->Disable();
  }
  // the status has been set, just show the values
  b = m_cbOptLogUseDefaultPath->GetValue();
  if( b )
  {
    m_tcOptLogFilesDestDir->SetValue( wxStandardPaths::Get().GetDataDir() );
  }
  else
  {
    m_tcOptLogFilesDestDir->SetValue(
      m_iniPrefs.data[IE_LOG_DIR_PATH].dataCurrent.wsVal );
  }
  m_tcOptLogFilesDestDir->SetValue(
    m_iniPrefs.data[IE_LOG_DIR_PATH].dataCurrent.wsVal );
  m_sliderOptLogVerbosity->SetValue(
    m_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal );
  // ------------------------------------------------------------------
  // and the Filter tab
  // list all of the current filters in m_checkListBoxFilter
  UpDateFilterList();
  // set the default status color
  m_colourPickerStatusDefColor->SetColour(
    m_iniPrefs.data[IE_STATUS_DEFAULT_COLOR].dataCurrent.wsVal );
  return true;
}

// ------------------------------------------------------------------
/**
 * Break out this code so we can update the list after modifying the
 * filter list because of either additions, deletion or name changes.
 */
void wxMsOptionsDialog::UpDateFilterList()
{
  wxArrayString wasChoices;
  for ( std::vector<MyFilterListEl>::iterator it = wxGetApp().m_FilterList.begin();
    it != wxGetApp().m_FilterList.end(); ++it )
  {
    // need to add them to the front to keep them in
    // the same sequence as they are in the list.
    wasChoices.Add( it->m_wsName );
  }
  // insert the filter names
  m_checkListBoxFilter->Clear();
  if( wasChoices.GetCount() ) // any contents??
  {
    m_checkListBoxFilter->InsertItems( wasChoices, 0);
    // set the checkboxes as needed
    int i = 0;
    for ( std::vector<MyFilterListEl>::iterator it = wxGetApp().m_FilterList.begin();
        it != wxGetApp().m_FilterList.end(); ++it, i++ )
    {
      m_checkListBoxFilter->Check( i, it->m_bState );
    }
    // just in case the filter file was fiddled with
    int iNFilters = std::min( (long)(wxGetApp().m_FilterList.size() - 1),
       m_iniPrefs.data[IE_FILTER_LAST_SEL].dataCurrent.lVal );
    m_checkListBoxFilter->SetSelection( iNFilters );
    ExplainFilter( iNFilters );
  }
}

// ------------------------------------------------------------------

bool wxMsOptionsDialog::TransferDataFromWindow()
{
  // 'General' tab
  // Launch mail client after processing mail
  m_iniPrefs.data[IE_LAUNCH_MAIL_CLIENT].dataCurrent.bVal =
    m_checkBoxOptLaunchMailClient->GetValue();
  // e-mail client path
  m_iniPrefs.data[IE_MAIL_CLIENT_PATH].dataCurrent.wsVal = 
    m_textCtrlEmailClient->GetValue();
  // check for new mail at startup
  m_iniPrefs.data[IE_CHECK_MAIL_STARTUP].dataCurrent.bVal
    = m_checkBoxOptCheckMailAtStart->GetValue();
  // play sound when new mail arrives
  m_iniPrefs.data[IE_SOUND_4_NEW_MAIL].dataCurrent.bVal =
    m_checkBoxOptPlaySound->GetValue();
  // schedule mail check
  m_iniPrefs.data[IE_SCHEDULE_MAIL_CHECK].dataCurrent.bVal =
    m_checkBoxScheduleMailCheck->GetValue();

  // schedule mail check interval - minutes
  m_iniPrefs.data[IE_MAIL_CHECK_INTERVAL].dataCurrent.lVal =
    m_spinCtrlMailCheckInterval->GetValue();

  // schedule mail srerver connection check
  m_iniPrefs.data[IE_SCHEDULE_SERVER_CHECK].dataCurrent.bVal =
    m_checkBoxScheduleMailCheck->GetValue();

  // schedule mail server connection check interval - minutes
  m_iniPrefs.data[IE_SERVER_CHECK_INTERVAL].dataCurrent.lVal =
    m_spinCtrlMailServerConectCheckInterval->GetValue();
  // schedule mail server connection check interval - minutes
   m_iniPrefs.data[IE_SERVER_CHECK_INTERVAL].dataCurrent.lVal =
     m_checkBoxScheduleConnectCheck->GetValue();

  // check for updates at startup
  m_iniPrefs.data[IE_OPT_AUTO_UPDATE_CHECK].dataCurrent.bVal =
    m_checkBoxAutoUpdateCheck->GetValue();
  // number of message lines to get with TOP request
  m_iniPrefs.data[IE_OPT_MAX_TOP_LINES].dataCurrent.lVal =
    m_spinCtrlMaxTopLines->GetValue();
  
  // -------------------------------------------------------
  // 'Log' tab
  m_iniPrefs.data[IE_LOG_FILE_WANTED].dataCurrent.bVal =
    m_cbOptLogToFile->GetValue();
  m_iniPrefs.data[IE_USE_LOG_DEF_DIR].dataCurrent.bVal =
    m_cbOptLogUseDefaultPath->GetValue();
  m_iniPrefs.data[IE_LOG_DIR_PATH].dataCurrent.wsVal
    = m_tcOptLogFilesDestDir->GetValue();
  m_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal =
    m_sliderOptLogVerbosity->GetValue();

  // update the filter enabled/disabled status
  int i = 0;
  for ( std::vector<MyFilterListEl>::iterator it = wxGetApp().m_FilterList.begin();
    it != wxGetApp().m_FilterList.end(); ++it, i++ )
  {
    it->m_bState = m_checkListBoxFilter->IsChecked( i );
  }
  m_iniPrefs.data[IE_FILTER_LAST_SEL].dataCurrent.lVal =
    m_checkListBoxFilter->GetSelection();

  // save the default status color
  m_iniPrefs.data[IE_STATUS_DEFAULT_COLOR].dataCurrent.wsVal =
    m_colourPickerStatusDefColor->GetColour().GetAsString();
  return true;
}

// ------------------------------- eof ------------------------------
