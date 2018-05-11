/*-----------------------------------------------------------------
 * Name:        wxMsListsh.h
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWindows licence
 *-------------------------------------------------------------- */
#ifndef _WX_MS_MSG_LIST_H
#define _WX_MS_MSG_LIST_H

// ------------------------------------------------------------------
#include <wx/list.h>
// -------------------------------------------------------------
/**
 * list of messages on the server
 */
class MyMsgListElement
{
public:
  unsigned int        m_uiNmbr;
  unsigned int        m_uiLen;    // in octets
  wxString            m_wsMsg;
  wxString            m_wsUID;
  MyMsgListElement( unsigned int uiNo, unsigned int uiLen,
    wxString wsUID, wxString wsMsg ) {
      m_uiNmbr = uiNo;
      m_uiLen = uiLen;
      m_wsUID = wsUID;
      m_wsMsg = wsMsg; };
};

WX_DECLARE_LIST(MyMsgListElement, MyMsgsList );
#include <wx/listimpl.cpp>

#endif  // _WX_MS_MSG_LIST_H
// ------------------------------- eof ---------------------------
