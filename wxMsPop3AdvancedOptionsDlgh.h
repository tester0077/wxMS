/*-----------------------------------------------------------------
 * Name:        wxMsPop3AdvancedOptionsDlgh.h
 * Purpose:     dialog to allow user to set the advanced options for the 
 *              POP3 server
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets licence
 *-------------------------------------------------------------- */
#ifndef _WX_MS_POP3_ADVANCED_OPTIONS_H
#define _WX_MS_POP3_ADVANCED_OPTIONS_H

// ------------------------------------------------------------------
#include "wxMsBaseFrame.h"
#include "wxMsFrameh.h"
#include "wxMsConfh.h"
#include "wxMsAccountsDialogh.h"
#include "wxMsAccountDetailsh.h"
// ------------------------------------------------------------------
/**
 * Advanced info is stored in the master INI file
 */
class MyAdvancedPopConfigDlg : public MyBaseAdvancedPopConfigDlg
{
public:
  MyAdvancedPopConfigDlg( MyAccountsDetailsDialog* parent );
  bool TransferDataToWindow( void );
  bool TransferDataFromWindow( void );

  MyAccountsDetailsDialog* m_parent;
//  MyMsAccountEl   * m_pCurrent;   // ptr to current account being edited/added
//  iniPrefs_t        m_iniAcctDetailsPrefs;
  // for new accounts, we don't have an entry in the account list yet
  // so we need to save the server test results until we do
//  wxString          m_wsTestRawCapaString;
//  bool              m_bTestHasCapa;
//  bool              m_bTestHasUidl;
//  void OnMailServerTest(wxCommandEvent& event);
};

#endif  // _WX_MS_POP3_ADVANCED_OPTIONS_H
// ------------------------------- eof ---------------------------

