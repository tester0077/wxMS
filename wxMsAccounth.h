/*-----------------------------------------------------------------
 * Name:        wxMsAccounth.h
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets license
 *-------------------------------------------------------------- */
#ifndef _WX_MS_ACCOUNT_H
#define _WX_MS_ACCOUNT_H
#include "wxMsh.h"
#include <wx/dynarray.h>
// ------------------------------------------------------------------
/**
 * Account info is stored in the master INI file
 *
 * See the Mailwasher registry entry at
 * HKeu_CURRENT_USERSoftware/eCOSN/Mailwasher/prefs/account/free
 */
class MyMsAccountEl {
public:
  wxString        m_wsAcctName;       ///< Account Name
  wxString        m_wsUserName;       ///< user name for this acct
  wxString        m_wsAcctKind;       ///< PPOP3, IMAP, GMAIL, Yahoo, ....
  wxString        m_wsPopServerUrl;   ///< POP3 server URL
  bool            m_bSavePassword;
  wxString        m_wsPassword;
  unsigned long   m_ulPopPort;        ///< port to use for POP3
  bool            m_bEnabled;         ///< allows accounts to be active or dormant
  bool            m_bServerAvailable; ///< is server available?
  bool            m_bAllowBounce;     ///< can mail be bounced?
  bool            m_bHasCapa;         ///< true if the server supports the CAPA command
  wxString        m_wsRawCapaString;  ///< capabilities as returned from CAPA command
  wxArrayString   m_wasUidl;          ///< string array of UIDLs
  bool            m_bHasUidl;         ///< true if server has UIDL feature
  bool            m_bPopUseSsl;
                                      ///< details needed for bouncing
  wxString        m_wsSmtpServerUrl;  ///<   "
  wxString        m_wsAcctEmailAddr;  ///< sender mail box address for bounce
  wxString        m_wsSmtpPassword;   ///<   "
  unsigned long   m_ulSmtpPort;       ///<   "
  MyMsAccountEl() : // default ctor for the struct
    m_wsAcctName(wxEmptyString),
    m_wsUserName(wxEmptyString),
    m_wsAcctKind(wxEmptyString),
    m_wsPopServerUrl(wxEmptyString),
    m_bSavePassword(false),
    m_wsPassword(wxEmptyString),
    m_ulPopPort(110),
    m_bEnabled(false),
    m_bServerAvailable(false),
    m_bAllowBounce(false),
    m_bHasCapa(false),
    m_wsRawCapaString(wxEmptyString),
    m_bHasUidl(false),
    m_bPopUseSsl(false),

    m_wsSmtpServerUrl(wxEmptyString),
    m_wsAcctEmailAddr(wxEmptyString),
    m_wsSmtpPassword(wxEmptyString),
    m_ulSmtpPort(0)
   {
   };
};

WX_DECLARE_LIST( MyMsAccountEl, MyAccountList );
#endif  // _WX_MS_ACCOUNT_H
// ------------------------------- eof ---------------------------
