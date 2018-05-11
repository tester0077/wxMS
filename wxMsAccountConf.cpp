/*-----------------------------------------------------------------
 * Name:        wxMsAccountConf.cpp
 * Purpose:     Handle saving & restoring account info to .INI file
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
#include "wxMsConfh.h"
#include "wxMsAccounth.h"
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
// ==================================================================
/* This MUST be called on main frame dtor - else the variables
 * holding the data will not be valid any longer
 * Note: LastSubject is saved in the main frame dtor
 * Also important: MUST save all members of the account structure
 * and use the approriate defaults until they are used fully
 m_wsAcctName(wxEmptyString),
    m_wsUserName(wxEmptyString),
    m_wsAcctKind(wxEmptyString),
    m_wsAcctEmailAddr(wxEmptyString),
    m_wsPopServerUrl(wxEmptyString),
    m_wsPassword(wxEmptyString),
    m_wsRawCapabilities(wxEmptyString),
    m_ulPopPort(110),
    m_bEnabled(false),
    m_bAllowBounce(false),
    m_bPopUseSsl(false),
    m_bSavePassword(false),
    m_bHasCapaFunction(false),
    m_bHasUidl(false),
    m_wsSmtpServerUrl(wxEmptyString),
    m_wsSmtpPassword(wxEmptyString),
    m_ulSmtpPort(0)
 */
void MyApp::SaveAccounts( void )
{
  wxArrayString asGroups;
//  size_t        iCount;
  unsigned int  i = 0;
  wxString      wsKey;
  wxString      wsData;

  wxString wsOldPath = m_pConfig->GetPath();
  // delete the existing 'Account' group
  m_pConfig->DeleteGroup( _("/Account") );
  MyAccountList::iterator iter;
  for ( i = 0, iter = wxGetApp().m_AccountList.begin();
    iter != wxGetApp().m_AccountList.end(); ++iter, i++ )
  {
    MyMsAccountEl *current = *iter;
    //    ...process the current element...
    wsKey.Printf( _("/Account/%d/Name"), i );
    m_pConfig->Write( wsKey, current->m_wsAcctName );

    wsKey.Printf( _("/Account/%d/User"), i );
    m_pConfig->Write( wsKey, current->m_wsUserName );

    wsKey.Printf( _("/Account/%d/AccountKind"), i );    // POP3, IMAP, GMAIL, Yahoo, ...
    m_pConfig->Write( wsKey, current->m_wsAcctKind );

    wsKey.Printf( _("/Account/%d/Pop3ServerUrl"), i );
    m_pConfig->Write( wsKey, current->m_wsPopServerUrl );

    wsKey.Printf( _("/Account/%d/AcctRemPW"), i );
    m_pConfig->Write( wsKey, current->m_bSavePassword ? _T("yes") : _T("no") );

    wsKey.Printf( _("/Account/%d/Password"), i );
    if ( current->m_bSavePassword )
      m_pConfig->Write( wsKey, current->m_wsPassword );
    else
      m_pConfig->Write( wsKey, wxEmptyString );

    wsKey.Printf( _("/Account/%d/Pop3Port"), i );
    m_pConfig->Write( wsKey, current->m_ulPopPort );

    wsKey.Printf( _("/Account/%d/Enabled"), i );
    m_pConfig->Write( wsKey, current->m_bEnabled ? _T("yes") : _T("no") );

    wsKey.Printf( _("/Account/%d/AllowBounce"), i );
    m_pConfig->Write( wsKey, current->m_bAllowBounce ? _T("yes") : _T("no") );

    wsKey.Printf( _("/Account/%d/HasCAPA"), i );
    m_pConfig->Write( wsKey, current->m_bHasCapa ? _T("yes") : _T("no") );

    wsKey.Printf( _("/Account/%d/RawCapaString"), i );
    m_pConfig->Write( wsKey, current->m_wsRawCapaString );

    wsKey.Printf( _("/Account/%d/HasUidl"), i );
    m_pConfig->Write( wsKey, current->m_bHasUidl ? _T("yes") : _T("no") );

    wsKey.Printf( _("/Account/%d/UsePop3SSL"), i );
    m_pConfig->Write( wsKey, current->m_bPopUseSsl ? _T("yes") : _T("no") );

    wsKey.Printf( _("/Account/%d/SmtpServerUrl"), i );
    m_pConfig->Write( wsKey, current->m_wsSmtpServerUrl );

    wsKey.Printf( _("/Account/%d/SmtpServerPassword"), i );
    m_pConfig->Write( wsKey, current->m_wsSmtpPassword );

    wsKey.Printf( _("/Account/%d/SmtpServerPort"), i );
    m_pConfig->Write( wsKey, current->m_ulSmtpPort );
    delete current;
  }
  m_pConfig->SetPath( wsOldPath );
  m_pConfig->Flush();
}

