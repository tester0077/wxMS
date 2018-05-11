/////////////////////////////////////////////////////////////////////////////
// Name:        wxMSOptionsFilterConf.cpp
// Purpose:     Config file for wxMS Mail Screener filter data
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
#include "wxMsFilterConfh.h"
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
iniFilterPrefs_t g_iniFilterPrefs = {
 CURRENT_PREF_VERSION,
 {
   { _T("Filter"),    // path - INI heading       [HEADING]                   // enabled/disabled
     _T("State"),     // key                      key=false/0/1.23/some string
      eBool,          // type of config entry
                      // default INI data
                      // values are only present up to last needed
                      // others are deemed to be set to 0/NULL/0.0
         true,        // bool
         0,           // long
         _T(""),      // string
         0.0,         // float
                  // current values - only need to define up to the type
         true },
    { _T("Filter"),               _T("Name"),       eString, 0,     0,  _T("") },  // filter name
    { _T("Filter"),               _T("Status"),     eString, 0,     0,  _T("") },  // string to show in status field
    { _T("Filter"),               _T("Color"),      eString, 0,     0,  _T("#000000") },  // color for status field string
    { _T("Filter"),               _T("Logic"),      eString, 0,     0,  _T("AND") },// use AND or OR to connect rule parts
    { _T("Filter"),               _T("Action"),     eString, 0,     0,  _T("") },  // resulting action: mark for: legitimate, delete, bounce, ...
    { _T("Filter"),               _T("RuleSource"), eString, 0,     0,  _T("") },  // source parts to search: from, subject, message body, ....
    { _T("Filter"),               _T("Criteria"),   eString, 0,     0,  _T("") },  // contains | !contains | is equal to | contains regex | ! contains regex
    { _T("Filter"),               _T("MsgText"),    eString, 0,     0,  _T("") },  // message text or regex string to look for
  }
};

// ------------------------------------------------------------------
void MyApp::CreateFilterConfig( void )
{
  wxASSERT( sizeof(g_iniPrefs.data)/sizeof( entry) == IE_MAX );
  // By default, the config data goes to registry under Windows

  // we're using wxConfig's "create-on-demand" feature: it will create the
  // config object when it's used for the first time. It has a number of
  // advantages compared with explicitly creating our wxConfig:
  //  1) we don't pay for it if we don't use it
  //  2) there is no danger to create it twice

  // ====== .INI file ends up in
  // Home directory
  // XP:            c:/Documents and Settings/userName/Application data/wxSr/
  // Vista:         c:/Users/UserName/AppData/Roaming/wxMS/
  // Win7/8/10      c:/Users/UserName/AppData/Roaming/wxMS/
  wxString wsFileName;

  wsFileName = m_wsConfigDir + _T("/") + m_wsAppName + _T("Filter.ini");

  m_pFilterConfig = new wxFileConfig(
      m_wsAppName,
      wxEmptyString,                // vendor name
      wsFileName,                   // local filename
      _T(""),                       // global filename
      wxCONFIG_USE_LOCAL_FILE );
  wxConfigBase::Set( m_pFilterConfig );

  // see if we have an entry PrefVersion & whether it is recent enough
  if( m_pFilterConfig->Exists( _T("/PrefVersion") )
    && ( m_pFilterConfig->Read( _T("/PrefVersion"), &g_iniPrefs.lPrefVer )
    && ( g_iniFilterPrefs.lPrefVer <= CURRENT_PREF_VERSION ) ) )
  {
    // initialize the default values
    for( int i = 0; i < IE_MAX_FILTER; i++ )
    {
      if ( g_iniFilterPrefs.data[i].eType == eString )
      {
        g_iniFilterPrefs.data[i].dataCurrent.wsVal =
          g_iniFilterPrefs.data[i].dataDefault.wsVal;
      }
    }
  }
  // either found no .INI file or a newer version
  // set defaults and save
  else
  {
    g_iniFilterPrefs.lPrefVer = CURRENT_PREF_VERSION;
    // read any existing entries, with a default, in
    // case they are missing
    //pConfig->Read( "key", &Settings.value, default_value);

    // might be an idea to delete the file here to clean
    // out any cruft
    m_pFilterConfig->DeleteAll();
    m_pFilterConfig->Write( _T("/PrefVersion"), g_iniPrefs.lPrefVer );
    wxString wsData;
    for( int i = 0; i < IE_MAX_FILTER; i++ )
    {
      wxASSERT_MSG( !g_iniPrefs.data[i].wsPathStr.IsEmpty() &&
        !g_iniFilterPrefs.data[i].wsKeyStr.IsEmpty(), _T("NULL entries in iniPrefs.data[]") );
      wsData.Printf( _T("%s/%s"), g_iniFilterPrefs.data[i].wsPathStr,
        g_iniFilterPrefs.data[i].wsKeyStr );
      switch( g_iniFilterPrefs.data[i].eType )
      {
      case eBool:
        m_pFilterConfig->Write( wsData,
          // Note: current == default - supposed to, but may not
          (bool)(g_iniFilterPrefs.data[i].dataDefault.bVal) );
        break;
      case eLong:
        m_pFilterConfig->Write( wsData,
          g_iniFilterPrefs.data[i].dataDefault.lVal );
        break;
      case eString:
        m_pFilterConfig->Write( wsData,
          g_iniFilterPrefs.data[i].dataDefault.wsVal );
        g_iniPrefs.data[i].dataCurrent.wsVal =
          g_iniFilterPrefs.data[i].dataDefault.wsVal;
        break;
      case eFloat:
        m_pFilterConfig->Write( wsData,
          (float)g_iniFilterPrefs.data[i].dataDefault.fVal );
        break;
      }
    }
    m_pFilterConfig->Flush();
  }
}

