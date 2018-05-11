/*-----------------------------------------------------------------
 * Name:        wxMsAccountsDetails.cpp
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
#include "wxMsAccounth.h"
#include "wxMsAccountsDialogh.h"
#include "wxMsAccountDetailsh.h"
#include "wxMsPop3AdvancedOptionsDlgh.h"
#if defined( _MSC_VER )
#include "Urlmon.h"
#endif
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
// -------------------------------------------------------------
// Constructor
MyAccountsDetailsDialog::MyAccountsDetailsDialog( MyAccountsDialog* parent ) :
    MyBaseAccountsDetailsDialog( parent  )
{
  m_parent = parent;
  m_pCurrent = NULL;
  m_wsTestRawCapaString = wxEmptyString;
  m_bTestHasCapa = m_bTestHasUidl = false;
}

//------------------------------------------------------------------
// Called automagically by InitDialog() to set up the data
// prior to the dialog being shown
// Note since this overrides a predefined function, it MUST have
// the name and return type as given here
/**
  * For now set the first account data.
  * Down the road, we will have to save the last used account
  * and display its data
  */
bool MyAccountsDetailsDialog::TransferDataToWindow( void )
{
  int i;
  if ( m_parent->m_bEditingAccount )
  {
    MyAccountList::iterator iter;
    for ( i = 0, iter = wxGetApp().m_AccountList.begin();
      iter != wxGetApp().m_AccountList.end(); ++iter, i++ )
    {
      MyMsAccountEl *current = *iter;
      if( i == m_parent->m_iSelectedAccount )
      {
        m_pCurrent = current;
        m_textCtrlAccountName->SetValue( current->m_wsAcctName );
        m_textCtrlPop3ServerUrl->SetValue( current->m_wsPopServerUrl );
        m_textCtrlUserName->SetValue( current->m_wsUserName );
        m_textCtrlPassword->SetValue( current->m_wsPassword );
        m_checkBoxRememberPW->SetValue( current->m_bSavePassword );
      }
    }
  }
  else  // we must be adding a new account
  {
    m_textCtrlAccountName->Clear();
    m_textCtrlPop3ServerUrl->Clear();
    m_textCtrlUserName->Clear();
    m_textCtrlPassword->Clear();
    m_checkBoxRememberPW->SetValue( false );
  }
  m_notebookAcctDetails->SetSelection( 0 ); // select 'incoming' page
  return true;
}
//------------------------------------------------------------------
// Called automagically when dialog is dismissed via OK button
// Note since this overrides a predefined function, it MUST have
// the name and return type as given here

bool MyAccountsDetailsDialog::TransferDataFromWindow( void )
{
  // Note for the style chosen, this is always true - textctrl does not accept CR/LF etc
//  bool bIsSingleLine = m_textCtrlAccountName->IsSingleLine();
  wxString wsAcctName = m_textCtrlAccountName->GetLineText( 0 );
  if ( wsAcctName.IsEmpty() )
  {
    wxMessageBox( _T("The account name cannot be empty!"), "Notice", wxOK );
    m_textCtrlUserName->SetFocus();
    m_textCtrlPop3ServerUrl->SetFocus();
    return false;
  }
  wxString wsPopServerUrl = m_textCtrlPop3ServerUrl->GetLineText( 0 );
  if ( wsPopServerUrl.IsEmpty() )
  {
    wxMessageBox( _T("The server address cannot be empty!"), "Notice", wxOK );
    m_textCtrlPop3ServerUrl->SetFocus();
    return false;
  }
  wxString wsUserName = m_textCtrlUserName->GetLineText( 0 );
  if ( wsUserName.IsEmpty() )
  {
    wxMessageBox( _T("The user name cannot be empty!"), "Notice", wxOK );
    return false;
  }
  if ( m_parent->m_bEditingAccount )  // update the existing account
  {
    // since we saved the pointer to the item in the account list

    m_pCurrent->m_wsAcctName = wsAcctName;
    m_pCurrent->m_wsPopServerUrl =wsPopServerUrl;
    m_pCurrent->m_wsUserName = wsUserName;
    m_pCurrent->m_wsPassword = m_textCtrlPassword->GetLineText( 0 );
    if( m_checkBoxRememberPW->GetValue() )
    {
      m_pCurrent->m_bSavePassword = true;
    }
    else
    {
      m_pCurrent->m_bSavePassword = false;
    }
  }
  else    // must be adding a new account
  {
    // make sure we have no duplicate account names
    size_t i;
    wxString wsT;
    MyAccountList::iterator iter;
    for ( i = 0, iter = wxGetApp().m_AccountList.begin();
        iter != wxGetApp().m_AccountList.end(); ++iter, i++ )
    {
        MyMsAccountEl *current = *iter;
        if ( current->m_wsAcctName == wsAcctName )
        {
          wsT.Printf(
            _("Another account with the name %s already exists!\n")
            +_("Please chose a different name."),
            current->m_wsAcctName );
          wxMessageBox( wsT, "Notice", wxOK );
          m_textCtrlAccountName->SetFocus();
          return false;
        }
    }
    // we must have a new account - add its info to the array
    MyMsAccountEl *p = new MyMsAccountEl;
    p->m_bEnabled = true; // enable a new account by default
    p->m_wsPopServerUrl = m_textCtrlPop3ServerUrl->GetValue();
    // set the current default account type/kind
    p->m_wsAcctKind = _T("POP3");
    p->m_wsAcctName = m_textCtrlAccountName->GetValue();
    p->m_wsUserName = m_textCtrlUserName->GetValue();
    if( m_checkBoxRememberPW->GetValue() )
    {
      p->m_wsPassword = m_textCtrlPassword->GetValue();
      p->m_bSavePassword = m_checkBoxRememberPW->GetValue();
    }
    else
    {
      p->m_wsPassword = wxEmptyString;
      p->m_bSavePassword = false;
    }
    // add any results from the server test
    p->m_wsRawCapaString = m_wsTestRawCapaString;
    p->m_bHasCapa = m_bTestHasCapa;
    p->m_bHasUidl = m_bTestHasUidl;
    wxGetApp().m_AccountList.Append( p );
  }
  Fit();
  return true;
}

