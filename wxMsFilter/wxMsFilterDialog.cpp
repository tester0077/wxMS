/*-----------------------------------------------------------------
 * Name:        wxMsDialogFilter.cpp
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
#include "wxMsh.h"
#include "wxMsOptionsDialog/wxMsOptionsDialogh.h"
#include "wxMsFilterDialogh.h"
#include <wx/tokenzr.h>
#include <wx/colour.h>
#include <algorithm>
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

void wxMsOptionsDialog::OnFilterAdd(wxCommandEvent& WXUNUSED(event) )
{
  int iRet = 0;
  wxMsDialogFilterEdit dlg( this );
  dlg.m_bEditing = false;
  dlg.m_wsSelection = m_checkListBoxFilter->GetStringSelection();

  // set the title to 'Add Filter'
  dlg.SetLabel( _("Add Filter" ) );
  iRet = dlg.ShowEditFilterDialog();
  if ( iRet == wxID_OK )
  {
    // all info MUST be saved here or else aborting the dialog
    // will have bad side effects as some of the INI data will
    // have been changed but not everything

    // all data is saved in TransferFromWindow()
    UpDateFilterList();

    // select the just added filter
    m_checkListBoxFilter->SetSelection( wxGetApp().m_FilterList.size() - 1 );
  }
  dlg.SaveSize();
}

// ------------------------------------------------------------------

void wxMsOptionsDialog::OnFilterEdit(wxCommandEvent& WXUNUSED(event) )
{
  int iRet;
  wxMsDialogFilterEdit dlg( this );
  dlg.m_bEditing = true;
  dlg.m_wsSelection = m_checkListBoxFilter->GetStringSelection();

  iRet = dlg.ShowEditFilterDialog();
  if ( iRet == wxID_OK )
  {
    // all info MUST be saved here or else aborting the dialog
    // will have bad side effects as some of the INI data will
    // have been changed but not everything

    // all data is saved in TransferFromWindow()
    UpDateFilterList();
    int iIndex = m_checkListBoxFilter->FindString( dlg.m_wsSelection );
    m_checkListBoxFilter->SetSelection( iIndex );
  }
  dlg.SaveSize();
}

// ------------------------------------------------------------------

void wxMsOptionsDialog::OnFilterDelete(wxCommandEvent& WXUNUSED(event) )
{
  int iSel = m_checkListBoxFilter->GetSelection();
  int i = 0;
  for ( std::vector<MyFilterListEl>::iterator it = wxGetApp().m_FilterList.begin();
      it != wxGetApp().m_FilterList.end(); ++it, i++ )
  {
    if( i == iSel )
    {
      int iRes;
      wxString wsT;
      wsT.Printf( _("Delete filter %s"), it->m_wsName );
      iRes = wxMessageBox( wsT, _("Confirm"), wxYES_NO );
      if( iRes != wxYES )
        return;
      else
      {
        wxGetApp().m_FilterList.erase( it );
        break;
      }
    }
  }
  UpDateFilterList();
}

// ------------------------------------------------------------------
/**
 * Move the selected filter one level up in priority.
 */
void wxMsOptionsDialog::OnFilterUp(wxCommandEvent& event)
{
  unsigned int uiSel = m_checkListBoxFilter->GetSelection();
  if ( uiSel > 0 )
  {
    std::swap( wxGetApp().m_FilterList[uiSel], wxGetApp().m_FilterList[uiSel-1] );
    UpDateFilterList();
    m_checkListBoxFilter->SetSelection( uiSel-1 );
  }
}

// ------------------------------------------------------------------
/**
 * Move the selected filter one level down in priority.
 */
void wxMsOptionsDialog::OnFilterDown(wxCommandEvent& event)
{
  unsigned int uiSel = m_checkListBoxFilter->GetSelection();
  if ( uiSel <  (wxGetApp().m_FilterList.size() - 1 ) )
  {
    std::swap( wxGetApp().m_FilterList[uiSel], wxGetApp().m_FilterList[uiSel+1] );
    UpDateFilterList();
    m_checkListBoxFilter->SetSelection( uiSel+1 );
  }
}

// ------------------------------------------------------------------
/**
 * Handler to explain the filter action in a text message.
 */
void wxMsOptionsDialog::OnFilterListSelectionChanging(wxCommandEvent& event)
{
  unsigned int uiSel = m_checkListBoxFilter->GetSelection();
  ExplainFilter( uiSel );
}

// ------------------------------------------------------------------
/**
 * Make up the text string to explain the filter action.
 */
