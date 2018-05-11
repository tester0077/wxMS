/////////////////////////////////////////////////////////////////////////////
// Name:        wxMSConfig.cpp
// Purpose:     Config file for wxMS Mail Screener
// Author:      A. Wiegert based on Julian Smart's  code
// Modified by:
// Created:     Mar 15/07
// Copyright:   (c) A. Wiegert
// Licence:     wxWidgets license
/////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------
// Note __VISUALC__ is defined by wxWidgets, not by MSVC IDE
// and thus won't be defined until some wxWidgets headers are included
#if defined( _MSC_VER )
#  if defined ( _DEBUG )
     // this statement NEEDS to go BEFORE all headers
#    define _CRTDBG_MAP_ALLOC
#  endif
#endif
#include "wxMsPreProcDefsh.h"   // needs to be first
// ------------------------------------------------------------------
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
    #include "wx/log.h"
#endif

#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/filename.h>

#include "wxMsh.h"
#include "wxMsConfh.h"
#include "wxMsFrameh.h"

// ------------------------------------------------------------------
// Note __VISUALC__ is defined by wxWidgets, not by MSVC IDE
// and thus won't be defined until some wxWidgets headers are included
#if defined( _MSC_VER )
// only good for MSVC
// this block needs to go AFTER all headers
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif
// ------------------------------------------------------------------

#ifdef __WIN32__
    // this is not supported by native control
    #define NO_VARIABLE_HEIGHT
#endif

// ------------------------------------------------------------------
#undef _USE_REGISTRY__  // use .INI file instead
// ------------------------------------------------------------------
/* Note: the entries in enum iniEnts define the sequence of data entries here
 * and MUST be in corresponding sequence
 * Note the number of initializers here must match the
 * number of entries specified in the config.h file
 */
iniPrefs_t g_iniPrefs = {
 CURRENT_PREF_VERSION,
 {
   { _T("LogWindow"),// path - INI heading       [HEADING]
     _T("Wanted"),   // key                      key=false/0/1.23/some string
      eBool,      // type of config entry
                  // default INI data
                  // values are only present up to last needed
                  // others are deemed to be set to 0/NULL/0.0
         true,       // bool
         0,          // long
         _T(""),     // string
         0.0,        // float
                  // current value
         true },
    { _T("Sash"),                 _T("Position"),   eLong,   0,     60            },
    { _T("MainFrame"),            _T("x"),          eLong,   0,     10            },
    { _T("MainFrame"),            _T("y"),          eLong,   0,     20            },
    { _T("MainFrame"),            _T("w"),          eLong,   0,     300           },
    { _T("MainFrame"),            _T("h"),          eLong,   0,     300           },
    { _T("OptDlg"),               _T("Height"),     eLong,   0,     250           },
    { _T("OptDlg"),               _T("Width"),      eLong,   0,     400           },
    { _T("Config"),               _T("Last"),       eString, 0,     0,  _T("")    },

    // mail grid col widths, as string of numbers with | separator
    { _T("MailGrid"),             _T("Cols"),       eString, 0,     0,
      _T("40|40|40|80|120|120|80|80|120|80")                                      },
    { _T("MailGrid"),             _T("Labels"),     eString, 0,     0,
      _T("Delete|Bounce|Blacklist|Status|Size|From|Subject|To|Date|Account")      },
    { _T("MaxTasks"),             _T("Number"),     eLong,   0,    20             },

    { _T("LogFile"),              _T("Wanted"),     eBool,   false                },
    { _T("Log"),                  _T("Verbosity"),  eLong,   0,    4              },
    { _T("NewLog"),               _T("Frequency"),  eLong,   0,    0              },
    { _T("LogDir"),               _T("Path"),       eString, 0,    0,  _T("")     },
    { _T("LogDir"),               _T("UseDefault"), eBool,   true                 },
    { _T("LastOptionDlg"),        _T("Tab"),        eLong,   0,    0              },

    { _T("MailClient"),           _T("Launch"),     eBool,   true                 },
    { _T("MailSound"),            _T("NewMail"),    eBool,   true                 },
    { _T("MailCheck"),            _T("Startup"),    eBool,   false                },
    { _T("MailCheck"),            _T("Schedule"),   eBool,   true                 },
    { _T("MailCheck"),            _T("Interval"),   eLong,   0,    10             },
    { _T("ServerCheck"),          _T("Schedule"),   eBool,   true                 },
    { _T("ServerCheck"),          _T("Interval"),   eLong,   0,    10             },
    { _T("Update"),               _T("AutoCheck"),  eBool,   true                 },
    { _T("TOP_Lines"),            _T("Number"),     eLong,   0,    0              },
    { _T("AcctDlg"),              _T("Height"),     eLong,   0,     250           },
    { _T("AcctDlg"),              _T("Width"),      eLong,   0,     400           },
    { _T("AccountGrid"),          _T("Cols"),       eString, 0,     0,   _T("40|40|40|80") },
    { _T("Preview"),              _T("Height"),     eLong,   0,     250           },
    { _T("Preview"),              _T("Width"),      eLong,   0,     400           },
    { _T("Filter"),               _T("Height"),     eLong,   0,     250           },
    { _T("Filter"),               _T("Width"),      eLong,   0,     400           },
    { _T("Filter"),               _T("LastSel"),    eLong,   0,     0             },
    { _T("Action"),               _T("Legit"),      eBool,   true                 },  // action: 'legit' implemented
    { _T("Action"),               _T("Blacklist"),  eBool,   false                },  // action: 'blacklist' not implemented
    { _T("Action"),               _T("Delete"),     eBool,   true                 },  // action: 'delete' implemented
    { _T("Action"),               _T("Bounce"),     eBool,   false                },  // action: 'bounce' not implemented
    { _T("POP3"),                 _T("TimeOut"),    eLong,   0,    10             },  // secs to wait for POP3 server response
    { _T("Status"),               _T("DefColor"),   eString, 0,    0,   _T("#000000")  },  // default color for the status field - black
    { _T("EmailClient"),          _T("Path"),       eString, 0,    0,   _T("")    },
    { _T("AdvPop3Optn"),          _T("Height"),     eLong,   0,     250           },
    { _T("AdvPop3Optn"),          _T("Width"),      eLong,   0,     400           },
  }
};

