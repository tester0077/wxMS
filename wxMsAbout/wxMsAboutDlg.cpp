/*-----------------------------------------------------------------
 * Name:        wxMsAboutDlg.cpp
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets licence
 *---------------------------------------------------------------- */

/*----------------------------------------------------------------
 * Standard wxWidgets headers
 *---------------------------------------------------------------- */
#if defined( _MSC_VER )
#  if defined ( _DEBUG )
 // this statement NEEDS to go BEFORE all headers
#    define _CRTDBG_MAP_ALLOC
#    include <stdlib.h>
#    include <crtdbg.h>
#  endif
#endif
#include "wxMsPreProcDefsh.h"		// MUST be first
// ------------------------------------------------------------------

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
#include "wxMsAboutDlgBase.h"
#include "wxMsFrameh.h"
#include <curl/curl.h>
// ------------------------------------------------------------------
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
class wxMsAboutDlg: public wxMsAboutDlgBase
{
public:
  /// Constructors
  wxMsAboutDlg();
  wxMsAboutDlg( wxWindow* parent );

  /// Destructor
  ~wxMsAboutDlg() {};

  wxString m_wsLibcurlVersion;
  wxString m_wsMimeticVersion;
};

// ------------------------------------------------------------------
wxMsAboutDlg::wxMsAboutDlg(wxWindow* parent) :
  wxMsAboutDlgBase( parent  )
{
  m_wsLibcurlVersion = curl_version();
  m_wsMimeticVersion = mimetic::version.str();
}

