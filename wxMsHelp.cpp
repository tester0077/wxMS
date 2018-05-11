//-------------------------------------------------------------------
// Name:        wxMseHelp.cpp
// Purpose:     Code for the wxMS Help support
// Author:
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

// ------------------------------------------------------------------
// Note __VISUALC__ is defined by wxWidgets, not by MSVC IDE
// and thus won't be defined until some wxWidgets headers are included
#if defined( _MSC_VER )
// only good for MSVC
// this block needs to AFTER all headers
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif
#endif
// -------------------------------------------------------------

#if defined( WANT_ZIP_HELP )
void MyFrame::OnShowSection_FileMenu(wxCommandEvent &WXUNUSED( event ) )
{
  m_msHtmlHelp.DisplaySection( _T("File"));
}
#endif

#if defined( WANT_HELP )
// ------------------------------------------------------------------

void MyFrame::OnShowHelpContents(wxCommandEvent &WXUNUSED( event ) )
{
  wxFileName wfnHelp( m_wsCurHelpFile );
  if( !wfnHelp.FileExists() )
  {
    wxMessageBox( _("Can't find the Help file: \n") + m_wsCurHelpFile,
      _("Error"),  wxOK | wxICON_ERROR );
    return;
  }
  m_msHtmlHelp.DisplayContents();
}

// ------------------------------------------------------------------

void MyFrame::OnKeywordSearch(wxCommandEvent &WXUNUSED( event ) )
{
  wxString keyword = wxGetTextFromUser(_("Keyword Search"));
  if(!keyword.IsEmpty())
  {
    m_msHtmlHelp.KeywordSearch(keyword);
  }
}

// ------------------------------------------------------------------

void MyFrame::OnMenuShowHelpContents( wxCommandEvent& event )
{
  OnShowHelpContents( (wxHelpEvent&)event );
}
#endif
// ------------------------------------------------------------------

#if 0
void MyFrame::OnDialogContextHelp(wxCommandEvent &WXUNUSED( event ) )
{
  MyDialog dlg(this);
  dlg.ShowModal();
}
#endif

// ------------------------------- eof ------------------------------
