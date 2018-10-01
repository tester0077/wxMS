/*-----------------------------------------------------------------
 * Name:        wxMsPop3MessagePreview.cpp
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
#include "rfc822/address.h"
#include "wxMsFrameh.h"
#include "wxMsExtendedAddressh.h"

#include <wx/stdstream.h>
#include <wx/wfstream.h>
#include "wxMsMessagePreview.h"
#include "wxMsPop3Preview/codec/rfc2047.h"
#include "wxMsPop3Preview/codec/rfc2231.h"

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
 * Called when user selects a message and selects
 * 'preview' from the context menu or
 * when a message is selected and user presses the space bar.
 */
void MyFrame::OnEmailPreview(wxCommandEvent& event)
{
  // Get selected row
  wxArrayInt waiRows = m_gridMail->GetSelectedRows();
  if ( (waiRows.GetCount() > 1) || ( waiRows.GetCount() == 0 )  )
    return;
  unsigned long lRow = waiRows[0];
  wxString wsAccount = m_gridMail->GetCellValue( lRow, 9 );
  PreviewMessage( wsAccount, lRow + 1 );
}

// ------------------------------------------------------------------
/**
 * Download the full message text, if it was not downlaoded previously
 * and display it.
 */
void MyFrame::PreviewMessage( wxString a_wsAccount, long a_lIndex )
{
  wxString wsT;
  MyAccountList::iterator iter;
  long j = 0;
  for ( iter = wxGetApp().m_AccountList.begin();
    iter != wxGetApp().m_AccountList.end(); ++iter )
  {
    MyMsAccountEl *pCurrent = *iter;
    if ( pCurrent->m_bEnabled && (a_wsAccount.IsSameAs( pCurrent->m_wsAcctName ) ) )
    {
#if 0
      // this causes interference since I have removed the mail server check 
      // may have to re-instae it in some way, without causing the lockup on Black
      if ( !pCurrent->m_bServerAvailable )
      {
        wsT.Printf( _("Mail server %s is currently not available."), a_wsAccount );
        wxMessageBox( wsT, _("Error"), wxOK | wxICON_HAND );
        return;
      }
#endif
      std::list<MyPop3MsgElement>::iterator it;
      // assume messages in list  are in same sequence as in mail grid
      for ( j = 1, it = m_Pop3MsgList.begin(); it != m_Pop3MsgList.end(); ++it, j++ )
      {
        if( j == a_lIndex )
        {
          if ( it->m_wsAcctName.IsSameAs( a_wsAccount ) )
          {
            // now get the full message data - if we haven't gotten it already
            if( it->m_wsMessage.IsEmpty() )
            {
              /* Instead of the message seqy=uence number, we pass in the UIDL
               * and then get the UIDLS of all messages on the server to find
               * the corresponding sequence number needed for the actual RETR
               * command.
               */
              {
                wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
                ClearPop3CommandData();     // make sure we use the old initial setup
                m_Pop3CommandData.sm_wsPop3ServerUrl = pCurrent->m_wsPopServerUrl;
                m_Pop3CommandData.sm_wsAccountName = pCurrent->m_wsAcctName;
                m_Pop3CommandData.sm_wsUserName = pCurrent->m_wsUserName;
                m_Pop3CommandData.sm_wsUserPassword = pCurrent->m_wsPassword;
                m_Pop3CommandData.sm_wsResultData.Clear();
                // no line feeds, no carriage returns
                wsT = it->m_wsUidl.AfterFirst( ' ' ); // strip the current sequence number
                m_Pop3CommandData.sm_wasCommands.Add( _T("RETR ") + wsT );
              }
              // display  a 'busy' notice
              {
                bool bLocalRun = true;
                wxWindowDisabler disableAll;
                wxBusyInfo wait("Downloading message data ...");
                wxASSERT( m_Pop3CommandData.sm_wasCommands.GetCount() );
                wxASSERT_MSG( !m_Pop3CommandData.sm_wsAccountName.IsEmpty(), m_Pop3CommandData.sm_wsAccountName );
                wxASSERT_MSG( !m_Pop3CommandData.sm_wsPop3ServerUrl.IsEmpty(), m_Pop3CommandData.sm_wsPop3ServerUrl );
                wxASSERT_MSG( !m_Pop3CommandData.sm_wsUserName.IsEmpty(), m_Pop3CommandData.sm_wsUserName );
                wxASSERT_MSG( !m_Pop3CommandData.sm_wsUserPassword.IsEmpty(), m_Pop3CommandData.sm_wsUserPassword );
                RunLibcurlThread();         // handle the rest via threads
                // need to wait to let libcurl download the headers
                while ( bLocalRun )
                {
                  {
                    wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
                    bLocalRun = m_bRunning;
                  }
#if defined( _MSC_VER )
                  ::wxYield();
#endif
                  ::wxMilliSleep( 100 );
                }
              }
              {
                wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
                it->m_wsMessage = m_Pop3CommandData.sm_wsResultData;
              }
            }
            // else we still have & use the data from last call
            // now we have the full message in m_Pop3CommandData.sm_wsResultData
            //m_Pop3CommandData.sm_wsResultData
            // handle the message header parsing here
            // Note: we could save the downloaded message text here to
            // it->m_wsMessage, but if the user were to preview several large
            // messages, such as the VTMS logs, it might not be the best way.
//            wxASSERT( !m_Pop3CommandData.sm_wsResultData.IsEmpty() );
            wxASSERT( !it->m_wsMessage.IsEmpty() );
            int w, h;
            wxMsMessagePreview dlg( this );
            {
              wxBusyInfo wait("Analyzing data ...");

//              std::string stream_str((const char*)m_Pop3CommandData.sm_wsResultData.mb_str(wxConvLocal));
              std::string stream_str((const char*)it->m_wsMessage.mb_str(wxConvLocal));

              // since the string as received contains \r\n as line terminators
              // and wxTextCtrl only wants \n, the original data adds extra blank lines to the output
              // strip them of with
              // from https://stackoverflow.com/questions/2528995/remove-r-from-a-string-in-c
              stream_str.erase( std::remove(stream_str.begin(), stream_str.end(), '\r'), stream_str.end() );
              std::istringstream istream(stream_str);
              mimetic::MimeEntity me(istream);

              if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
              {
                printMimeStructure(&me);
              }
              /* Extract all to addresses */
              m_toArray.clear();
              std::vector<mimetic::Address>::iterator to_it;
              for ( to_it = me.header().to().begin(); to_it != me.header().to().end(); to_it++)
              {
                wxString wsT = to_it->mailbox().mailbox();    // avoid lone '@'
                if ( !wsT.IsEmpty() )
                {
                  m_toArray.push_back(
                    ExtendedAddress(wxString(to_it->mailbox().mailbox().c_str(), wxConvLocal) +
                    _T("@")+wxString(to_it->mailbox().domain().c_str(), wxConvLocal),
                    wxRfc2047::Decode(wxString(to_it->mailbox().label().c_str(), wxConvLocal))));
                }
              }
              /* Extract all cc addresses */
              m_ccArray.clear();
              std::vector<mimetic::Address>::iterator cc_it;
              for ( cc_it = me.header().cc().begin(); cc_it != me.header().cc().end(); cc_it++)
              {
                wxString wsT = cc_it->mailbox().mailbox();
                if ( !wsT.IsEmpty() )
                {
                  m_ccArray.push_back(
                    ExtendedAddress(wxString(cc_it->mailbox().mailbox().c_str(), wxConvLocal) +
                    _T("@")+wxString(cc_it->mailbox().domain().c_str(), wxConvLocal),
                    wxRfc2047::Decode(wxString(cc_it->mailbox().label().c_str(), wxConvLocal))));
                }
              }
              /* Extract all bcc addresses */
              m_bccArray.clear();
              std::vector<mimetic::Address>::iterator bcc_it;
              for ( bcc_it = me.header().bcc().begin(); bcc_it != me.header().bcc().end(); bcc_it++)
              {
                wxString wsT = bcc_it->mailbox().mailbox();
                if ( !wsT.IsEmpty() )
                {
                  m_bccArray.push_back(
                    ExtendedAddress(wxString(bcc_it->mailbox().mailbox().c_str(),
                    wxConvLocal)+_T("@") +
                    wxString(bcc_it->mailbox().domain().c_str(), wxConvLocal),
                    wxRfc2047::Decode(wxString(bcc_it->mailbox().label().c_str(), 
                    wxConvLocal))));
                }
              }
              wxString wsT1;
              ContentType ct = me.header().contentType();
              wxString wsContentType = ct.type();
              wxString wsContentSubType = ct.subtype();
              wxString wsCharset = wxRfc2231::Decode(me.header().contentType().paramList(),
                                        _T("charset"));
              if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 1 )
              {
                wxLogMessage( _("Content type: %s, subtype: %s"), wsContentType, wsContentSubType );
              }
              dlg.GetStaticTextDataFrom()->SetLabel( it->m_wsFrom );
              dlg.GetStaticTextDataDate()->SetLabel( it->m_wsDateSent );
#if defined( _DEBUG )
              unsigned long nb_elements = m_toArray.size();
              nb_elements++;
              nb_elements--;  // try to keep GCC happy
#endif
              wsT = wxEmptyString;
              std::list<ExtendedAddress>::const_iterator itAdr;
              for ( itAdr = m_toArray.begin(); itAdr != m_toArray.end(); itAdr++)
              {
                if ( itAdr->HasName() )
                {
                  wsT1 = itAdr->GetName();
                }
                wsT += itAdr->GetExtendedAddress();
                if ( itAdr != m_toArray.end() )
                  wsT +=  _T(";");
              }
              wsT = wsT.BeforeLast( ';' );
              /**
                * Note: the message source seems to have the string
                * 'undisclosed recipients' but mimetic does not pass it on
                * and we have to make up for this
                */
              if ( wsT.IsEmpty() )
                wsT = _("undisclosed recipients" );
              dlg.GetStaticTextDataTo()->SetLabel( wsT );
              dlg.GetStaticTextDataSubject()->SetLabel( it->m_wsSubject );
              if( !wsCharset.IsEmpty() )
                wsT = _T(" : ") + wsCharset;
              else
                wsT = wxEmptyString;
              dlg.GetStaticTextMsgType()->SetLabel( wsContentType + _T("/") + wsContentSubType +  wsT );

              dlg.SetSize( g_iniPrefs.data[IE_PREVIEW_DLG_W].dataCurrent.lVal,
                g_iniPrefs.data[IE_PREVIEW_DLG_H].dataCurrent.lVal );

              // display the data in the 'source' window of the preview dialog
              printMimeStructure( &me, 0, dlg.GetTextCtrlMimeStructure() );
              m_found_plain_text_body = false;
              m_found_html_body = false;
              m_iNAttachments = 0;

              std::string stream_str1((const char*)it->m_wsMessage.mb_str(wxConvLocal));
              dlg.GetTextCtrlMessageSource()->AppendText((const char*)it->m_wsMessage.mb_str(wxConvLocal));
              // since the string as received contains \r\n as line terminators
              // and wxTextCtrl only wants \r, the original data contains extra blank lines to the output
              // strip them of with
              // from https://stackoverflow.com/questions/2528995/remove-r-from-a-string-in-c
              stream_str.erase( std::remove(stream_str.begin(), stream_str.end(), '\r'), stream_str.end() );
              std::istringstream istream1(stream_str);
              mimetic::MimeEntity me1(istream1);
              MIMEExtractAll( me1, m_found_plain_text_body, m_found_html_body );
              if ( m_found_plain_text_body )
              {
                dlg.GetTextCtrlMessage()->AppendText( m_wsTextBody );
              }
              if ( m_found_html_body )
              {
                dlg.GetHtmlWinHtml()->SetPage( m_wsHtmlBody );
              }
              else
              {
                dlg.GetHtmlWinHtml()->SetPage( _T("<html><body>no HTML content</body></hmtl>") );
                // show the plain text page if there is no HTML to show
                dlg.GetNotebookMessagePreview()->SetSelection( 0 );
              }
            }
            if ( m_iNAttachments )
            {
              wsT.Printf( _("%d"), m_iNAttachments );
            }
            else
            {
              wsT = wxEmptyString;
            }
            dlg.GetStaticTextAttachments()->SetLabel( wsT );
            dlg.ShowModal();
            dlg.GetSize( &w, &h );
            g_iniPrefs.data[IE_PREVIEW_DLG_W].dataCurrent.lVal = w;
            g_iniPrefs.data[IE_PREVIEW_DLG_H].dataCurrent.lVal = h;
            return;
          }
        }
      }
    }
  }
}

// ------------------------------- eof ------------------------------
