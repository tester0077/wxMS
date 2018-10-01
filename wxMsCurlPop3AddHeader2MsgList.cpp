/*-----------------------------------------------------------------
 * Name:        wxMsCurlPop3AddHeader2MsgList.cpp
 * Purpose:     Add a header retrieved from POP3 server to message list.
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
#include "wxMsFrameh.h"
#include "wxMsPop3Preview/codec/rfc2047.h"
#include "wxMsPop3Preview/codec/rfc2231.h"

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
class wxRfc2047;
/**
 * Called from the thread event code used to update the mail grid display
 * whenever a new header has been downloaded from the mail server.
 */
// ------------------------------------------------------------------
void MyFrame::Add1Pop3Header2MsgList( size_t a_szIndex, wxString a_wsAccount )
{
  wxString wsT;
  wxString wsT1;
  wxString wsT2;
  size_t j = 0;
  {
    wxCriticalSectionLocker lock(m_CS_Pop3MsgList);
    for (std::list<MyPop3MsgElement>::iterator it = m_Pop3MsgList.begin();
            it != m_Pop3MsgList.end(); ++it, j++ )
    {
      wsT2 = wxEmptyString;
      if ( j == a_szIndex )
      {
        // handle the header parsing here
        wxString wsHeader = it->m_wsHeader;
        if ( wsHeader.StartsWith( _T("+OK"))) // strip server response, if any
          wsHeader = wsHeader.AfterFirst( '\n' ); 
        std::istringstream ss(std::string( wsHeader.mb_str(), wsHeader.Len() ) );
        MimeEntity me(ss);
        if ( g_iniPrefs.data[IE_LOG_VERBOSITY].dataCurrent.lVal > 4 )
        {
          printMimeStructure(&me);
        }
        it->m_wsAcctName = a_wsAccount;
        // extract the header data of interest and
        // setup the corresponding fields in the message list
        wxString charset = wxRfc2231::Decode(me.header().contentType().paramList(),
                                        _T("charset"));
        ContentType ct = me.header().contentType();
        it->m_wsContentType = ct.type();
        it->m_wsContentSubType = ct.subtype();
        it->m_wsCharSet = charset;
        /* Extract all to addresses */
        std::vector<mimetic::Address>::iterator to_it;
        it->m_wsTo = wxEmptyString;
        for ( to_it = me.header().to().begin(); to_it != me.header().to().end(); to_it++)
        {
           wxString wsT = to_it->mailbox().mailbox();    // avoid lone '@'
          if ( !wsT.IsEmpty() )
          {
            it->m_wsTo += wxString(to_it->mailbox().mailbox().c_str(), wxConvLocal)+_T("@") +
              wxString(to_it->mailbox().domain().c_str(), wxConvLocal) ;
            m_toArray.push_back(ExtendedAddress(wxString(to_it->mailbox().mailbox().c_str(), wxConvLocal) +
              _T("@")+wxString(to_it->mailbox().domain().c_str(), wxConvLocal),
                          wxRfc2047::Decode(wxString(to_it->mailbox().label().c_str(), wxConvLocal))));
          }
        }

        /* Extract all cc addresses */
        std::vector<mimetic::Address>::iterator cc_it;
        for ( cc_it = me.header().cc().begin(); cc_it != me.header().cc().end(); cc_it++)
        {
          m_ccArray.push_back(ExtendedAddress(wxString(cc_it->mailbox().mailbox().c_str(), wxConvLocal)+_T("@")+wxString(cc_it->mailbox().domain().c_str(), wxConvLocal),
                        wxRfc2047::Decode(wxString(cc_it->mailbox().label().c_str(), wxConvLocal))));
        }

        /* Extract all bcc addresses */
        std::vector<mimetic::Address>::iterator bcc_it;
        for ( bcc_it = me.header().bcc().begin(); bcc_it != me.header().bcc().end(); bcc_it++)
        {
          m_bccArray.push_back(ExtendedAddress(wxString(bcc_it->mailbox().mailbox().c_str(), wxConvLocal)+_T("@")+wxString(bcc_it->mailbox().domain().c_str(), wxConvLocal),
                          wxRfc2047::Decode(wxString(bcc_it->mailbox().label().c_str(), wxConvLocal))));
        }
        Header::iterator hbit, heit;
        hbit = me.header().begin();
        heit = me.header().end();
        for(; hbit != heit; ++hbit)
        {
          wsT1 = hbit->name();
          wsT2 = hbit->value();
          if( wsT1.MakeUpper().IsSameAs( _T("FROM")) )
          {
#if 0
            if( wsT2.StartsWith( _T("\"" ) ) )
            {
              wsT2 = wsT2.Mid( 1 );
              wsT2.RemoveLast();
            }
#endif
            it->m_wsFrom = wxRfc2047::Decode( wsT2 );
          }
          else if (  wsT1.MakeUpper().IsSameAs( _T("TO")) )
          {
            it->m_wsTo =  wxRfc2047::Decode( wsT2 );
          }
          else if (  wsT1.MakeUpper().IsSameAs( _T("DATE")) )
          {
            // decode, just in case, then clean up
            wsT2 =  wxRfc2047::Decode( wsT2 );
            // typical values:
            // L"Tue, 06 Feb 2018 01:51:20 -0500"
            // L"Mon, 12 Feb 2018 22:48:17 +0100 (CET)"
            // but we can also get 
            // L"Wed, 04 Apr 2018 11:15:05 -0600" or
            // L"Wed, 04 Apr 2018 10:57:13 (Pacific Daylight Time)"
            // strip off trailer(s)
            wsT2 = wsT2.BeforeFirst( '(' );
            wsT2.Trim();      // clean up the posible trailing space
            wsT2 = wsT2.BeforeLast( ' ' );
            // and strip off day of the week with:
            it->m_wsDateSent = wsT2.AfterFirst( ' ' );
          }
          else if (  wsT1.MakeUpper().IsSameAs( _T("SUBJECT")) )
          {
            if( it->m_wsCharSet.IsSameAs( _T("UTF-8")))
            {
              wxString wsTT = wxString::FromUTF8( wsT2 );
              wsT2 = wsTT;
            }
            it->m_wsSubject =  wxRfc2047::Decode( wsT2 );
          }
//          else if ( wsT1.IsSameAs( _T("REPLY_TO")))
//          {
//            wxString wsTT = wsT2;
//          }
        }
        break;
      }
    }
  }
}

// ------------------------------- eof ------------------------------
