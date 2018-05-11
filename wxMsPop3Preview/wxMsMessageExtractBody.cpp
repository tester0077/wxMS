/*-----------------------------------------------------------------
 * Name:        wxMsMessageExtractBody.cpp
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
#include "codec/rfc2047.h"
#include "rfc822/address.h"
#include "wxMsFrameh.h"
#include <wx/stdstream.h>
#include <wx/wfstream.h>
#include "codec/charsetconv.h"
#include "codec/rfc2231.h"
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

void MyFrame::MIMEExtractBody(mimetic::MimeEntity& entity, wxString& ar_wsOut, bool shall_recode)
{
   /* Try to get type */
   wxString encoding_type;
   if (entity.header().hasField("Content-Transfer-Encoding"))
   {
      encoding_type = wxString(entity.header().field("Content-Transfer-Encoding").value().c_str(), wxConvLocal);
   }

   /* Get charset */
   wxString charset = wxRfc2231::Decode(entity.header().contentType().paramList(),
                                        _T("charset"));

   /* Try identifying the type */
   if (encoding_type.CmpNoCase(_T("7bit")) == 0)
   {
      /* No need to perform conversion : everything is ASCII character... */
     ar_wsOut = entity.body();
   }
   else if (encoding_type.CmpNoCase(_T("8bit")) == 0)
   {
      /* Extract content */
      wxString result = wxCharsetConverter::ConvertCharset(wxString(entity.body().c_str(), wxConvLocal), charset);

      /* Send it to stream */
      ar_wsOut = result.mb_str(wxConvLocal);
   }
   else if (encoding_type.CmpNoCase(_T("base64")) == 0)
   {
      /* Instanciate decoder */
      mimetic::Base64::Decoder b64;

      if (shall_recode)
      {
         /* Decode base 64 data */
         std::vector<unsigned char> buffer;
         mimetic::decode(entity.body().begin(),
                         entity.body().end(),
                         b64,
                         std::back_inserter(buffer));
         buffer.push_back(0);

         /* Flush content in a string */
         wxString content((const char*)&buffer[0], wxConvLocal);

         /* Convert it to proper charset */
         wxString result = wxCharsetConverter::ConvertCharset(content, charset);

         /* Send it to stream */
         ar_wsOut = result.mb_str(wxConvLocal);
      }
      else
      {
         std::vector<unsigned char> buffer;

         mimetic::decode(entity.body().begin(),
                         entity.body().end(),
                         b64,
                         std::back_inserter(buffer));
         buffer.push_back(0);

         for (unsigned char* p = &buffer[0]; p != &buffer[buffer.size()-1]; p++)
         {
            ar_wsOut << *p;
         }
      }
   }
   else if (encoding_type.CmpNoCase(_T("quoted-printable")) == 0)
   {
      /* Instantiate decoder */
      mimetic::QP::Decoder qp;

      /* Decode base 64 data */
      std::vector<unsigned char> buffer;
      mimetic::decode(entity.body().begin(),
                      entity.body().end(),
                      qp,
                      std::back_inserter(buffer));
      buffer.push_back(0);

      /* Flush content in a string */
      wxString content;
      for (unsigned char* p = &buffer[0]; p != &buffer[buffer.size()-1]; p++)
      {
         content.Append(*p, 1);
      }

      /* Convert it to proper charset */
      wxString result = wxCharsetConverter::ConvertCharset(content, charset);

      /* Send it to stream */
      ar_wsOut = result;//.mb_str(wxConvLocal);
   }
   else if (encoding_type.CmpNoCase(_T("binary")) == 0)
   {
      /* Does not seem to be a valid encoding scheme in current MIME definition -> we will return data as is... */
      ar_wsOut = entity.body().c_str();
   }
   else if (encoding_type.CmpNoCase(_T("ietf-token")) == 0)
   {
      //TODO : to implement : ietf-token format
      ar_wsOut =  _T("Not supported type ") + encoding_type;
   }
   else if (encoding_type.CmpNoCase(_T("x-token")) == 0)
   {
      //TODO : to implement : x-token format
      ar_wsOut =  _T("Not supported type ") + encoding_type +_("!");
   }
   else
   {
      /* Not supported encoding type -> we will provide content as is as some e-mails are not properly formatted... */
      ar_wsOut =  entity.body();
   }
}

void MyFrame::MIMEExtractAll(mimetic::MimeEntity& entity, bool& found_plain_text_body, bool& found_html_body )
{
   mimetic::Header& h = entity.header();                   // get header object

   if (!h.contentType().isMultipart())
   {
      /* get type/subtype */
      wxString type_subtype = wxString(h.contentType().type().c_str(), wxConvLocal) << _T("/") <<
            wxString(h.contentType().subtype().c_str(), wxConvLocal);
      if ((type_subtype.CmpNoCase(_T("text/plain")) == 0) &&
          (!found_plain_text_body))
      {
         found_plain_text_body = true;
         wxString wsOut;
         MIMEExtractBody(entity, wsOut, true);
         m_wsTextBody = wsOut;
      }
      else if ((type_subtype.CmpNoCase(_T("text/html")) == 0) &&
               (!found_html_body))
      {
         found_html_body = true;
         wxString wsOut;
         MIMEExtractBody(entity, wsOut, true);

         m_has_html_alternative = true;
         m_wsHtmlBody = wsOut;
      }
      else
      {
         /* We will consider this as an attachment */
#if 1
        m_iNAttachments++;
#else
         /* retrieve attachment name */
         wxString original_name = wxRfc2231::Decode(entity.header().contentDisposition().paramList(),
                                                    _T("filename"));

         /* Get temporary file with this name */
         wxString temp_file_name = wxFileName::CreateTempFileName(original_name);

         /* Open the file as an std output stream */
         std::ofstream file_stream;
         file_stream.open((const char*)temp_file_name.mb_str(wxConvFile),
                          std::ios_base::out|
                             std::ios_base::trunc|
                             std::ios_base::binary);

         /* Extract content in file */
         MIMEExtractBody(entity, file_stream, false);

         /* Close the file */
         file_stream.close();

         /* flush attachment */
         AddAttachment(temp_file_name, original_name, 
           wxString(entity.header().contentId().str().c_str(), 
           wxConvLocal).AfterFirst('<').BeforeLast('>'));
#endif
      }
   }
   mimetic::MimeEntityList& parts = entity.body().parts(); // list of sub entities obj
   // cycle on sub entities list and print info of every item
   mimetic::MimeEntityList::iterator mbit = parts.begin(), meit = parts.end();
   for(; mbit != meit; ++mbit)
   {
      MIMEExtractAll(**mbit, found_plain_text_body, found_html_body);
   }
}


// ------------------------------- eof ------------------------------
