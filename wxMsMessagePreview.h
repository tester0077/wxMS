/*-----------------------------------------------------------------
 * Name:        xxxh.h
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets licence
 *-------------------------------------------------------------- */
#ifndef WXMSMESSAGEPREVIEW_H
#define WXMSMESSAGEPREVIEW_H
#include "wxMsBaseFrame.h"
// ------------------------------------------------------------------

class wxMsMessagePreview : public wxMsMessagePreviewBase
{
public:
  wxMsMessagePreview( MyFrame* parent);
  virtual ~wxMsMessagePreview();
//  bool TransferDataToWindow();
//  bool TransferDataFromWindow();

protected:
  MyFrame *m_pParent;
  void OnSaveMimeSource2File(wxCommandEvent& event);
  void OnSaveMimeStruct2File(wxCommandEvent& event);
};

#endif // WXMSMESSAGEPREVIEW_H

// ------------------------------- eof ---------------------------