void wxMsOptionsDialog::ExplainFilter( unsigned int a_uiIndex )
{
  unsigned int i = 0;
  for ( std::vector<MyFilterListEl>::iterator it = wxGetApp().m_FilterList.begin();
      it != wxGetApp().m_FilterList.end(); ++it, i++ )
  {
    if( i == a_uiIndex )
    {
      wxString wsT;
      wsT.Printf( _("Explain filter %s - to be done yet"), it->m_wsName );
      m_textCtrlFilterDescription->Clear();
      m_textCtrlFilterDescription->AppendText( wsT );
    }
  }
}

// ==================================================================
/**
 * Filter editor ctor.
 */
wxMsDialogFilterEdit::wxMsDialogFilterEdit(wxWindow* parent) :
    MyDialogFilterEditBase( parent  )
{
  m_pParent = parent;
  m_bEditing = false; // assume adding a new filter
}

// ------------------------------------------------------------------

int wxMsDialogFilterEdit::ShowEditFilterDialog()
{
  m_wsDlgAppName = wxGetApp().GetAppName();
  wxGetApp().MakeConfigCopy( ((wxMsOptionsDialog *)m_parent)->m_iniPrefs, m_iniPrefs );
  SetSize( wxDefaultCoord, wxDefaultCoord,
    m_iniPrefs.data[IE_FILTER_DLG_W].dataCurrent.lVal,
    m_iniPrefs.data[IE_FILTER_DLG_H].dataCurrent.lVal );
  return ShowModal();
}

// ------------------------------------------------------------------
void wxMsDialogFilterEdit::SaveSize()
{
  // save the dialog size
  int w, h;
  GetSize( &w, &h );
  m_iniPrefs.data[IE_FILTER_DLG_W].dataCurrent.lVal = w;
  m_iniPrefs.data[IE_FILTER_DLG_H].dataCurrent.lVal = h;
  wxGetApp().MakeConfigCopy( m_iniPrefs, ((wxMsOptionsDialog *)m_parent)->m_iniPrefs );
}
// ------------------------------------------------------------------
bool wxMsDialogFilterEdit::TransferDataToWindow()
{
  int iIndex;
  // enable those for which code is implemented
  // currently only 'legit & delete
  m_checkBoxActionBlacklist->Enable(
    g_iniPrefs.data[IE_FILTER_IMPL_BLACKLIST].dataCurrent.bVal );
  m_checkBoxActionMarkDel->Enable(
    g_iniPrefs.data[IE_FILTER_IMPL_DELETE].dataCurrent.bVal );
  m_checkBoxActionMarkBounce->Enable(
    g_iniPrefs.data[IE_FILTER_IMPL_BOUNCE].dataCurrent.bVal );
  // show the default rules choices
  m_choiceRuleSourceString->SetSelection( 0 );
  m_choiceRuleCriteria->SetSelection( 0 );
  if ( m_bEditing )
  {
    // get  the current data
#if defined( _DEBUG )
    int n = wxGetApp().m_FilterList.size();
#endif
    for ( std::vector<MyFilterListEl>::iterator it = wxGetApp().m_FilterList.begin();
      it != wxGetApp().m_FilterList.end(); it++ )
    {
      if( it->m_wsName.IsSameAs( m_wsSelection ) )
      {
        m_textCtrlFilterEdit_FilterName->SetDefaultStyle(wxTextAttr(wxColour(it->m_wsColorHex )));
        m_textCtrlFilterEdit_FilterName->Clear();
        m_textCtrlFilterEdit_FilterName->AppendText( it->m_wsName );
        m_textCtrlFilterEdit_FilterStatus->SetDefaultStyle(wxTextAttr(wxColour(it->m_wsColorHex )));
        m_textCtrlFilterEdit_FilterStatus->Clear();
        m_textCtrlFilterEdit_FilterStatus->AppendText( it->m_wsStatus );
        m_radioBoxFilterAndOr->SetSelection( it->m_bAndOr ? 0 : 1);
        m_colourPickerFilterColor->SetColour( wxColour(it->m_wsColorHex ) );
        if( !it->m_wsRuleSource.IsEmpty() )
        {
          iIndex = m_choiceRuleSourceString->FindString( it->m_wsRuleSource );
          wxASSERT( iIndex != wxNOT_FOUND );
        }
        else
          iIndex = 0;
        m_choiceRuleSourceString->SetSelection( iIndex );

        if( !it->m_wsRuleCriteria.IsEmpty() )
        {
          iIndex = m_choiceRuleCriteria->FindString( it->m_wsRuleCriteria );
          wxASSERT( iIndex != wxNOT_FOUND );
        }
        else
          iIndex = 0;
        m_choiceRuleCriteria->SetSelection( iIndex );
        m_textCtrlRuleMsgText->Clear();
        m_textCtrlRuleMsgText->AppendText( it->m_wsRuleText );
        // now sort out the action
        // first set all check boxes as they should be
        // first turn all off,
        m_checkBoxActionLegit->SetValue( false );
        m_checkBoxActionBlacklist->SetValue( false );
        m_checkBoxActionMarkDel->SetValue( false );
        m_checkBoxActionMarkBounce->SetValue( false );

        // then set them as needed
        wxStringTokenizer tokenizer( it->m_wsAction, ",");
        while ( tokenizer.HasMoreTokens() )
        {
          wxString wsToken = tokenizer.GetNextToken();
          if ( wsToken.IsSameAs( _T("legit") ) )
          {
            m_checkBoxActionLegit->SetValue( true );
            // ignore the other check boxes - disable them
            // it should not be possible to have any other checked
            m_checkBoxActionBlacklist->Enable( false );
            m_checkBoxActionMarkDel->Enable( false );
            m_checkBoxActionMarkBounce->Enable( false );
            continue;
          }
          if ( wsToken.IsSameAs( _T("blackList") ) )
          {
            m_checkBoxActionBlacklist->SetValue( true );
            continue;
          }
          if ( wsToken.IsSameAs( _T("delete") ) )
          {
            m_checkBoxActionMarkDel->SetValue( true );
            continue;
          }
          if ( wsToken.IsSameAs( _T("bounce") ) )
          {
            m_checkBoxActionMarkBounce->SetValue( true );
            continue;
          }
        }
      }
    }
  }
  return true;
}

