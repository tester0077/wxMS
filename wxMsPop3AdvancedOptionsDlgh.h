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

};

#endif  // _WX_MS_POP3_ADVANCED_OPTIONS_H
// ------------------------------- eof ---------------------------

