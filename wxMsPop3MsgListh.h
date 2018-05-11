/*-----------------------------------------------------------------
 * Name:        wxMsPop3MsgListh.h
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets license
 *-------------------------------------------------------------- */
#ifndef _WX_MS_POP3_LIST_H
#define _WX_MS_POP3_LIST_H
// ------------------------------------------------------------------
#include "wx/wx.h"
#include <list>
// ------------------------------------------------------------------

class MyPop3MsgElement   // Pop3 Msgs
{
public:
  long              m_lSize;
  wxString          m_wsUidl;
  wxString          m_wsFrom;
  wxString          m_wsTo;
  wxString          m_wsSubject;
  wxString          m_wsDateSent;
  wxString          m_wsPriority;
  wxString          m_wsAttachments;
  wxString          m_wsAcctName;
  wxString          m_wsHeader;
  wxString          m_wsMessage;
  wxString          m_wsContentType;
  wxString          m_wsContentSubType;
  bool              m_bHaveMessage; ///< true if we have all of the message in memory
  bool              m_bNeedHeader;  ///< we are in neewd of the header information
  bool              m_bDelete;      ///< state of 'delete' checkbox
  bool              m_bBounce;      ///< state of 'bounce' checkbox
  bool              m_bBlacklist;   ///< state of 'blacklist' checkbox

  MyPop3MsgElement(
    wxString  wsUidl,
    long      lSize )
  {                 // initialize all members
    m_lSize         = lSize;
    m_wsUidl        = wsUidl;
    m_wsFrom = m_wsTo = m_wsSubject = m_wsDateSent = m_wsAttachments = wxEmptyString;
    m_wsAcctName    = m_wsHeader = m_wsMessage =
      m_wsContentType = m_wsContentSubType = wxEmptyString;
    m_bHaveMessage  = false;
    m_bNeedHeader   = true;
    m_bDelete = m_bBounce = m_bBlacklist = false;
  };
};

#endif  // _WX_MS_POP3_LIST_H
// ------------------------------- eof ---------------------------