// ------------------------------------------------------------------
void MyApp::CreateConfig( void )
{
#if !defined( _MSC_VER )
  wxCHANGE_UMASK( 00666 );
#endif
  wxASSERT( sizeof(g_iniPrefs.data)/sizeof( entry) == IE_MAX );
  // By default, the config data goes to registry under Windows

  // we're using wxConfig's "create-on-demand" feature: it will create the
  // config object when it's used for the first time. It has a number of
  // advantages compared with explicitly creating our wxConfig:
  //  1) we don't pay for it if we don't use it
  //  2) there is no danger to create it twice

  // ====== .INI file ends up in
  // Home directory
  // XP:     c:/Documents and Settings/userName/Application data/wxSr/
  // Vista:  c:/Users/UserName/AppData/Roaming/wxMS/
  // Win7    c:/Users/UserName/AppData/Roaming/wxMS/
  // Linux   ~/$USER/.wxMS/
  wxString wsFileName;
#if 1 //defined( _MSC_VER )
  wsFileName = m_wsConfigDir + _T("/") + m_wsAppName + _T(".ini");
#else
   /**
   * Setup default directory stuff for Linux
    * as per https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
    * summary at:
    https://stackoverflow.com/questions/1024114/location-of-ini-config-files-in-linux-unix

    $XDG_CONFIG_HOME (default: "$HOME/.config"): user-specific configuration files.
   *
   in my case $HOME -> /home/arnold -> /home/$USER
   */
  wsFileName = m_wsConfigDir + /*_T("/.") + m_wsAppName + */ _T("/")  + m_wsAppName + _T(".ini");
#endif

//  if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
//    wxLogMessage( _T(".INI file: ") + wsFileName );

  m_pConfig = new wxFileConfig(
      m_wsAppName,
      wxEmptyString,                // vendor name
      wsFileName,                   // local filename
      _T(""),                       // global filename
      wxCONFIG_USE_LOCAL_FILE );
  wxConfigBase::Set( m_pConfig );
#if !defined( _MSC_VER )
  m_pConfig->SetUmask( 00777 );
#endif
  // see if we have an entry PrefVersion & whether it is recent enough
  if( m_pConfig->Exists( _T("/PrefVersion") )
    && ( m_pConfig->Read( _T("/PrefVersion"), &g_iniPrefs.lPrefVer )
    && ( g_iniPrefs.lPrefVer <= CURRENT_PREF_VERSION ) ) )
  {
    // initialize the default values
    for( int i = 0; i < IE_MAX; i++ )
    {
      if ( g_iniPrefs.data[i].eType == eString )
      {
        g_iniPrefs.data[i].dataCurrent.wsVal =
          g_iniPrefs.data[i].dataDefault.wsVal;
      }
    }
  }
  // either found no .INI file or a newer version
  // set defaults and save
  else
  {

    g_iniPrefs.lPrefVer = CURRENT_PREF_VERSION;
    // read any existing entries, with a default, in
    // case they are missing
    //pConfig->Read( "key", &Settings.value, default_value);

    // might be an idea to delete the file here to clean
    // out any cruft
    m_pConfig->DeleteAll();
    m_pConfig->Write( _T("/PrefVersion"), g_iniPrefs.lPrefVer );
    wxString wsData;
    for( int i = 0; i < IE_MAX; i++ )
    {
      wxASSERT_MSG( !g_iniPrefs.data[i].wsPathStr.IsEmpty() &&
        !g_iniPrefs.data[i].wsKeyStr.IsEmpty(), _T("NULL entries in iniPrefs.data[]") );
      wsData.Printf( _T("%s/%s"), g_iniPrefs.data[i].wsPathStr.c_str(),
        g_iniPrefs.data[i].wsKeyStr.c_str() );
      switch( g_iniPrefs.data[i].eType )
      {
      case eBool:
        m_pConfig->Write( wsData,
          // Note: current == default - supposed to, but may not
          (bool)(g_iniPrefs.data[i].dataDefault.bVal) );
        break;
      case eLong:
        m_pConfig->Write( wsData,
          g_iniPrefs.data[i].dataDefault.lVal );
        break;
      case eString:
        m_pConfig->Write( wsData,
          g_iniPrefs.data[i].dataDefault.wsVal );
        g_iniPrefs.data[i].dataCurrent.wsVal =
          g_iniPrefs.data[i].dataDefault.wsVal;
        break;
      case eFloat:
        m_pConfig->Write( wsData,
          (float)g_iniPrefs.data[i].dataDefault.fVal );
        break;
      }
    }
    m_pConfig->Flush();
  }
}
//-----------------------------------------------------------------------------