// ------------------------------------------------------------------

void MyAccountsDialog::OnAccountAdd( wxCommandEvent& event )
{
  m_bEditingAccount = false;
  int j = 0;
  MyAccountsDetailsDialog dlg( this  );
  wxGetApp().MakeConfigCopy( g_iniPrefs, dlg.m_iniAcctDetailsPrefs );
  // data setup and saving is handled by the two functions
  // MyAccountsDialog::TransferDataTo/FromWindow()
  // all we need to do is 'Show' it ...
  if ( dlg.ShowModal() == wxID_OK )
  {
    // all info MUST be saved here or else aborting the dialog
    // will have bad side effects as some of the INI data will/may
    // have been changed but not everything
    wxGetApp().MakeConfigCopy( dlg.m_iniAcctDetailsPrefs, g_iniPrefs );

    // also redraw the accounts grid completely
    m_gridMailAccounts->BeginBatch();
    m_gridMailAccounts->ClearGrid();    // clear the data
    int iNRows = m_gridMailAccounts->GetNumberRows();
    if ( iNRows )
      m_gridMailAccounts->DeleteRows( 0, iNRows );  // clear the grid itself
    MyAccountList::iterator iter;
    for ( j = 0, iter = wxGetApp().m_AccountList.begin();
      iter != wxGetApp().m_AccountList.end(); ++iter, j++)
    {
      MyMsAccountEl *current = *iter;
      //    ...process the current element...
      m_gridMailAccounts->InsertRows( j, 1 );
      m_gridMailAccounts->SetCellRenderer( j, 0, new wxGridCellBoolRenderer);
      m_gridMailAccounts->SetCellEditor(  j, 0, new wxGridCellBoolEditor);
      m_gridMailAccounts->SetCellAlignment( j, 0, wxALIGN_CENTRE, wxALIGN_CENTRE );
      m_gridMailAccounts->SetCellValue( j, 0, current->m_bEnabled ? _T("1") : _T("") );
      m_gridMailAccounts->SetCellValue( j, 1, current->m_wsAcctName );
      m_gridMailAccounts->SetCellValue( j, 2, current->m_wsPopServerUrl );
      m_gridMailAccounts->SetCellValue( j, 3, current->m_wsUserName );
      // password & rest are not displayed in this dialog
    }
    m_gridMailAccounts->EndBatch();
  }
#if defined _DEBUG
  int i = m_gridMailAccounts->GetNumberRows();
  i++;
  i--;  // keep GCC happy
#endif
  AccountGridSize();
  m_gridMailAccounts->SelectRow( j );
}

