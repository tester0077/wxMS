/*-----------------------------------------------------------------
 * Name:        wxMsFilterListh.h
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets licence
 *-------------------------------------------------------------- */
#ifndef _WX_MS_FILTER_LIST_H
#define _WX_MS_FILTER_LIST_H

// ------------------------------------------------------------------

#pragma once
#include "wx/wx.h"
#include <list>
// ------------------------------------------------------------------

class MyFilterListEl   // Pop3 Msgs
{
public:
  bool            m_bState;           //  IE_FILTER_STATE             // enabled/disabled
  wxString        m_wsName;           //  IE_FILTER_NAME,             // filter name
  wxString        m_wsStatus;         //  IE_FILTER_STATUS_STRING,    // string to show in status field
  wxString        m_wsColorHex;       //  IE_FILTER_COLOR,            // color for status field string - Hex #rrggbb format
  bool            m_bAndOr;           //  IE_FILTER_AND_OR,           // use AND or OR to connect rule parts
  wxString        m_wsAction;         //  IE_FILTER_ACTION,           // resulting action: mark for: legitimate, delete, bounce, ...
  
  wxString        m_wsRuleSource;     //  IE_FILTER_RULE_NAME,        // source parts to search: from, subject, message body, ....
  wxString        m_wsRuleCriteria;   //  IE_FILTER_RULE_CRITERIA,    // contains | !contains | is equal to | contains regex | ! contains regex
  wxString        m_wsRuleText;       //  IE_FILTER_RULE_TEXT,        // text or regex string to look for

  MyFilterListEl(
    bool bState, wxString wsName = wxEmptyString, wxString wsStatus = wxEmptyString, wxString wsHexColor = _T("#000000"),
    bool bAndOr = true, wxString wsAction = wxEmptyString, wxString wsRuleSource = wxEmptyString, 
    wxString wsRuleCriteria = wxEmptyString, wxString wsRuleText = wxEmptyString ) 
  {                 // initialize all members
    m_bState          = bState;
    m_wsName          = wsName;
    m_wsStatus        = wsStatus;
    m_wsColorHex      = wsHexColor; 
    m_bAndOr          = bAndOr;
    m_wsAction        = wsAction;
    m_wsRuleSource    = wsRuleSource;
    m_wsRuleCriteria  = wsRuleCriteria;
    m_wsRuleText      = wsRuleText;
  };

  MyFilterListEl() 
  {
    ;
  };
};

#endif  // _WX_MS_FILTER_LIST_H
// ------------------------------- eof ---------------------------
