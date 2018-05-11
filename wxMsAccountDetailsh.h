/*-----------------------------------------------------------------
 * Name:        wxMsAccountsDetailsh.h
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets license
 *-------------------------------------------------------------- */
#ifndef _WX_MS_ACCOUNTS_DETAILS_H
#define _WX_MS_ACCOUNTS_DETAILS_H

// ------------------------------------------------------------------
#include "wxMsBaseFrame.h"
#include "wxMsFrameh.h"
#include "wxMsConfh.h"
// ------------------------------------------------------------------
/**
 * Account info is stored in the master INI file
 */
class MyAccountsDetailsDialog : public MyBaseAccountsDetailsDialog
{
public:
  MyAccountsDetailsDialog( MyAccountsDialog* parent );
  bool TransferDataToWindow( void );
  bool TransferDataFromWindow( void );

  MyAccountsDialog* m_parent;
  MyMsAccountEl   * m_pCurrent;   // ptr to current account being edited/added
  iniPrefs_t        m_iniAcctDetailsPrefs;
  // for new accounts, we don't have an entry in the account list yet
  // so we need to save the server test results until we do
  wxString          m_wsTestRawCapaString;
  bool              m_bTestHasCapa;
  bool              m_bTestHasUidl;
  void OnMailServerTest(wxCommandEvent& event);
  void OnAdvancedPopConfig(wxCommandEvent& event);
};
#endif  // _WX_MS_ACCOUNTS_DETAILS_H
// ------------------------------- eof ---------------------------