// ------------------------------------------------------------------
void MyAccountsDialog::OnAccountEdit( wxCommandEvent& event )
{
  m_bEditingAccount = true;
  wxArrayInt waiRows = m_gridMailAccounts->GetSelectedRows();
  if ( (waiRows.GetCount() > 1) || ( waiRows.GetCount() == 0 )  )
  {
    wxMessageBox( _("No account selected!"), _T(""), wxOK | wxICON_INFORMATION );
    return;
  }
  MyAccountsDetailsDialog dlg( this  );
  wxGetApp().MakeConfigCopy( g_iniPrefs, dlg.m_iniAcctDetailsPrefs );
  // data setup and saving is handled by the two functions
  // MyAccountsDialog::TransferDataTo/FromWindow()
  // all we need to do is 'Show' it ...
  if ( dlg.ShowModal() == wxID_OK )
  {
    // all info MUST be saved here or else aborting the dialog
    // will have bad side effects as some of the INI data will
    // have been changed but not everything
    wxGetApp().MakeConfigCopy( dlg.m_iniAcctDetailsPrefs, g_iniPrefs );
    int j;
    // display the information
    // see if we have a row with the same account already
    int iNRows = m_gridMailAccounts->GetNumberRows();
    // also redraw the accounts grid completely
    m_gridMailAccounts->BeginBatch();
    m_gridMailAccounts->ClearGrid();
    m_gridMailAccounts->DeleteRows( 0, iNRows );
    MyAccountList::iterator iter;
    for ( j = 0, iter = wxGetApp().m_AccountList.begin();
      iter != wxGetApp().m_AccountList.end(); ++iter, j++)
    {
      MyMsAccountEl *current = *iter;
      //    ...process the current element...
      m_gridMailAccounts->InsertRows( j, 1 );
      m_gridMailAccounts->SetCellRenderer( j, 0, new wxGridCellBoolRenderer);
      m_gridMailAccounts->SetCellEditor(  j, 0, new wxGridCellBoolEditor);
      m_gridMailAccounts->SetCellAlignment( j, 0, wxALIGN_CENTRE, wxALIGN_CENTRE );
      m_gridMailAccounts->SetCellValue( j, 0, current->m_bEnabled ? _T("1") : _T("") );
      m_gridMailAccounts->SetCellValue( j, 1, current->m_wsAcctName );
      m_gridMailAccounts->SetCellValue( j, 2, current->m_wsPopServerUrl );
      m_gridMailAccounts->SetCellValue( j, 3, current->m_wsUserName );
      // password & rest are not displayed in this dialog
    }
    m_gridMailAccounts->EndBatch();
  }
  AccountGridSize();
  m_gridMailAccounts->SelectRow( waiRows[0] );
}

// ------------------------------------------------------------------

void MyAccountsDialog::OnAccountDelete( wxCommandEvent& event )
{
  wxArrayInt waiRows = m_gridMailAccounts->GetSelectedRows();
  if ( (waiRows.GetCount() > 1) || ( waiRows.GetCount() == 0 )  )
    return;
  // remove the requested item
  int j;
  MyAccountList::iterator iter;
  for ( j = 0, iter = wxGetApp().m_AccountList.begin();
    iter != wxGetApp().m_AccountList.end(); ++iter, j++)
  {
    if( waiRows[0] == j )
    {
      wxGetApp().m_AccountList.remove( *iter );
      break;
    }
  }
  int iNRows = m_gridMailAccounts->GetNumberRows();
  // redraw the accounts grid completely
  m_gridMailAccounts->BeginBatch();
  m_gridMailAccounts->ClearGrid();    // clear the data
  m_gridMailAccounts->DeleteRows( 0, iNRows );  // clear the grid itself

  for ( j = 0, iter = wxGetApp().m_AccountList.begin();
    iter != wxGetApp().m_AccountList.end(); ++iter, j++)
  {
    MyMsAccountEl *current = *iter;
    //    ...process the current element...
    m_gridMailAccounts->InsertRows( j, 1 );
    m_gridMailAccounts->SetCellRenderer( j, 0, new wxGridCellBoolRenderer);
    m_gridMailAccounts->SetCellEditor(  j, 0, new wxGridCellBoolEditor);
    m_gridMailAccounts->SetCellAlignment( j, 0, wxALIGN_CENTRE, wxALIGN_CENTRE );
    m_gridMailAccounts->SetCellValue( j, 0, current->m_bEnabled ? _T("1") : _T("") );
    m_gridMailAccounts->SetCellValue( j, 1, current->m_wsAcctName );
    m_gridMailAccounts->SetCellValue( j, 2, current->m_wsPopServerUrl );
    m_gridMailAccounts->SetCellValue( j, 3, current->m_wsUserName );
    // password & rest are not displayed in this dialog
  }
  m_gridMailAccounts->EndBatch();
  m_gridMailAccounts->SelectRow( waiRows[0] );
}

// ------------------------------------------------------------------

void MyAccountsDetailsDialog::OnAdvancedPopConfig(wxCommandEvent& event)
{
  MyAdvancedPopConfigDlg dlg( this  );
  if ( dlg.ShowModal() == wxID_OK )
  {
    ;
  }
}

// ------------------------------- eof ------------------------------
