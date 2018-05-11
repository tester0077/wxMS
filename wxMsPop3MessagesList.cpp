/*-----------------------------------------------------------------
 * Name:        wxMsPopMessagesList.cpp
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets license
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
#  endif
#endif
#include "wxMsPreProcDefsh.h"   // needs to be first
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
#include "wxMsFrameh.h"
#include <wx/grid.h>
#include <wx/gdicmn.h>
// ------------------------------------------------------------------
// Note __VISUALC__ is defined by wxWidgets, not by MSVC IDE
// and thus won't be defined until some wxWidgets headers are included
#if defined( _MSC_VER )
// this block needs to go AFTER all headers
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif
// ------------------------------------------------------------------
/**
 * Clear all data in the current message grid.
 */
void MyFrame::ClearMailGrid()
{
  m_gridMail->BeginBatch();
  // clear out all old data
  m_gridMail->ClearGrid();
  // and clear any existing rows
  if ( m_gridMail->GetNumberRows() )
    m_gridMail->DeleteRows( 0, m_gridMail->GetNumberRows() );
  m_gridMail->EndBatch();
}

// ------------------------------------------------------------------

/**
 * Display the data in the current message list.
 * Called when the display needs to be renewed, such as after a 
 * filter has changed.
 */
void MyFrame::DisplayMailList()
{
  wxString wsT;
  size_t j = 0;

  // clear out all old data
  m_gridMail->ClearGrid();
  // and clear any existing rows
  if ( m_gridMail->GetNumberRows() )
    m_gridMail->DeleteRows( 0, m_gridMail->GetNumberRows() );
  for (std::list<MyPop3MsgElement>::iterator it = m_Pop3MsgList.begin();
            it != m_Pop3MsgList.end(); ++it, j++ )
  {
    AddOneMessageRow( j );
    m_gridMail->Refresh();
    wxYield();
    ::wxMilliSleep( 10 );
  }
  wsT.Printf( _T(" %ld messages"), m_Pop3MsgList.size() );
  SetStatusText( wsT, 0 );
}

// ------------------------------------------------------------------
/**
 * Insert one row of data at top of the mail grid and set the 'action' check
 * boxes according the the values returned from the 'filter' code.
 * Note: at this time the data has been added to the message list. 
 *  All we need to do is to display the new row in the grid.
 */
void MyFrame::AddOneMessageRow( size_t szIndex  )
{
  bool bDelMessage = false;
  wxString wsStatus;
  wxColour wcColor;
  wxString wsColor;
  wxString wsT;
  size_t j = 0;
  {
    wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
    for (std::list<MyPop3MsgElement>::iterator it = m_Pop3MsgList.begin();
            it != m_Pop3MsgList.end(); ++it, j++ )
    {
      if ( j == szIndex )
      {
        GetMessageFilterStatus( szIndex, it, bDelMessage, wsStatus, wsColor );

        it->m_bDelete = bDelMessage;
        long lRow = szIndex;
        wxString wsSize;
        m_gridMail->BeginBatch();
        m_gridMail->InsertRows( lRow );

        m_gridMail->SetCellRenderer(  lRow, 0, new wxGridCellBoolRenderer);
        m_gridMail->SetCellEditor(    lRow, 0, new wxGridCellBoolEditor);
        m_gridMail->SetCellAlignment( lRow, 0, wxALIGN_CENTRE, wxALIGN_CENTRE );
        m_gridMail->SetCellValue(     lRow, 0, bDelMessage ? _T("1") : _T("0") );

        m_gridMail->SetCellRenderer(  lRow, 1, new wxGridCellBoolRenderer);
        m_gridMail->SetCellEditor(    lRow, 1, new wxGridCellBoolEditor);
        m_gridMail->SetCellAlignment( lRow, 1, wxALIGN_CENTRE, wxALIGN_CENTRE );

        m_gridMail->SetCellRenderer(  lRow, 2, new wxGridCellBoolRenderer);
        m_gridMail->SetCellEditor(    lRow, 2, new wxGridCellBoolEditor);
        m_gridMail->SetCellAlignment( lRow, 2, wxALIGN_CENTRE, wxALIGN_CENTRE );
        bool bRet = wxFromString( wsColor, &wcColor );
        if (!bRet)
        {
          wcColor =  wxTheColourDatabase->Find( wsColor );
          bRet = wcColor.IsOk();
        }
        m_gridMail->SetCellTextColour( lRow, 3, wcColor );
        m_gridMail->SetCellValue( lRow, 3, wsStatus );
        FormatSize( (double)(it->m_lSize), wsSize );
        if ( it->m_lSize == -1l )
          wsSize = wxEmptyString;
        m_gridMail->SetCellValue( lRow, 4, wsSize );
        m_gridMail->SetCellAlignment( lRow, 4, wxALIGN_CENTRE, wxALIGN_CENTRE );
        // from
        // display the UIDLs for now in the status field
    //    m_gridMail->SetCellValue( lRow, 3, it->m_wsUidl );
        // From
        m_gridMail->SetCellValue( lRow, 5, it->m_wsFrom );
        // subject
        m_gridMail->SetCellValue( lRow, 6, it->m_wsSubject );
        // To
        /**
         * Note: the message source seems to have the string
         * 'undisclosed recipients' but mimetic does not pass it on
         * and we have to make up for this
         */
        wsT = it->m_wsTo;
        if ( wsT.IsEmpty() )
           wsT = _("undisclosed recipients" );
        m_gridMail->SetCellValue( lRow, 7, wsT );
        // date
        m_gridMail->SetCellValue( lRow, 8, it->m_wsDateSent );
        // account
        m_gridMail->SetCellValue( lRow, 9, it->m_wsAcctName );
        m_gridMail->EndBatch();
        m_gridMail->Refresh();
      }
    }
  }
}

// ------------------------------------------------------------------
// Adapted from:
// http://www.codeproject.com/Articles/3275/Format-File-Sizes-in-Human-Readable-Format

size_t MyFrame::FormatSize(double dNumber, wxString &wsOutput )
{
  int iDepth = 0;
  wxString wsSuffix = _T("error");
  wxString wsFormat = _T("%.02f");

  double dResult = GetDepth(dNumber, iDepth);
  wsOutput.Printf( wsFormat, dResult);
  switch (iDepth)
  {
    case 0:
      wsSuffix = _T(" B");
      wsOutput.Printf( _T("%d"), (int)dResult);
      break;
    case 1:
      wsSuffix = _T(" KB");
      break;
    case 2:
      wsSuffix = _T(" MB");
      break;
    case 3:
      wsSuffix = _T(" GB");
      break;
    case 4:
      wsSuffix = _T(" TB");
      break;
    case 5:
      wsSuffix = _T(" PB");
      break;
    case 6:
      wsSuffix = _T(" EB");
      break;
  }
  wsOutput.Append( wsSuffix );
  return wsOutput.Length();
}

// ------------------------------------------------------------------

double MyFrame::GetDepth(double dNumber, int& iDepth)
{
  if (dNumber < 1024.0)
  {
    return dNumber;
  }
  else
  {
    return GetDepth(dNumber / 1024.0, ++iDepth);
  }
}

// ------------------------------- eof ------------------------------