//-----------------------------------------------------------------------------

void MyApp::RestoreFilterConfig( void )
{
  wxASSERT( sizeof(g_iniFilterPrefs.data)/sizeof( entry) == IE_MAX_FILTER );

  wxString wsOldPath = m_pFilterConfig->GetPath();
  wxString wsFilter( _("/Filter") );
  long lDummy;
  bool bCont;
  bool bOK;
  wxUnusedVar( bOK ); // keep linux compiler happy in release mode
  unsigned int  i = 0;
  wxString wsStr;
  wxString wsKey;
  wxString wsData;
  wxArrayString asGroups;

  // look for Group 'Filter' and then get all members
  m_pFilterConfig->SetPath( wsFilter );
  bCont = m_pFilterConfig->GetFirstGroup( wsStr, lDummy );
  while(  bCont )
  {
    MyFilterListEl *p = new MyFilterListEl();
    wsKey.Printf( _("/Filter/%d/State"), i );
    bOK = m_pFilterConfig->Read( wsKey, &(p->m_bState), true );

    wsKey.Printf( _("/Filter/%d/Name"), i );
    bOK = m_pFilterConfig->Read( wsKey, &(p->m_wsName), _("") );

    wsKey.Printf( _("/Filter/%d/Status"), i );
    bOK = m_pFilterConfig->Read( wsKey, &(p->m_wsStatus), _("") );

    wsKey.Printf( _("/Filter/%d/Color"), i );
    bOK = m_pFilterConfig->Read( wsKey, &(p->m_wsColorHex), _T("grey") );

    wsKey.Printf( _("/Filter/%d/Logic"), i );
    bOK = m_pFilterConfig->Read( wsKey, &(p->m_bAndOr), true);

    wsKey.Printf( _("/Filter/%d/Action"), i );
    bOK = m_pFilterConfig->Read( wsKey, &(p->m_wsAction), _("") );

    wsKey.Printf( _("/Filter/%d/RuleSource"), i );
    bOK = m_pFilterConfig->Read( wsKey, &(p->m_wsRuleSource), _("") );

    wsKey.Printf( _("/Filter/%d/Criteria"), i );
    bOK = m_pFilterConfig->Read( wsKey, &(p->m_wsRuleCriteria), _("") );

    wsKey.Printf( _("/Filter/%d/MsgText"), i );
    bOK = m_pFilterConfig->Read( wsKey, &(p->m_wsRuleText), _("") );

    m_FilterList.push_back( *p );
    delete p;
    i++;
    bCont = m_pFilterConfig->GetNextGroup(wsStr, lDummy );
    wxASSERT( bOK );  // to keep GCC quiet
  }
  m_pFilterConfig->SetPath( wsOldPath );
}

//-----------------------------------------------------------------------------
void MyApp::SaveFilterConfig( void )
{
  if ( m_pFilterConfig == NULL )
    return;
  m_pFilterConfig->SetPath(_T("/"));

  unsigned int  i = 0;
  wxString      wsKey;
  wxString      wsData;

  wxString wsOldPath = m_pFilterConfig->GetPath();
  // delete the existing 'Filter' group
  m_pFilterConfig->DeleteGroup( _("/Filter") );
  i = 0;
  for ( std::vector<MyFilterListEl>::iterator it = m_FilterList.begin();
    it != m_FilterList.end(); ++it, i++ )
  {
    //    ...process the current element...
    wsKey.Printf( _("/Filter/%d/State"), i );
    m_pFilterConfig->Write( wsKey, it->m_bState );
    wsKey.Printf( _("/Filter/%d/Name"), i );
    m_pFilterConfig->Write( wsKey, it->m_wsName );
    wsKey.Printf( _("/Filter/%d/Status"), i );
    m_pFilterConfig->Write( wsKey, it->m_wsStatus );
    wsKey.Printf( _("/Filter/%d/Color"), i );
    m_pFilterConfig->Write( wsKey, it->m_wsColorHex );
    wsKey.Printf( _("/Filter/%d/Logic"), i );
    m_pFilterConfig->Write( wsKey, it->m_bAndOr );
    wsKey.Printf( _("/Filter/%d/Action"), i );
    m_pFilterConfig->Write( wsKey, it->m_wsAction );
    wsKey.Printf( _("/Filter/%d/RuleSource"), i );
    m_pFilterConfig->Write( wsKey, it->m_wsRuleSource );
    wsKey.Printf( _("/Filter/%d/Criteria"), i );
    m_pFilterConfig->Write( wsKey, it->m_wsRuleCriteria );
    wsKey.Printf( _("/Filter/%d/MsgText"), i );
    m_pFilterConfig->Write( wsKey, it->m_wsRuleText );
  }
  m_pFilterConfig->SetPath( wsOldPath );
  m_pFilterConfig->Flush();

  m_pFilterConfig->Flush();// needed to ensure data is written
  // avoid memory leaks
  delete m_pFilterConfig;
}

// ------------------------------------------------------------------

/**
  Make a copy of the Config data structure. The copy is intended to
  * be passed to a dialog which might change data. Using the copy
  * will allow the user to save the data on exit or
  * abandon the changes if he choses 'Cancel'
  */
void MyApp::MakeFilterConfigCopy( iniFilterPrefs_t& iniPrefsSrc,
    iniFilterPrefs_t& iniPrefsTarg )
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
