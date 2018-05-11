/*-----------------------------------------------------------------
 * Name:         wxMsOptionsh.h
 * Purpose:
 * Author:      A. Wiegert
 *
 * Copyright:
 * Licence:     wxWidgets license
 *-------------------------------------------------------------- */
#ifndef _WX_MS_OPTIONSH_H_
#define _WX_MS_OPTIONSH_H_
#include "wxMsPreProcDefsh.h" // needs to be first
// ------------------------------------------------------------------
#include "wxMsOptionsDialogBase.h"
#include <list>
#include "wxMsFilter/wxMsFilterListh.h"
#include "wxMsConfh.h"

#include "wx/stdpaths.h"  // needed for wxStandardPaths - logs
#include "mimetic.h"
#include "utils.h"
using namespace std;
using namespace mimetic;

// ------------------------------------------------------------------

class wxMsOptionsDialog : public MyDialogOptionsBase
{
public:
  wxMsOptionsDialog(wxWindow* parent);
  ~wxMsOptionsDialog(){;}
  bool TransferDataToWindow();
  bool TransferDataFromWindow();
  
  // General panel
  void OnPickEmailClient(wxCommandEvent& event);

  // 'Log' panel event handlers
  void OnOptDlgLog_Log2FileClick( wxCommandEvent& event );
  void OnOptDlgLog_DefPathClick( wxCommandEvent& event );
  void OnOptDlgLog_SelLogFilePathClick( wxCommandEvent& event );

  // Filter dialog
  void OnFilterAdd(wxCommandEvent& event);
  void OnFilterEdit(wxCommandEvent& event);
  void OnFilterDelete(wxCommandEvent& event);
  void OnFilterUp(wxCommandEvent& event);
  void OnFilterDown(wxCommandEvent& event);
  void OnFilterListSelectionChanging(wxCommandEvent& event);
  void ExplainFilter( unsigned int a_uiIndex );
  void UpDateFilterList();

  wxString    m_wsDlgAppName;   // set once dialog is created
  iniPrefs_t  m_iniPrefs;
  wxWindow    *m_pParent;
};

#endif  // _WX_MS_OPTIONSH_H_
// ------------------------------- eof ---------------------------
