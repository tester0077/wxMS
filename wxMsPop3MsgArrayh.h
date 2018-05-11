/*-----------------------------------------------------------------
 * Name:        wxMsPop3MsgArrayh.h
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets license
 *-------------------------------------------------------------- */
#ifndef _WX_MS_POP3_ARRAY_H
#define _WX_MS_POP3_ARRAY_H

// ------------------------------------------------------------------

#pragma once
#include "wx/wx.h"
#include "wx/wxemail.h"
// ------------------------------------------------------------------
#if !defined( WANT_MSG_LIST )
class MyPop3MsgElement   // Pop3 Msgs
{
public:
  long              m_lSequence;
  long              m_lSize;
  wxString          m_wsUidl;
  wxString          m_wsHeader;
  wxString          m_wsMessage;
  wxEmailMessage*   m_pEmailMessage;
  bool              m_bHaveMessage; ///< true if we have all of the message in memory
  bool              m_bNeedHeader;  ///< we are in neewd of the header information

  MyPop3MsgElement(
    long  lSequence,
    long  lSize )
  {                 // initialize all members
    m_lSequence     = lSequence;
    m_lSize         = lSize;
    m_wsUidl        = wxEmptyString;
    m_wsHeader      = wxEmptyString;
    m_wsMessage     = wxEmptyString;
    m_pEmailMessage = NULL;
    m_bHaveMessage  = false;
    m_bNeedHeader   = true;
  };
};

WX_DECLARE_OBJARRAY( MyPop3MsgElement, MyPop3MsgArray );
#endif
#endif  // _WX_MS_POP3_ARRAY_H
// ------------------------------- eof ---------------------------
