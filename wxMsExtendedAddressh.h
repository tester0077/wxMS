/*-----------------------------------------------------------------
 * Name:        wxMsExtendedAddressh.h
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets licence
 *-------------------------------------------------------------- */
#ifndef _WX_MS_EXTENDED_ADDRESS_H
#define _WX_MS_EXTENDED_ADDRESS_H
#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
// ------------------------------------------------------------------
/**
 * Borrowed from Andre Brice, borrowd from Frank Buﬂ
 */
// ------------------------------------------------------------------
/*!
  * This class represents an e-mail address.
  *
  * It contains two fields :
  *    \li \c address The mail address (ex. name@domain.com). This field is mandatory.
  *    \li \c name The name of the address proprietary (ex. John Doe). This field is optional
  */
class ExtendedAddress
{
    public:
      /*!
        * Default constructor
        *
        * \param address The mail address
        * \param name The name of the owner of the address
        * \todo it should be nice to check address format...
        */
      ExtendedAddress(const wxString& address = _T(""), const wxString& name = _T("")) : address(address), name(name) {}

      /*!
        * Retrieves the mail address
        *
        * \return The mail address
        */
      const wxString& GetExtendedAddress() const {return address;}

      /*!
        * Retrieves the name of the owner of the address the address
        *
        * \return The name of the owner of the address, empty if not set
        */
      const wxString& GetName() const {return name;}

      /*!
        * Allows determining if a name exists
        *
        * \return true if a name exists, false otherwise
        */
      bool HasName() const {return !name.IsEmpty();}

      /*!
        * \internal
        *
        * Returns an encoded MIME mail address
        * \return the MIME encoded mail address
        * \todo We should add coding
        */
      wxString GetMimeStr() const
      {
          if (HasName())
          {
            return name + _T(" <") + address + _T(">");
          }
          else
          {
            return _T("<") + address + _T(">");
          }
      }

    private:

      wxString address;
      wxString name;
};

#endif  // _WX_MS_EXTENDED_ADDRESS_H
// ------------------------------- eof ---------------------------

