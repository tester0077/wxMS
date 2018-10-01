/*-----------------------------------------------------------------
 * Name:        wxMsCheckInternet.cpp
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
#include "wxMsh.h"
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
#if defined( WANT_MSVC_INTERNET_TEST )
bool MyApp::IsInternetAvailable()
{
  bool bIsOnline = wxGetApp().GetDialer()->IsOnline();
  return bIsOnline;
}
#else
#if defined( _MSC_VER )
/**
 * From: http://www.codeproject.com/Tips/67577/Check-for-an-active-Internet-connection.aspx
 * "I have seen many different ways to check if the computer has an active internet connection.
 * The best one, IMO, is to check for the presence of the default route in the IP forwarding
 * table that is maintained by windows.
 * This can be checked by using GetIPForwardTable function found in the iphlpapi.dll library.
 * The default route is present if one of the entries in the table has a forwarding destination
 * of 0.0.0.0."
 * I don't know how well this works as I have not really tested this to any useful degree
 */

#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi")

bool MyApp::IsInternetAvailable()
{
  bool bIsInternetAvailable = false;
  // Get the required buffer size
  DWORD dwBufferSize = 0;
  if (ERROR_INSUFFICIENT_BUFFER == GetIpForwardTable(NULL, &dwBufferSize, false))
  {
    BYTE *pByte = new BYTE[dwBufferSize];
    if (pByte)
    {
      PMIB_IPFORWARDTABLE pRoutingTable =
        reinterpret_cast<PMIB_IPFORWARDTABLE>(pByte);
      // Attempt to fill buffer with routing table information
      if (NO_ERROR == GetIpForwardTable(pRoutingTable, &dwBufferSize, false))
      {
        DWORD dwRowCount = pRoutingTable->dwNumEntries; // Get row count
        // Look for default route to gateway
        for (DWORD dwIndex = 0; dwIndex < dwRowCount; ++dwIndex)
        {
          if (pRoutingTable->table[dwIndex].dwForwardDest == 0)
          { // Default route designated by 0.0.0.0 in table
            bIsInternetAvailable = true; // Found it
            break; // Short circuit loop
          }
        }
      }
      delete[] pByte; // Clean up. Just say "No" to memory leaks
    }
  }
  return bIsInternetAvailable;
}
#endif
#endif  // new wxWidgets dialup manager test
// ------------------------------- eof ------------------------------
