/*-----------------------------------------------------------------
 * Name:        wxMsLinkerTime.cpp
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
#if defined( WANT_EXE_LINK_DATE )
#include <wx/stdpaths.h>
#include "exeDosHdrh.h"
#endif
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
#if defined( WANT_EXE_LINK_DATE )
// ------------------------------------------------------------------
/**
 * RetrieveLinkerTimestamp - get the linker time stamp from the current executable.
 * Used to display the build date & copyright year in the 'About box.
 * The old way used the compile macro __DATE__, but that froze the build date
 * to the last time the file with the 'OnAbout() function was compiled,
 * not what I wanted.
 * Taken from:
http://www.codinghorror.com/blog/2005/04/determining-build-date-the-hard-way.html
 */

#define BYTE_BFR_SIZE 2048
#define PE_EXE_LINK_TIME  0x100

bool MyApp::RetrieveLinkerTimestamp( wxString& wsDate, wxString& wsYear )
{
  BYTE cByteBfr[BYTE_BFR_SIZE];
  size_t szBytesRead;
  long lSecondsSince1970;
  unsigned long ulNewHdrOffset;

  wsDate = wxEmptyString;
  wxString wsExePath = wxStandardPaths::Get().GetExecutablePath();
  wxFile wfExe( wsExePath );
  if ( !wfExe.IsOpened() )
    return false;
  szBytesRead = wfExe.Read( cByteBfr, BYTE_BFR_SIZE );
  if( szBytesRead < BYTE_BFR_SIZE )
    return false;
  DosExeHdr pDosExeHdr( cByteBfr, BYTE_BFR_SIZE );
  ulNewHdrOffset = pDosExeHdr.GetNewHdrOffset( cByteBfr );
  // get the EXE link date & time as seconds since Jan 1, 1970
  // located at NewHdrOffset + 8 bytes
  lSecondsSince1970 = *(long *)(cByteBfr + ulNewHdrOffset + 8);
  wxDateTime wdtExe( (time_t)lSecondsSince1970 );    // link time of .EXE
//  wxString wsIsoDate = wdtExe.FormatISODate();
//  wxString wsIsoTime = wdtExe.FormatISOTime();
  wsYear.Printf( _T("%d"), wdtExe.GetYear() );  // Copyright year
  // Note in wx 2.9.4 format "%e" - format day without leading '0' - bombs
  wsDate = wdtExe.Format( _T("%B %#d, %Y") );// _T("%B %e, %Y") );
  return true;
}

#endif  //#if defined( WANT_EXE_LINK_DATE )

// ------------------------------- eof ------------------------------
