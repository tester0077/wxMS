//-------------------------------------------------------------------
// Name:        wxMsCmdline.cpp
// Purpose:     command line parser for wxMS Mail Screener
// Author:      taken partly from wxWidgets wiki
// Copyright:   (c) ??
// Licence:     wxWidgets license
//-------------------------------------------------------------------
//-------------------------------------------------------------------
// Standard wxWidgets headers
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

#include "wxMsh.h"
//-------------------------------------------------------------------
// Header of this .cpp file
//-------------------------------------------------------------------

#include "wxMsCmdlineh.h"
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

#if defined (WANT_CMD_LINE)
// ------------------------------------------------------------------
void MyApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc (g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars ( _T("-"));
}
// ------------------------------------------------------------------
bool MyApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    m_bSilentMode = parser.Found( _T("s"));

    // to get at your unnamed parameters use
    wxArrayString files;
    for ( size_t i = 0; i < parser.GetParamCount(); i++)
    {
            files.Add(parser.GetParam(i));
    }

    // and other command line parameters

    // then do what you need with them.
    return true;
}
#endif
// ------------------------------- eof ------------------------------
