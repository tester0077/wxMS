/*-----------------------------------------------------------------
 * Name:        wxMsAccountsDialog.cpp
 * Purpose:     Account info is stored in the master INI file
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
#include "wxMsAccountsDialogh.h"
#include <wx/tokenzr.h>
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
BEGIN_EVENT_TABLE(MyAccountsDialog, wxDialog)
  EVT_BUTTON( wxID_BUTTON_ACCOUNT_ADD, MyAccountsDialog::OnAccountAdd )
END_EVENT_TABLE()

// ------------------------------------------------------------------
/**
 * If The Account selection has changed, we need to clear the current
 * message list
 */
void MyFrame::OnMenuAccounts( wxCommandEvent& event )
{
  MyAccountsDialog dlg( this );
  dlg.SetSize( wxDefaultCoord, wxDefaultCoord,
    g_iniPrefs.data[IE_ACCOUNT_DLG_W].dataCurrent.lVal,
    g_iniPrefs.data[IE_ACCOUNT_DLG_H].dataCurrent.lVal );
  wxGetApp().MakeConfigCopy( g_iniPrefs, dlg.m_iniDialogPrefs );
  // data setup and saving is handled by the two functions
  // MyAccountsDialog::TransferDataTo/FromWindow()
  // all we need to do is 'Show' it ...
  if ( dlg.ShowModal() == wxID_OK )
  {
    // all info MUST be saved here or else aborting the dialog
    // will have bad side effects as some of the INI data will
    // have been changed but not everything
    wxGetApp().MakeConfigCopy( dlg.m_iniDialogPrefs, g_iniPrefs );
    Check4NewMail();
  }
  // save the dialog size
  int w, h;
  dlg.GetSize( &w, &h );
  g_iniPrefs.data[IE_ACCOUNT_DLG_W].dataCurrent.lVal = w;
  g_iniPrefs.data[IE_ACCOUNT_DLG_H].dataCurrent.lVal = h;
  CheckConnectivity();
}

// -------------------------------------------------------------
// Constructor
MyAccountsDialog::MyAccountsDialog( wxWindow* parent ) :
    MyBaseAccountsDialog( parent  )
{
  m_bEditingAccount = false;
  m_iSelectedAccount = 0;
  InitAccountsGrid();
}

// ------------------------------------------------------------------
void MyAccountsDialog::InitAccountsGrid()
{
  // set up Accounts data grid
  m_gridMailAccounts->ClearGrid();
  m_gridMailAccounts->InsertCols( 0, 4 );
  m_gridMailAccounts->SetRowLabelSize( 1 );
  m_gridMailAccounts->EnableGridLines( true );
  m_gridMailAccounts->SetColLabelValue( 0, _T("Enabled") );
  m_gridMailAccounts->SetColLabelValue( 1, _T("Account") );
  m_gridMailAccounts->SetColLabelValue( 2, _T("Server") );
  m_gridMailAccounts->SetColLabelValue( 3, _T("User") );
  m_gridMailAccounts->SetSelectionMode( wxGrid::wxGridSelectRows );
}

//------------------------------------------------------------------
// Called automagically by InitDialog() to set up the data
// prior to the dialog being shown
// Note since this overrides a predefined function, it MUST have
// the name and return type as given here
bool MyAccountsDialog::TransferDataToWindow( void )
{
  int j = 0;
  int iAcct2Select = 0; // first enabled account to be selected
  m_gridMailAccounts->BeginBatch();
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
    bool bEnabled = current->m_bEnabled;
    if( bEnabled )
    {
      if( iAcct2Select == 0 )
        iAcct2Select = j;
      m_gridMailAccounts->SetCellValue( j, 0, _T("1") );
    }
    else
    {
      m_gridMailAccounts->SetCellValue( j, 0, _T("") );
    }
    m_gridMailAccounts->SetCellValue( j, 1, current->m_wsAcctName );
    m_gridMailAccounts->SetCellValue( j, 2, current->m_wsPopServerUrl );
    m_gridMailAccounts->SetCellValue( j, 3, current->m_wsUserName );
    // password & rest are not displayed in this dialog
  }
  m_gridMailAccounts->SelectRow( iAcct2Select );
  m_iSelectedAccount = iAcct2Select;
  // set the save column widths
  int i = 0;
  wxString wsColWidths =
    m_iniDialogPrefs.data[IE_ACCOUNT_GRID_COL_WIDTHS].dataCurrent.wsVal;
  long lWidth;
  int iCols = m_gridMailAccounts->GetNumberCols();
  wxStringTokenizer tokenizer(
    g_iniPrefs.data[IE_ACCOUNT_GRID_COL_WIDTHS].dataCurrent.wsVal, "|");
  while ( tokenizer.HasMoreTokens() )
  {
    wxString token = tokenizer.GetNextToken();
    // process token here
    token.ToLong( &lWidth ) ;
    m_gridMailAccounts->GetColSize( i++ );
    wxASSERT( i <= iCols );
    wxUnusedVar( iCols ); // keep linux compiler happy in release mode
  }
  m_gridMailAccounts->EndBatch();
  AccountGridSize();
  return true;
}