// ------------------------------------------------------------------
void MyFrame::OnAbout( wxCommandEvent& event )
{
  wxMsAboutDlg dlg( this );

//  dlg.m_wsLibcurlVersion = curl_version();

  // build information
#if defined( WANT_EXE_LINK_DATE )
  wxString wsBuildDate;
  wxString wsYear;
  bool bRet;
  wxString wsIndent(_T("      "));
  unsigned int uiYear1 = COPYRIGHT_YEAR_1;
  wxString wsCopyrightYear1;
  wsCopyrightYear1.Printf(_T("%u"), uiYear1);
  bRet = wxGetApp().RetrieveLinkerTimestamp(wsBuildDate, wsYear);
  wxASSERT_MSG(bRet, _T("RetrieveLinkerTimestamp() failed"));
  if (!wsYear.IsSameAs(wsCopyrightYear1))
  {
    wsYear = wsCopyrightYear1 + _T(" - ") + wsYear;
    wsIndent = wxEmptyString;
  }
#else
  wxString wsBuildDate( _T("unknown") );
  wxString wsYear( _T("unknown") );
#endif
  // copyright
  wxString wsT1 = dlg.GetStaticTextCopyright()->GetLabel();
  //  (wxChar) 0x00A9, wsYear,
  wxString wsT;
  wsT.Printf(wsT1, (wxChar)0x00A9, wsYear);
  dlg.GetStaticTextCopyright()->SetLabel( wsT );
#if defined( WANT_FILEVERSION )
  // Version
  wsT1 = dlg.GetStaticTextVersion()->GetLabel();
  wsT.Printf(_(" %d.%d - Build: %d - %s"), giMajorVersion, giMinorVersion, giBuildNumber, wsBuildDate);
  dlg.GetStaticTextVersion()->SetLabel( wsT1 + wsT );
#endif
  // wxWidgets
  wsT.Printf(_("wxWidgets: %d.%d.%d"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER);
  dlg.GetStaticTextwxWidgetsVersion()->SetLabel(wsT);

  // Compiler
  wxString wsCompiler = _("unknown");
#if defined( _MSC_VER )
  wsCompiler = _T("MSVC 2010");
#if _MSC_VER >= 1900
  wsCompiler = _T("MSVC 2015");
#elif _MSC_VER >= 1600
  wsCompiler = _T("MSVC 2010");
#endif
#else // asume it is Linux flavor
#endif
  wsT.Printf(_("Compiler: %s"), wsCompiler);
  dlg.GetStaticTextCompiler()->SetLabel(wsT);
  // OS
#if defined( _MSC_VER )
  wxPlatformInfo wxPI = wxPlatformInfo::Get();
  if ( wxPI.IsOk() )
  {
    // from: http://msdn.microsoft.com/en-us/library/windows/desktop/ms724832%28v=vs.85%29.aspx
    wxString wsOS;
    int iMinor = wxPI.GetOSMinorVersion();
    switch (wxPI.GetOSMajorVersion())
    {
    case 5:
      if (iMinor == 0)
        wsOS = _T("Win 2000");
      else if (iMinor == 1)
        wsOS = _T("XP");
      else if (iMinor == 2)
        wsOS = _T("Win Server 2003");
      break;
    case 6:
      if (iMinor == 0)
        wsOS = _T("Vista");
      else if (iMinor == 1)
        wsOS = _T("Win7");
      else if (iMinor == 2)
        wsOS = _T("Win 8");
      else if (iMinor == 3)
        wsOS = _T("Win 8.1");
      break;
    case 10:
      if (iMinor == 0)
        wsOS = _T("Win 10");
      break;
    default:
      wsOS = _T("unknown");
      break;
    }
    wsT.Printf(_("\nRunning on %s - %s (%d.%d), %s-Bit"), wxPI.GetOperatingSystemFamilyName(),
    wsOS, wxPI.GetOSMajorVersion(), wxPI.GetOSMinorVersion(),
    (wxPI.GetArchitecture() == wxARCH_64) ? _T("64") : _T("32"),
    wxPI.GetEndiannessName(), wxPI.GetPortIdName());
    dlg.GetStaticTextOS()->SetLabel(wsT);
  }
  wxString wsDebuggerPresent;
    wsDebuggerPresent =
      IsDebuggerPresent() ? _("Debugger") : _("No Debugger");
  wsT.Printf(_("%s - Port: %s - %s"), wxPI.GetEndiannessName(), wxPI.GetPortIdName(), wsDebuggerPresent );
#else // asume it is Linux flavor
  wxPlatformInfo wxPI = wxPlatformInfo::Get();
  if ( wxPI.IsOk() )
  {
    wsT.Printf(_("\nRunning on %s, %s-Bit Arch."), wxPI.GetOperatingSystemFamilyName(),
    (wxPI.GetArchitecture() == wxARCH_64) ? _T("64") : _T("32"),
    wxPI.GetEndiannessName(), wxPI.GetPortIdName());
    dlg.GetStaticTextOS()->SetLabel(wsT);
  }
#endif
  dlg.GetStaticTextOsPort()->SetLabel(wsT);
  // wxWidgets version
  wsT.Printf( _T("wxWidgets: %d.%d.%d"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER );
  dlg.GetStaticTextwxWidgetsVersion()->SetLabel( wsT );

  // libcurl version
  dlg.GetStaticTextLibcurlVersion()->SetLabel( _T("libcurl: ") + dlg. m_wsLibcurlVersion );
  // mimetic version
  dlg.GetStaticTextMimeticVersion()->SetLabel( _T("mimetic: ") +  dlg.m_wsMimeticVersion );

  // Config dir
#if defined( _MSC_VER )
  dlg.GetStaticTextConfigDir()->SetLabel( wxGetApp().m_wsConfigDir + _T("\\") );
#else
  dlg.GetStaticTextConfigDir()->SetLabel( wxGetApp().m_wsConfigDir + _T("/") );
#endif
  // hide the unused pages
//  dlg.GetNotebookData()->DeletePage(3); // license // start at highest #
//  dlg.GetNotebookData()->DeletePage(2); // Credits
  dlg.GetNotebookData()->DeletePage(1); // Artists
  dlg.EnableLayoutAdaptation( true );
  dlg.DoLayoutAdaptation();
  dlg.GetNotebookData()->SetSelection(3); // System Info page
  dlg.Fit();
	dlg.ShowModal();
}

// ------------------------------- eof ------------------------------
