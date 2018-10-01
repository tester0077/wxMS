/*-----------------------------------------------------------------
 * Name:        wxMsPop3AdvancedOptionsDlg.cpp
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
#include "wxMsh.h"
//#include "wxMsAccounth.h"
#include "wxMsPop3AdvancedOptionsDlgh.h"
//#include "wxMsAccountDetailsh.h"

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
// Constructor
MyAdvancedPopConfigDlg::MyAdvancedPopConfigDlg( MyAccountsDetailsDialog* parent ) :
    MyBaseAdvancedPopConfigDlg( parent  )
{
  m_parent = parent;
}

//------------------------------------------------------------------
// Called automagically by InitDialog() to set up the data
// prior to the dialog being shown
// Note since this overrides a predefined function, it MUST have
// the name and return type as given here
/**
  * For now set the first account data.
  * Down the road, we will have to save the last used account
  * and display its data
  */
bool MyAdvancedPopConfigDlg::TransferDataToWindow( void )
{
  Fit();
  return true;
}
//------------------------------------------------------------------
// Called automagically when dialog is dismissed via OK button
// Note since this overrides a predefined function, it MUST have
// the name and return type as given here

bool MyAdvancedPopConfigDlg::TransferDataFromWindow( void )
{
  return true;
}

// ------------------------------- eof ------------------------------
