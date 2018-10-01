/*-----------------------------------------------------------------
 * Name:        wxMsMessagePreview.cpp
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets licence
 *---------------------------------------------------------------- */

/*----------------------------------------------------------------
 * Standard wxWidgets headers
 *---------------------------------------------------------------- */
// Note __VISUALC__ is defined by wxWidgets, not by MSVC IDE
// and thus won't be defined until some wxWidgets headers are included
#if defined( _MSC_VER )
#  if defined ( _DEBUG )
     // this statement NEEDS to go BEFORE all headers
#    define _CRTDBG_MAP_ALLOC
#    include <stdlib.h>
#    include <crtdbg.h>
#  endif
#endif
#include "wxMsPreProcDefsh.h"   // MUST be first
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
#include "mimeentity.h"
#include "rfc822/address.h"
#include "wxMsFrameh.h"

#include <wx/stdstream.h>
#include <wx/wfstream.h>
#include "wxMsMessagePreview.h"

// ------------------------------------------------------------------
#if defined( _MSC_VER )
// only good for MSVC - see note above re __VISUALC__
// this block needs to AFTER all headers
# include <stdlib.h>
# include <crtdbg.h>
# ifdef _DEBUG
#   ifndef DBG_NEW
#     define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#     define new DBG_NEW
#   endif
# endif
#endif
// ------------------------------------------------------------------

wxMsMessagePreview::wxMsMessagePreview( MyFrame* parent)
    : wxMsMessagePreviewBase(parent)
{
  //my mods
  m_pParent = parent;
}

// ------------------------------------------------------------------

wxMsMessagePreview::~wxMsMessagePreview()
{
}

// ------------------------------------------------------------------

void wxMsMessagePreview::OnSaveMimeStruct2File(wxCommandEvent& event)
{
  wxString wsCaption;
  wxString wsWildcard = _T("Text files (*.txt)|*.txt|All files (*.*)|*.*");
  wxString wsDefaultDir;
  wxString wsDefaultFileName;
  wxFileDialog wfdlg( this, wsCaption, wsDefaultDir, wsDefaultFileName,
    wsWildcard, wxFD_SAVE );
  if ( wfdlg.ShowModal() == wxID_CANCEL)
    return;     // the user changed idea...
   // save the current contents in the file;
   // this can be done with e.g. wxWidgets output streams:

  wxFFileOutputStream file( wfdlg.GetPath() );
  wxStdOutputStream out( file );
  out << m_pParent->m_Pop3CommandData.sm_wsResultData;
  out.flush();
  if (!file.IsOk())
  {
    wxLogError("Cannot save current contents in file '%s'.", wfdlg.GetPath());
    return;
  }
}

// ------------------------------------------------------------------
/**
 * Save the MIME source to a user selected file.
 */

void wxMsMessagePreview::OnSaveMimeSource2File(wxCommandEvent& event)
{
  wxString wsCaption;
  wxString wsWildcard = _T("MSG files (*.msg)|*.msg|Text files (*.txt)|*.txt|All files (*.*)|*.*");
  wxString wsDefaultDir;
  wxString wsDefaultFileName;
  wxFileDialog wfdlg( this, wsCaption, wsDefaultDir, wsDefaultFileName,
    wsWildcard, wxFD_SAVE );
  if ( wfdlg.ShowModal() == wxID_CANCEL)
    return;     // the user changed idea...
   // save the current contents in the file;
  wxFFileOutputStream file( wfdlg.GetPath() );
  wxStdOutputStream out( file );
  out << m_pParent->m_Pop3CommandData.sm_wsResultData;
  out.flush();
  if (!file.IsOk())
  {
    wxLogError("Cannot save current contents in file '%s'.", wfdlg.GetPath());
    return;
  }
}

// ------------------------------------------------------------------
#include <wx/clipbrd.h>
/**
 * Copy all of the text from the text control to the clipboard.
 */
void wxMsMessagePreview::OnCopySource2Clipboard(wxCommandEvent& WXUNUSED(event) )
{
  wxString wsSource;
  if (wxTheClipboard->Open())
  {
    if (wxTheClipboard->IsSupported( wxDF_UNICODETEXT ))
    {
      wxTextDataObject data;
      int n = m_textCtrlMessageSource->GetNumberOfLines();
      int i;
      for( i = 0; i < n; i++ )
      {
        wsSource += m_textCtrlMessageSource->GetLineText( i ) + _T("\r\n");
      }
      if( wsSource.Length() )
      {
        wxTheClipboard->Clear();
        wxTheClipboard->SetData( new wxTextDataObject( wsSource ) );
        wxTheClipboard->Flush();
        // would need to translate all escape sequences
//        if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
//          wxLogMessage( wsSource );
      }
    }
    wxTheClipboard->Close();
  }
//  event.Skip();
}

// ------------------------------- eof ------------------------------