// ------------------------------------------------------------------
void MyApp::RestoreAccounts( void )
{
  // NB: in this program, the config object is already created at this moment
  // because we had called Get() from MyApp::OnInit(). However, if you later
  // change the code and don't create it before this line, it won't break
  // anything - unlike if you manually create wxConfig object with Create()
  // or in any other way (then you must be sure to create it before using it!).

  wxString wsOldPath = m_pConfig->GetPath();
  wxString wsAccount( _("/Account") );
  long lDummy;
  long lLong;   // used for ports
  bool bCont;
  bool bOK;
  wxUnusedVar( bOK ); // keep linux compiler happy in release mode
  unsigned int  i = 0;
  wxString wsStr;
  wxString wsKey;
  wxString wsData;
  wxString wsT;
  wxArrayString asGroups;

  // look for Group 'Account' and then get all members
  m_pConfig->SetPath( wsAccount );
  bCont = m_pConfig->GetFirstGroup( wsStr, lDummy );
  while(  bCont )
  {
    MyMsAccountEl *p = new MyMsAccountEl;
    wsKey.Printf( _("/Account/%d/Name"), i );
    bOK = m_pConfig->Read( wsKey, &wsData, _("") );
    p->m_wsAcctName = wsData;
    wxASSERT_MSG( bOK, wsData );
    wsKey.Printf( _("/Account/%d/User"), i );
    bOK = m_pConfig->Read( wsKey, &wsData, _("") );
    p->m_wsUserName = wsData;
    wxASSERT_MSG( bOK, wsData );
    wsKey.Printf( _("/Account/%d/AccountKind"), i );    // POP3, IMAP, GMAIL, Yahoo, ...
    bOK = m_pConfig->Read( wsKey, &wsData, _("POP3") ); // default to POP3
    p->m_wsAcctKind = wsData;
    wxASSERT_MSG( bOK, wsData );
    wsKey.Printf( _("/Account/%d/Pop3ServerUrl"), i );
    bOK = m_pConfig->Read( wsKey, &wsData, _("") );
    p->m_wsPopServerUrl = wsData;
    wxASSERT_MSG( bOK, wsData );
    wsKey.Printf( _("/Account/%d/AcctRemPW"), i );
    bOK = m_pConfig->Read( wsKey, &wsData, _("") );
    wxASSERT_MSG( bOK, wsData );
    if( wsData.IsSameAs( _T("yes") ) )
    {
      wsKey.Printf( _("/Account/%d/Password"), i );
      bOK = m_pConfig->Read( wsKey, &wsData, _("") );
      p->m_wsPassword = wsData;
      p->m_bSavePassword = true;
    }
    else
    {
      p->m_wsPassword = wxEmptyString;
      p->m_bSavePassword = false;
    }

    wsKey.Printf( _("/Account/%d/Pop3Port"), i );
    bOK = m_pConfig->Read( wsKey, &lLong, 110l );    // default port is 110
    p->m_ulPopPort = lLong;
    wsT.Printf( _T("%ld"), lLong );
    wxASSERT_MSG( bOK, wsT );
    wsKey.Printf( _("/Account/%d/Enabled"), i );
    bOK = m_pConfig->Read( wsKey, &wsData, _("yes") );
    p->m_bEnabled = wsData.IsSameAs( _T("yes") );
    wxASSERT_MSG( bOK, wsData );
    wsKey.Printf( _("/Account/%d/AllowBounce"), i );
    bOK = m_pConfig->Read( wsKey, &wsData, _("no") );
    p->m_bAllowBounce = wsData.IsSameAs( _T("yes") );
    wxASSERT_MSG( bOK, wsData );
    wsKey.Printf( _("/Account/%d/HasCAPA"), i );
    bOK = m_pConfig->Read( wsKey, &wsData, _("no") );
    p->m_bHasCapa = wsData.IsSameAs( _T("yes") );
    wxASSERT_MSG( bOK, wsData );
    wsKey.Printf( _("/Account/%d/RawCapaString"), i );
    bOK = m_pConfig->Read( wsKey, &wsData, _("") );
    p->m_wsRawCapaString = wsData;
    wxASSERT_MSG( bOK, wsData );
    wsKey.Printf( _("/Account/%d/HasUidl"), i );
    bOK = m_pConfig->Read( wsKey, &wsData, _("no") );
    p->m_bHasUidl = wsData.IsSameAs( _T("yes") );
    wxASSERT_MSG( bOK, wsData );
    wsKey.Printf( _("/Account/%d/UsePop3SSL"), i );
    bOK = m_pConfig->Read( wsKey, &wsData, _("no") );
    p->m_bPopUseSsl = wsData.IsSameAs( _T("yes") );
    wxASSERT_MSG( bOK, wsData );
    wsKey.Printf( _("/Account/%d/SmtpServerUrl"), i );
    bOK = m_pConfig->Read( wsKey, &wsData, _("") );
    p->m_wsSmtpServerUrl = wsData;
    wxASSERT_MSG( bOK, wsData );
    wsKey.Printf( _("/Account/%d/SmtpServerPassword"), i );
    bOK = m_pConfig->Read( wsKey, &wsData, _("") );
    p->m_wsSmtpPassword = wsData;
    wxASSERT_MSG( bOK, wsData );
    wsKey.Printf( _("/Account/%d/SmtpServerPort"), i );
    bOK = m_pConfig->Read( wsKey, &lLong, 25l );    // default port is 25 for SMTP
    p->m_ulPopPort = lLong;
    wsT.Printf( _T("%ld"), lLong );
    wxASSERT_MSG( bOK, wsT );
    m_AccountList.Append( p );
    i++;
    bCont = m_pConfig->GetNextGroup(wsStr, lDummy );
  }
  m_pConfig->SetPath( wsOldPath );
}

// ------------------------------- eof ------------------------------
