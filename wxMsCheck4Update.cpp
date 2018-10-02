//--------------------------------------------------------------
// Name:        wxMsCheck4Update.cpp
// Purpose:
// Author:      A. Wiegert
//
// Copyright:
// Licence:     wxWidgets license
//--------------------------------------------------------------

//--------------------------------------------------------------
// Standard wxWidgets headers
//--------------------------------------------------------------
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

// For all others, include the necessary headers (this file is
// usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wxMsFrameh.h"
#include "wxMsUpdateCheckCurlh.h"

// ------------------------------------------------------------------
// Note __VISUALC__ is defined by wxWidgets, not by MSVC IDE
// and thus won't be defined until some wxWidgets headers are included
#if defined( _MSC_VER )
// only good for MSVC
// this block needs to go AFTER all headers
#include <crtdbg.h>
#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif
#endif
// ------------------------------------------------------------------
/**
 * Check for if we have the latest version.
 * Get the string from the download site and advise user
 * if a more recent version is available.
 * This rotine is called from the OnIdle() handler when the application
 * first starts up, with bFromUser = false 
 * To allow the user to call for a check, we'll return the status
 * so we can advise the user if there is no newer version
 * But note: if the remote site is down or can't be reached, then for the release
 * version, we simply return.
 * for the debug version, we want to throw up a bit more information
 */

bool MyFrame::Check4Update( bool bFromUser )
{
#if defined( WANT_FILEVERSION )
  wxString wsUrl = CHECK_4_UPDATE_URL;
  wxString wsUserAgent;
  wxString wsT;
  wxString wsVersion;

  wsUserAgent.Printf( _T("%s-%d.%d.%d"),  USER_AGENT, giMajorVersion, giMinorVersion, giBuildNumber);
  MyUpdateCurlHttpStatusObject MyCurl( wsUrl, wsUserAgent );
  wxString wsVersionHtml = MyCurl.getData();
  CURLcode code = MyCurl.GetReturnCode();
  // CURLE_OK simply means that libcurl found the remote site
  // it does not imply that it found the file we were looking for
  if( code != CURLE_OK )
  {
#if defined (_DEBUG )
    wsT.Printf( _("%s, Line: %ld - Got error %d %s from libcurl interface!\n") +
      _("Likely cannot reach the remote site %s"), 
      __FILE__, __LINE__, code, curl_easy_strerror(code), wsUrl );
    wsT.Printf( wsT );
    wxMessageBox( wsT, "Error", wxOK );
#endif
    return false;
  }
#if defined( _DEBUG )
  if( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
    wxLogMessage( wsVersionHtml );
#endif
  // extract & clean up the string
  // first find the 'Latest Version" line
  int iStart = wsVersionHtml.Find( _T("Latest Version") );
#if _DEBUG
  if ( iStart == wxNOT_FOUND )
  {
    wsT = _("Could not locate:\n ") + wsUrl;
    wxMessageBox( wsT, "Notice", wxOK );
    return false;
  }
#else
  wxASSERT( iStart != wxNOT_FOUND );
#endif
  wsVersion = wsVersionHtml.Mid( iStart );
  wsVersion = wsVersion.AfterFirst( ':' );
  wsVersion = wsVersion.BeforeFirst( '<' );
  wsVersion = wsVersion.Trim( true );
  wsVersion = wsVersion.Trim( false );

  // now convert & test against current version
  wxString wsMajor = wsVersion.BeforeFirst( '.' );
  wxString wsMinor = wsVersion.AfterFirst( '.' );
  wxString wsBuild = wsMinor.AfterFirst( '.' );
  wsMinor = wsMinor.BeforeFirst( '.' );
  long lMajor;
  long lMinor;
  long lBuild;
  wsMajor.ToLong( &lMajor );
  wsMinor.ToLong( &lMinor );
  wsBuild.ToLong( &lBuild );
  // set up the dialog so we can retrieve the format string for the version
  MyDialogUpdate dlg( this );
  wxHyperlinkCtrl *pURL = dlg.GetHyperlinkwxMS();
  wxString wsTVersion = dlg.GetStaticTextVersion()->GetLabel();

  wsT.Printf( wsTVersion,(int)lMajor, (int)lMinor, (int)lBuild );
  if( (lMajor > giMajorVersion) ||
    ( (lMajor == giMajorVersion) && (lMinor > giMinorVersion) ) ||
    ( (lMajor == giMajorVersion) && (lMinor == giMinorVersion)  &&
    (lBuild > giBuildNumber) ) )
  {
    dlg.GetStaticTextVersion()->SetLabel( wsT );
    // check for updates at startup
    dlg.GetCheckBoxCheckAtStartup()->SetValue(
      g_iniPrefs.data[IE_OPT_AUTO_UPDATE_CHECK].dataCurrent.bVal );
    dlg.ShowModal();
    // save the new state
    g_iniPrefs.data[IE_OPT_AUTO_UPDATE_CHECK].dataCurrent.bVal =
      dlg.GetCheckBoxCheckAtStartup()->GetValue();
    return true;
  }
  if ( bFromUser )
  {
    wxMessageBox( _T("You have the latest version!"), "Notice", wxOK );
  }
#endif
  return false;
}

// ------------------------------------------------------------------
/**
 * Allow user to manually check for a newer version.
 * Here we only need to handle the case where the user has
 * the latest available version.
 * The alternate case is handled by the called function.
 */
void MyFrame::OnCheck4Update(wxCommandEvent& event)
{
#if defined( WANT_FILEVERSION )
  // suppress the extra "You have the latest version" message
  //                    V
  if( ! Check4Update( false ) )
  {
#if 1 // new dialog
    MyDialogNoUpdate dlg( this );
    // check for updates at startup
    dlg.GetCheckBoxCheckAtStartup()->SetValue(
      g_iniPrefs.data[IE_OPT_AUTO_UPDATE_CHECK].dataCurrent.bVal );
    dlg.ShowModal();
    // save the new state
    g_iniPrefs.data[IE_OPT_AUTO_UPDATE_CHECK].dataCurrent.bVal =
      dlg.GetCheckBoxCheckAtStartup()->GetValue();
#else
    wxString wsT;
    wsT.Printf( _("    No newer version found!\n") +
      _("    You are using version: %d.%d.%d.") +
      _("\n\n    Please note:\n") +
      _("    Automatic checking for updates at startup can be\n") +
      _("    enabled or disabled in the 'Options' dialog"),
      giMajorVersion, giMinorVersion, giBuildNumber );
      wxMessageBox( wsT, "Notice", wxOK );
#endif
  }
#endif
  event.Skip();
}

// ------------------------------- eof -------------------------
