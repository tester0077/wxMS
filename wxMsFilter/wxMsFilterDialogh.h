/*-----------------------------------------------------------------
 * Name:        wxMsFilterh.h
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets license
 *-------------------------------------------------------------- */
#ifndef _WX_MS_OPTIONS_FILTER_H_
#define _WX_MS_OPTIONS_FILTER_H_

// ------------------------------------------------------------------
#include "wxMsPreProcDefsh.h" // needs to be first
// ------------------------------------------------------------------
#include "wxMsOptionsDialog/wxMsOptionsDialogBase.h"

class wxMsDialogFilterEdit : public MyDialogFilterEditBase
{
public:
  wxMsDialogFilterEdit(wxWindow* parent);
  ~wxMsDialogFilterEdit(){;}
  bool TransferDataToWindow();
  bool TransferDataFromWindow();
  void OnColourChanged(wxColourPickerEvent& event);
  void OnFilterEditIsLegitimate(wxCommandEvent& event);
  int  ShowEditFilterDialog();
  void SaveSize();
  wxWindow      *m_pParent;
  wxString      m_wsDlgAppName;
  iniPrefs_t    m_iniPrefs;
  bool          m_bEditing;
  wxString      m_wsSelection;
  wxColour      m_StatusColor;
  wxString      m_wsStatusColor;
};

#endif  // _WX_MS_OPTIONS_FILTER_H_
// ------------------------------- eof ---------------------------