void MyApp::RestoreConfig ( void )
{
//  int x, y, w, h;
//  x = sizeof(g_iniPrefs.data);
//  y = sizeof( entry);
//  w = x/y;
  wxASSERT( sizeof(g_iniPrefs.data)/sizeof( entry) == IE_MAX );

  // restore the control's values from the config

  // NB: in this program, the config object is already created
  // at this moment because we had called Get() from MyApp::OnInit().
  // However, if you later change the code and don't create it
  // before this line, it won't break anything - unlike if you
  // manually create wxConfig object with Create() or in any
  // other way (then you must be sure to create it before using it!).
  m_pConfig = (wxFileConfig *)wxConfigBase::Get();

  if( m_pConfig->Read( _T("/PrefVersion"), &g_iniPrefs.lPrefVer)
      && g_iniPrefs.lPrefVer < CURRENT_PREF_VERSION )
  {
    int answer = wxMessageBox(
      _T("The configuration file version is incompatible\n")
      _T("with the current application.\n")
      _T("OK to overwrite?"), _T("Confirm"), wxYES_NO );
    if ( answer == wxYES )
    {
      m_pConfig->DeleteAll();
      CreateConfig();
    }
    // simply carry on if user won't let us update the config file
    // it WILL complain the next time as well, until ...
    //else
    //  return;
  }
  m_pConfig->SetPath(_T("/"));

  wxString wsKey;
  bool  bOK = false;
  wxString wsS = _T("");
  for( int i = 0; i < IE_MAX; i++ )
  {
    wxASSERT( !g_iniPrefs.data[i].wsPathStr.IsEmpty()
      && !g_iniPrefs.data[i].wsKeyStr.IsEmpty() );
    wsKey.Printf( _T("/%s/%s"), g_iniPrefs.data[i].wsPathStr.c_str(),
      g_iniPrefs.data[i].wsKeyStr.c_str() );
    switch( g_iniPrefs.data[i].eType )
    {
      case eBool:
        bOK = m_pConfig->Read( wsKey, &(g_iniPrefs.data[i].dataCurrent.bVal),
          g_iniPrefs.data[i].dataDefault.bVal );
        break;
      case eLong:
        bOK = m_pConfig->Read( wsKey, &(g_iniPrefs.data[i].dataCurrent.lVal),
          g_iniPrefs.data[i].dataDefault.lVal );
        break;
      case eString:
        bOK = m_pConfig->Read( wsKey, &g_iniPrefs.data[i].dataCurrent.wsVal,
          g_iniPrefs.data[i].dataDefault.wsVal );
        break;
      case eFloat:
        double f;
        bOK = m_pConfig->Read( wsKey, &f,
          g_iniPrefs.data[i].dataDefault.fVal );
        if( bOK )
          g_iniPrefs.data[i].dataCurrent.fVal = f;
        break;
    }
  }
}

