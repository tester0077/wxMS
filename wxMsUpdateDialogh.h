/*-----------------------------------------------------------------
 * Name:        wxMsUpdateDialogh.h
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWindows licence
 *-------------------------------------------------------------- */
#ifndef _WX_SCRAPE_UPDATE_DIALOG_H
#define _WX_SCRAPE_UPDATE_DIALOG_H

// ------------------------------------------------------------------
#include "wxMsUpdateCheckBase.h"

// ------------------------------------------------------------------
class MyCustomUpdateDialog : public MyUpdateCheckBase
{
public:

  MyCustomUpdateDialog( wxWindow* parent ) : MyUpdateCheckBase( parent )
  {};
  ~MyCustomUpdateDialog(){};
};

#endif  // _WX_SCRAPE_UPDATE_DIALOG_H
// ------------------------------- eof ---------------------------
