/*-----------------------------------------------------------------
 * Name:        wxMsAccountsDialogh.h
 * Purpose:     Account info is stored in the master INI file
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets license
 *-------------------------------------------------------------- */
#ifndef _WX_MS_ACCOUNTS_DIALOG_H
#define _WX_MS_ACCOUNTS_DIALOG_H

// ------------------------------------------------------------------
#include "wxMsBaseFrame.h"
#include "wxMsFrameh.h"

// ------------------------------------------------------------------
extern iniPrefs_t g_iniPrefs;
// ------------------------------------------------------------------
class MyAccountsDialog : public MyBaseAccountsDialog
{
public:
  MyAccountsDialog( wxWindow* parent );
  bool TransferDataToWindow( void );
  bool TransferDataFromWindow( void );
  void OnAccountAdd( wxCommandEvent& event );
  void OnAccountEdit( wxCommandEvent& event );
  void OnAccountDelete( wxCommandEvent& event );
  void OnAccountSelect( wxGridEvent& event);
  void InitAccountsGrid();
  void OnAccountGridSize(wxSizeEvent& event);
  void AccountGridSize();

  bool            m_bEditingAccount;
  iniPrefs_t      m_iniDialogPrefs; // Account info is stored in the master INI file
  int             m_iSelectedAccount;

  DECLARE_EVENT_TABLE()
};
#endif  // _WX_MS_ACCOUNTS_DIALOG_H

// ------------------------------- eof ---------------------------