//------------------------------------------------------------------
// Called automagically when dialog is dismissed via OK button
// Note since this overrides a predefined function, it MUST have
// the name and return type as given here
/**
 * All  work done in the 'Add/edit/delete' handlers
 */
bool MyAccountsDialog::TransferDataFromWindow( void )
{
  // collect the grid column sizes so they can be restored
  // next time
  m_gridMailAccounts->BeginBatch();
  int i;
  wxString wsColWidths;
  wxString wsT;
  int iCols = m_gridMailAccounts->GetNumberCols();
  for ( i = 0; i < iCols; i++ )
  {
    int iSize = m_gridMailAccounts->GetColSize( i );
    wsT.Printf( _T("%d|"), iSize );
    wsColWidths += wsT;
  }
  m_iniDialogPrefs.data[IE_ACCOUNT_GRID_COL_WIDTHS].dataCurrent.wsVal =
    wsColWidths;
  m_gridMailAccounts->EndBatch();
  return true;
}

// ------------------------------------------------------------------
void MyAccountsDialog::OnAccountSelect(wxGridEvent& event )
{
  wxString wsValue;
  int iCol = event.GetCol();
  int iRow = event.GetRow();
  m_iSelectedAccount = iRow;
#ifdef _DEBUG
  wxLogMessage( _T("Accounts: left click: row: %d, col: %d"), iRow, iCol );
#endif
  m_gridMailAccounts->SelectRow( iRow );
  bool bEnabled = false;
  if( (iCol == 0) )
  {
#ifdef _DEBUG
    wxLogMessage( _T(" show editor: row: %d, col: %d"), iRow, iCol );
#endif
    m_gridMailAccounts->EnableCellEditControl();
    m_gridMailAccounts->ShowCellEditControl();
    wsValue = m_gridMailAccounts->GetCellValue( iRow, iCol );
    if( wsValue.IsSameAs( _T("1") ) )
    {
      m_gridMailAccounts->SetCellValue( iRow, iCol, _T("") );
    }
    else
    {
      m_gridMailAccounts->SetCellValue( iRow, iCol, _T("1") );
      bEnabled = true;
    }
    m_gridMailAccounts->DisableCellEditControl();
    // also need to update the appropriate account in the list
    int i;
    MyAccountList::iterator iter;
    for ( i = 0, iter = wxGetApp().m_AccountList.begin();
      iter != wxGetApp().m_AccountList.end(); ++iter, i++ )
    {
      MyMsAccountEl *current = *iter;
      if( i == m_iSelectedAccount )
      {
        wxASSERT( iRow == i );
        current->m_bEnabled = bEnabled;
        break;
      }
    }
  }
}

// ------------------------------------------------------------------
/** This function is intended to resize the grid when the size of the
 * main frame or any of its components is changed.
 * Also after any of the editable data fields have been changed.
 * It was originally based on hgrid.h by Yuri Borsky, but I had to
 * make significant changes for this code.
 */
void MyAccountsDialog::OnAccountGridSize(wxSizeEvent& event)
{
  AccountGridSize();
  event.Skip();
}

// ------------------------------------------------------------------
void MyAccountsDialog::AccountGridSize()
{
//  int iFixedWidth = 0;
  int iFullWidth = 0, h;
  unsigned int uiCols = m_gridMailAccounts->GetNumberCols();
//  iCols = iCols;  // keep compiler quiet
  wxWindow *pWinParent = m_gridMailAccounts->GetGridWindow();
  pWinParent->GetClientSize( &iFullWidth, &h );
  // this is needed during startup, while the grid is being constructed
  if ( iFullWidth < 10 )
    return;
  m_gridMailAccounts->BeginBatch();
  size_t i = 0;
  // get total assigned width
  long lTotalWidth = 0;

  for ( i = 0; i < uiCols; i++ )
  {
    if( i <  uiCols - 1 )
    {
      lTotalWidth += m_gridMailAccounts->GetColSize( i );
    }
    else
      m_gridMailAccounts->SetColSize( i, std::max( 0, iFullWidth - (int)lTotalWidth ) );
  }
  m_gridMailAccounts->EndBatch();
}

// ------------------------------- eof ------------------------------