//-----------------------------------------------------------------------------
void MyApp::SaveConfig( void )
{
  if ( m_pConfig == NULL )
    return;
  m_pConfig->SetPath(_T("/"));
  wxString wsKey;
  for( int i = 0; i < IE_MAX; i++ )
  {
    wsKey.Printf( _T("%s/%s"), g_iniPrefs.data[i].wsPathStr.c_str(),
         g_iniPrefs.data[i].wsKeyStr.c_str() );
    switch( g_iniPrefs.data[i].eType )
    {
      case eBool:
        m_pConfig->Write( wsKey,
          (bool)(g_iniPrefs.data[i].dataCurrent.bVal) );
        break;
      case eLong:
        m_pConfig->Write( wsKey,
          (int)g_iniPrefs.data[i].dataCurrent.lVal );
        break;
      case eString:
        m_pConfig->Write( wsKey,
          g_iniPrefs.data[i].dataCurrent.wsVal );
        break;
      case eFloat:
        m_pConfig->Write( wsKey,
              (float)g_iniPrefs.data[i].dataCurrent.fVal );
        break;
    }
  }
  m_pConfig->Flush();// needed to ensure data is written
  // avoid memory leaks
  delete m_pConfig;
}

// ------------------------------------------------------------------

/**
  Make a copy of the Config data structure. The copy is intended to
  * be passed to a dialog which might change data. Using the copy
  * will allow the user to save the data on exit or
  * abandon the changes if he choses 'Cancel'
  */
void MyApp::MakeConfigCopy( iniPrefs_t& iniPrefsSrc,
    iniPrefs_t& iniPrefsTarg )
{
  iniPrefsTarg.lPrefVer = iniPrefsSrc.lPrefVer;
  for( int i = 0; i < IE_MAX; i++ )
  {
    iniPrefsTarg.data[i].wsPathStr = iniPrefsSrc.data[i].wsPathStr;
    iniPrefsTarg.data[i].wsKeyStr = iniPrefsSrc.data[i].wsKeyStr;
    iniPrefsTarg.data[i].eType = iniPrefsSrc.data[i].eType;
    switch( iniPrefsSrc.data[i].eType )
    {
      case eBool:
        iniPrefsTarg.data[i].dataCurrent.bVal =
          iniPrefsSrc.data[i].dataCurrent.bVal;
        break;
      case eLong:
        iniPrefsTarg.data[i].dataCurrent.lVal =
          iniPrefsSrc.data[i].dataCurrent.lVal;
        break;
      case eString:
        iniPrefsTarg.data[i].dataCurrent.wsVal =
          iniPrefsSrc.data[i].dataCurrent.wsVal;
        break;
      case eFloat:
        iniPrefsTarg.data[i].dataCurrent.fVal =
          iniPrefsSrc.data[i].dataCurrent.fVal;
        break;
    }
  }
}

// ---------------------------- eof -------------------------------
