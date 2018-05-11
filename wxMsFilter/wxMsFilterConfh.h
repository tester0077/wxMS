//////////////////////////////////////////////////////////////////
// Name:        wxMSOptionsFilterConfh.h
// Purpose:     Config header for wxMS Mail Screener filter data
// Author:      A. Wiegert
// Modified by:
// Created:
// Licence:     wxWidgets license
//////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !defined( __WX_MS_OPT_FILTER_CONF_H )
#define  __WX_MS_OPT_FILTER_CONF_H
#if 0
// Pref version
#define CURRENT_PREF_VERSION  3

enum entryType 
{ eBool, eLong, eString, eFloat };

typedef struct  
{
   bool  bVal;
   long  lVal;
   // index in corresponding string array if a string
   long  indexVal;   
   wxString wsVal;
   float fVal;
} iniData;


typedef struct entry_
{
   wxString  wsPathStr;
   wxString  wsKeyStr;
   entryType eType;
   iniData  dataDefault;
   iniData  dataCurrent;
} entry;
#endif
// ------------------------------------------------------------------ 
/* Note: these entries define the sequence in iniPrefs
 */
enum iniFilterEnts
{
  // Note the number of entries here must match the 
  // number of initializers 
  IE_FILTER_STATE = 0,        // enabled/disabled
  IE_FILTER_NAME,             // filter name
  IE_FILTER_STATUS_STRING,    // string to show in status field
  IE_FILTER_COLOR,            // color for status field string
  IE_FILTER_AND_OR,           // use AND or OR to connect rule parts
  IE_FILTER_ACTION,           // resulting action: mark for: legitimate, delete, bounce, ...
  IE_FILTER_RULE_SOURCE,      // source parts to search: from, subject, message body, ....
  IE_FILTER_RULE_CRITERIA,    // contains | !contains | is equal to | contains regex | ! contains regex
  IE_FILTER_RULE_TEXT,        // text or regex string to look for

  IE_MAX_FILTER            // keep as last - determines size of struct iniPrefs
};
typedef struct _iniFilterPrefs_t
{
  long      lPrefVer;   // magic number
  entry     data[IE_MAX_FILTER];
} iniFilterPrefs_t; 

#endif   //  __WX_MS_OPT_FILTER_CONF_H
// ---------------------- eof ---------------------------------------
