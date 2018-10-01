//////////////////////////////////////////////////////////////////
// Name:        wxMSConfh.h
// Purpose:     Config header for wxMS Mail Screener
// Author:      A. Wiegert
// Licence:     wxWidgetss license
//////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !defined( __WX_MS_CONF_H )
#define __WX_MS_CONF_H

// Pref version
#define CURRENT_PREF_VERSION  3

enum entryType
{ eBool, eLong, eString, eFloat };

typedef struct
{
   bool  bVal;
   long  lVal;
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

// ------------------------------------------------------------------
/* Note: these entries define the sequence in iniPrefs
 */
enum iniEnts
{
  // Note the number of entries here must match the
  // number of initializers
  IE_LOG_WIN_WANTED = 0,
  IE_SASH_POS,
  IE_FRAME_X,
  IE_FRAME_Y,
  IE_FRAME_W,
  IE_FRAME_H,
  IE_OPT_DLG_H,                     // option dialog height
  IE_OPT_DLG_W,                     // option dialog width
#if defined( WANT_SCALING )
  IE_RESOLUTION_X,                  // last saved screen resolution X
  IE_RESOLUTION_Y,                  // last saved screen resolution Y
#endif
  IE_OPT_LAST_CONF_FILE,            // last configuration file loaded

  IE_MAIL_GRID_COL_WIDTHS,          // mail grid col widths w1|w2|w3.. |wn
  IE_MAIL_GRID_COL_LABELS,          // mail grid col Labels l1|l2|l3.. |ln
  IE_MAX_TASKS,                     // Maximum number of concurrent tasks allowed.

  IE_LOG_FILE_WANTED,               // write log to a file
  IE_LOG_VERBOSITY,                 // level of log detail
  IE_LOG_FREQUENCY,                 // enum to start new log: ignore - do nothing, every job or every day
  IE_LOG_DIR_PATH,                  // path to log file directory
  IE_USE_LOG_DEF_DIR,               // use the default log directory
  IE_LOG_ROT_MAX,                   // max number of rotated logs to keep
  IE_LOG_ROT_NEXT,                  // next log rotation index

  IE_LAST_OPTION_TAB,               // last used tab in options dialog

  IE_LAUNCH_MAIL_CLIENT,            // launch mail client after processing mail
  IE_SOUND_4_NEW_MAIL,              // play sound when new mail arrives
  IE_CHECK_MAIL_STARTUP,            // start mail check at startup
  IE_SCHEDULE_MAIL_CHECK,           // schedule mail check
  IE_MAIL_CHECK_INTERVAL,           // mail check interval - minutes
  IE_SCHEDULE_SERVER_CHECK,         // schedule mail server connection check
  IE_SERVER_CHECK_INTERVAL,         // mail server connection check interval - minutes
  IE_OPT_AUTO_UPDATE_CHECK,         // check automatically for newer version at start up
  IE_OPT_MAX_TOP_LINES,             // max # of message lines to get with TOP request
  IE_ACCOUNT_DLG_H,                 // account dialog height
  IE_ACCOUNT_DLG_W,                 // account dialog width
  IE_ACCOUNT_GRID_COL_WIDTHS,       // string with mail grid col widths w1|w2|w3.. |wn
  IE_PREVIEW_DLG_H,                 // preview dialog height
  IE_PREVIEW_DLG_W,                 // preview dialog width
  IE_FILTER_DLG_H,                  // filter dialog height
  IE_FILTER_DLG_W,                  // filter dialog width
  IE_FILTER_LAST_SEL,               // filter dialog last selection
  IE_FILTER_IMPL_LEGIT,             // action: 'legit' implemented
  IE_FILTER_IMPL_BLACKLIST,         // action: 'blacklist' implemented
  IE_FILTER_IMPL_DELETE,            // action: 'delete' implemented
  IE_FILTER_IMPL_BOUNCE,            // action: 'bounce' implemented

  IE_POP3_SERVER_TIMEOUT,           // POP3 server timeout
  IE_STATUS_DEFAULT_COLOR,          // default color to be used for status field
  IE_MAIL_CLIENT_PATH,              // path to e-mail client
  IE_ADV_POP3_OPTION_DLG_H,         // adv POP3 options dialog height
  IE_ADV_POP3_OPTION_DLG_W,         // adv POP3 options dialog width
#if defined( WANT_AUTO_START )
  IE_AUTO_START,
#endif

  IE_MAX            // keep as last - determines size of struct iniPrefs
};

typedef struct _iniPrefs_t
{
  long      lPrefVer;   // magic number
  entry     data[IE_MAX];
} iniPrefs_t;

#endif   // __WX_MS_CONF_H
// ---------------------- eof ---------------------------------------