// ------------------------------------------------------------------

bool wxMsDialogFilterEdit::TransferDataFromWindow()
{
  // make sure we have all required data
  wxString wsName = m_textCtrlFilterEdit_FilterName->GetLineText( 0 );
  if ( wsName.IsEmpty() )
  {
    wxMessageBox( _("The filter name cannot be blank!"), _("Error"),
      wxOK | wxICON_ERROR );
    m_textCtrlFilterEdit_FilterName->SetFocus();
    return false;
  }
  wxString wsStatus = m_textCtrlFilterEdit_FilterStatus->GetLineText( 0 );
  if ( wsStatus.IsEmpty() )
  {
    wxMessageBox( _("The filter status cannot be blank!"), _("Error"),
      wxOK | wxICON_ERROR );
    m_textCtrlFilterEdit_FilterStatus->SetFocus();
    return false;
  }
  wxString wsAction = wxEmptyString;
  if ( m_checkBoxActionLegit->IsChecked() )
    wsAction = _T("legit,");
  if ( m_checkBoxActionBlacklist->IsChecked() )
    wsAction += _T("blackList,");
  if ( m_checkBoxActionMarkDel->IsChecked() )
    wsAction += _T("delete,");
  if ( m_checkBoxActionMarkBounce->IsChecked() )
    wsAction += _T("bounce,");
  if ( wsAction.IsEmpty() )
  {
    wxMessageBox( _("You must select at least one action!"), _("Error"),
      wxOK | wxICON_ERROR );
    return false;
  }
  if ( m_textCtrlRuleMsgText->GetLineText( 0 ).IsEmpty() )
  {
    wxMessageBox( _("You must provide some rule text!"), _("Error"),
      wxOK | wxICON_ERROR );
    m_textCtrlRuleMsgText->SetFocus();
    return false;
  }
  if ( m_bEditing )   // editing an existing filter
  {
    for ( std::vector<MyFilterListEl>::iterator it = wxGetApp().m_FilterList.begin();
        it != wxGetApp().m_FilterList.end(); it++ )
    {
      // find the list entry corresponding to it
      if( it->m_wsName.IsSameAs( m_wsSelection ) )
      {
        it->m_wsName = m_textCtrlFilterEdit_FilterName->GetLineText( 0 );
        it->m_wsStatus = m_textCtrlFilterEdit_FilterStatus->GetLineText( 0 );
        it->m_bAndOr = (m_radioBoxFilterAndOr->GetSelection() == 0 );
        int iSel = m_choiceRuleSourceString->GetSelection();
        it->m_wsRuleSource = m_choiceRuleSourceString->GetString( iSel );
        iSel = m_choiceRuleCriteria->GetSelection();
        it->m_wsRuleCriteria = m_choiceRuleCriteria->GetString( iSel );
        it->m_wsRuleText = m_textCtrlRuleMsgText->GetLineText( 0 );
        wxColour wcFilter = m_colourPickerFilterColor->GetColour();
        // use rgb(r,g,b) syntax
        it->m_wsColorHex = wcFilter.GetAsString( wxC2S_CSS_SYNTAX );
        // now sort out the action
        it->m_wsAction = wxEmptyString;
        if ( m_checkBoxActionLegit->IsChecked() )
          it->m_wsAction = _T("legit,");
        if ( m_checkBoxActionBlacklist->IsChecked() )
          it->m_wsAction += _T("blackList,");
        if ( m_checkBoxActionMarkDel->IsChecked() )
          it->m_wsAction += _T("delete,");
        if ( m_checkBoxActionMarkBounce->IsChecked() )
          it->m_wsAction += _T("bounce,");

        return true; // we're done - look no further
      }
    }
  }
  else// adding a new filter
  {
    for ( std::vector<MyFilterListEl>::iterator it = wxGetApp().m_FilterList.begin();
        it != wxGetApp().m_FilterList.end(); it++ )
    {
      if( it->m_wsName.IsSameAs( wsName ) )
      {
        wxString wsT;
        wsT.Printf(
          _("The filter '%s' already exist\nPlease chose a different name!"),
          m_wsSelection );
        wxMessageBox( wsT, _("Error"),
          wxOK | wxICON_ERROR );
        m_textCtrlFilterEdit_FilterName->SetFocus();
        return false;
      }
    }
    MyFilterListEl *pCurrent = new MyFilterListEl();
    pCurrent->m_wsName = m_textCtrlFilterEdit_FilterName->GetLineText( 0 );
    pCurrent->m_wsStatus = m_textCtrlFilterEdit_FilterStatus->GetLineText( 0 );
    pCurrent->m_bAndOr = (m_radioBoxFilterAndOr->GetSelection( ) == 0 );
    int iSel = m_choiceRuleSourceString->GetSelection();
    pCurrent->m_wsRuleSource = m_choiceRuleSourceString->GetString( iSel );
    iSel = m_choiceRuleCriteria->GetSelection();
    pCurrent->m_wsRuleCriteria = m_choiceRuleCriteria->GetString( iSel );
    pCurrent->m_wsRuleText = m_textCtrlRuleMsgText->GetLineText( 0 );
    pCurrent->m_wsColorHex = m_wsStatusColor;
    // now sort out the action
    pCurrent->m_wsAction = wxEmptyString;
    if ( m_checkBoxActionLegit->IsChecked() )
      pCurrent->m_wsAction = _T("legit,");
    if ( m_checkBoxActionBlacklist->IsChecked() )
      pCurrent->m_wsAction += _T("blackList,");
    if ( m_checkBoxActionMarkDel->IsChecked() )
      pCurrent->m_wsAction += _T("delete,");
    if ( m_checkBoxActionMarkBounce->IsChecked() )
      pCurrent->m_wsAction += _T("bounce,");
    pCurrent->m_bState = true;       // enable the filter by default
    wxGetApp().m_FilterList.push_back( *pCurrent );
    delete pCurrent;
  }
  return true;
}

// ------------------------------------------------------------------
void wxMsDialogFilterEdit::OnColourChanged(wxColourPickerEvent& event)
{
  m_wsStatusColor = event.GetColour().GetAsString();
  event.Skip();
}

// ------------------------------------------------------------------
void wxMsDialogFilterEdit::OnFilterEditIsLegitimate(wxCommandEvent& event)
{
  // enable those for which code is implemented
  // currently only 'legit & delete
  m_checkBoxActionBlacklist->Enable(
    g_iniPrefs.data[IE_FILTER_IMPL_BLACKLIST].dataCurrent.bVal &&
    !event.IsChecked() );
  m_checkBoxActionMarkDel->Enable(
    g_iniPrefs.data[IE_FILTER_IMPL_DELETE].dataCurrent.bVal &&
    !event.IsChecked() );
  m_checkBoxActionMarkBounce->Enable(
    g_iniPrefs.data[IE_FILTER_IMPL_BOUNCE].dataCurrent.bVal &&
    !event.IsChecked() );
  event.Skip();
}

// ------------------------------- eof ------------------------------
