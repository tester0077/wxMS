//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxMsOptionsw.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "wxMsOptionsDialogBase.h"


// Declare the bitmap loading function
extern void wxC43D5InitBitmapResources();

static bool bBitmapLoaded = false;


MyDialogOptionsBase::MyDialogOptionsBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC43D5InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(bSizer1);
    
    m_notebookOptions = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_notebookOptions->SetName(wxT("m_notebookOptions"));
    
    bSizer1->Add(m_notebookOptions, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelGeneral = new wxPanel(m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebookOptions, wxSize(-1, -1)), wxTAB_TRAVERSAL);
    m_notebookOptions->AddPage(m_panelGeneral, _("General"), false);
    
    wxStaticBoxSizer* sbSizer1 = new wxStaticBoxSizer( new wxStaticBox(m_panelGeneral, wxID_ANY, _("General settings")), wxVERTICAL);
    m_panelGeneral->SetSizer(sbSizer1);
    
    wxFlexGridSizer* flexGridSizer18 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer18->SetFlexibleDirection( wxBOTH );
    flexGridSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    sbSizer1->Add(flexGridSizer18, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_checkBoxOptLaunchMailClient = new wxCheckBox(m_panelGeneral, wxID_ANY, _("Launch mail client after processing"), wxDefaultPosition, wxDLG_UNIT(m_panelGeneral, wxSize(-1,-1)), 0);
    m_checkBoxOptLaunchMailClient->SetValue(false);
    m_checkBoxOptLaunchMailClient->SetToolTip(_("Launch mail client after processing"));
    
    flexGridSizer18->Add(m_checkBoxOptLaunchMailClient, 0, wxALL, WXC_FROM_DIP(5));
    
    m_buttonPickMailClient = new wxButton(m_panelGeneral, wxID_ANY, _("..."), wxDefaultPosition, wxDLG_UNIT(m_panelGeneral, wxSize(-1,-1)), 0);
    m_buttonPickMailClient->SetToolTip(_("Pick the mail client to launch"));
    
    flexGridSizer18->Add(m_buttonPickMailClient, 0, 0, WXC_FROM_DIP(5));
    
    wxStaticBoxSizer* staticBoxSizer218 = new wxStaticBoxSizer( new wxStaticBox(m_panelGeneral, wxID_ANY, _("E-Mail Client:")), wxVERTICAL);
    
    sbSizer1->Add(staticBoxSizer218, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_textCtrlEmailClient = new wxTextCtrl(m_panelGeneral, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelGeneral, wxSize(-1,-1)), wxTE_READONLY);
    m_textCtrlEmailClient->SetToolTip(_("Path to e-mail client"));
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlEmailClient->SetHint(wxT(""));
    #endif
    
    staticBoxSizer218->Add(m_textCtrlEmailClient, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    wxFlexGridSizer* flexGridSizer220 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer220->SetFlexibleDirection( wxBOTH );
    flexGridSizer220->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    sbSizer1->Add(flexGridSizer220, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_checkBoxOptCheckMailAtStart = new wxCheckBox(m_panelGeneral, wxID_ANY, _("Check mail at startup"), wxDefaultPosition, wxDLG_UNIT(m_panelGeneral, wxSize(-1,-1)), 0);
    m_checkBoxOptCheckMailAtStart->SetValue(false);
    
    flexGridSizer220->Add(m_checkBoxOptCheckMailAtStart, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText20 = new wxStaticText(m_panelGeneral, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelGeneral, wxSize(-1,-1)), 0);
    
    flexGridSizer220->Add(m_staticText20, 0, wxALL, WXC_FROM_DIP(5));
    
    m_checkBoxOptPlaySound = new wxCheckBox(m_panelGeneral, wxID_ANY, _("Play sound when new mail arrives"), wxDefaultPosition, wxDLG_UNIT(m_panelGeneral, wxSize(-1,-1)), 0);
    m_checkBoxOptPlaySound->SetValue(false);
    m_checkBoxOptPlaySound->Enable(false);
    
    flexGridSizer220->Add(m_checkBoxOptPlaySound, 0, wxALL, WXC_FROM_DIP(5));
    
    m_buttonPickSound = new wxButton(m_panelGeneral, wxID_ANY, _("Select"), wxDefaultPosition, wxDLG_UNIT(m_panelGeneral, wxSize(-1,-1)), 0);
    m_buttonPickSound->SetToolTip(_("Pick the sound file to play"));
    m_buttonPickSound->Enable(false);
    
    flexGridSizer220->Add(m_buttonPickSound, 0, 0, WXC_FROM_DIP(5));
    
    wxStaticBoxSizer* staticBoxSizerMailCheck = new wxStaticBoxSizer( new wxStaticBox(m_panelGeneral, wxID_ANY, _("Mail Check")), wxHORIZONTAL);
    
    sbSizer1->Add(staticBoxSizerMailCheck, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_checkBoxScheduleMailCheck = new wxCheckBox(m_panelGeneral, wxID_ANY, _("Mail check every"), wxDefaultPosition, wxDLG_UNIT(m_panelGeneral, wxSize(-1,-1)), 0);
    m_checkBoxScheduleMailCheck->SetValue(false);
    
    staticBoxSizerMailCheck->Add(m_checkBoxScheduleMailCheck, 0, wxALL, WXC_FROM_DIP(5));
    
    m_spinCtrlMailCheckInterval = new wxSpinCtrl(m_panelGeneral, wxID_ANY, wxT("20"), wxDefaultPosition, wxDLG_UNIT(m_panelGeneral, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_spinCtrlMailCheckInterval->SetToolTip(_("Set the interval for mail checks"));
    m_spinCtrlMailCheckInterval->SetRange(1, 120);
    m_spinCtrlMailCheckInterval->SetValue(20);
    
    staticBoxSizerMailCheck->Add(m_spinCtrlMailCheckInterval, 0, 0, WXC_FROM_DIP(5));
    
    m_staticText36 = new wxStaticText(m_panelGeneral, wxID_ANY, _("minutes"), wxDefaultPosition, wxDLG_UNIT(m_panelGeneral, wxSize(-1,-1)), 0);
    m_staticText36->Enable(false);
    
    staticBoxSizerMailCheck->Add(m_staticText36, 0, wxALL, WXC_FROM_DIP(5));
    
    wxStaticBoxSizer* staticBoxSizerUpdateCheck = new wxStaticBoxSizer( new wxStaticBox(m_panelGeneral, wxID_ANY, _("Check for newer version at startup")), wxVERTICAL);
    
    sbSizer1->Add(staticBoxSizerUpdateCheck, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_checkBoxAutoUpdateCheck = new wxCheckBox(m_panelGeneral, wxID_ANY, _("Enable"), wxDefaultPosition, wxDLG_UNIT(m_panelGeneral, wxSize(-1,-1)), 0);
    m_checkBoxAutoUpdateCheck->SetValue(false);
    
    staticBoxSizerUpdateCheck->Add(m_checkBoxAutoUpdateCheck, 0, wxALL, WXC_FROM_DIP(5));
    
    wxStaticBoxSizer* staticBoxSizerTopLines = new wxStaticBoxSizer( new wxStaticBox(m_panelGeneral, wxID_ANY, _("Number of lines to retrieve in TOP request")), wxVERTICAL);
    
    sbSizer1->Add(staticBoxSizerTopLines, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_spinCtrlMaxTopLines = new wxSpinCtrl(m_panelGeneral, wxID_ANY, wxT("10"), wxDefaultPosition, wxDLG_UNIT(m_panelGeneral, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_spinCtrlMaxTopLines->SetToolTip(_("Number of messages lines to retrieve with TOP request (0- 300)"));
    m_spinCtrlMaxTopLines->SetRange(0, 300);
    m_spinCtrlMaxTopLines->SetValue(10);
    
    staticBoxSizerTopLines->Add(m_spinCtrlMaxTopLines, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    wxStaticBoxSizer* staticBoxSizerConnectCheck = new wxStaticBoxSizer( new wxStaticBox(m_panelGeneral, wxID_ANY, _("Connection Check")), wxHORIZONTAL);
    
    sbSizer1->Add(staticBoxSizerConnectCheck, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_checkBoxScheduleConnectCheck = new wxCheckBox(m_panelGeneral, wxID_ANY, _("Server check every"), wxDefaultPosition, wxDLG_UNIT(m_panelGeneral, wxSize(-1,-1)), 0);
    m_checkBoxScheduleConnectCheck->SetValue(false);
    m_checkBoxScheduleConnectCheck->SetToolTip(_("Interval for mail server connection check"));
    
    staticBoxSizerConnectCheck->Add(m_checkBoxScheduleConnectCheck, 0, wxALL, WXC_FROM_DIP(5));
    
    m_spinCtrlMailServerConectCheckInterval = new wxSpinCtrl(m_panelGeneral, wxID_ANY, wxT("10"), wxDefaultPosition, wxDLG_UNIT(m_panelGeneral, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_spinCtrlMailServerConectCheckInterval->SetToolTip(_("Set the interval for mail checks"));
    m_spinCtrlMailServerConectCheckInterval->SetRange(1, 120);
    m_spinCtrlMailServerConectCheckInterval->SetValue(10);
    
    staticBoxSizerConnectCheck->Add(m_spinCtrlMailServerConectCheckInterval, 0, 0, WXC_FROM_DIP(5));
    
    m_staticText364 = new wxStaticText(m_panelGeneral, wxID_ANY, _("minutes"), wxDefaultPosition, wxDLG_UNIT(m_panelGeneral, wxSize(-1,-1)), 0);
    m_staticText364->Enable(false);
    
    staticBoxSizerConnectCheck->Add(m_staticText364, 0, wxALL, WXC_FROM_DIP(5));
    
    m_panelLog = new wxPanel(m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebookOptions, wxSize(-1, -1)), wxTAB_TRAVERSAL);
    m_notebookOptions->AddPage(m_panelLog, _("Log"), false);
    
    wxStaticBoxSizer* sbSizer4 = new wxStaticBoxSizer( new wxStaticBox(m_panelLog, wxID_ANY, _("Logging Destination:")), wxVERTICAL);
    m_panelLog->SetSizer(sbSizer4);
    
    m_cbOptLogToFile = new wxCheckBox(m_panelLog, wxID_ANY, _("Log to file"), wxDefaultPosition, wxDLG_UNIT(m_panelLog, wxSize(-1, -1)), 0);
    m_cbOptLogToFile->SetValue(false);
    
    sbSizer4->Add(m_cbOptLogToFile, 0, wxALL, WXC_FROM_DIP(5));
    
    wxStaticBoxSizer* sbSizer5 = new wxStaticBoxSizer( new wxStaticBox(m_panelLog, wxID_ANY, _("Log file directory:")), wxVERTICAL);
    
    sbSizer4->Add(sbSizer5, 0, wxEXPAND, WXC_FROM_DIP(5));
    
    m_cbOptLogUseDefaultPath = new wxCheckBox(m_panelLog, wxID_ANY, _("Use default path"), wxDefaultPosition, wxDLG_UNIT(m_panelLog, wxSize(-1, -1)), 0);
    m_cbOptLogUseDefaultPath->SetValue(false);
    
    sbSizer5->Add(m_cbOptLogUseDefaultPath, 0, wxALL, WXC_FROM_DIP(5));
    
    wxStaticBoxSizer* sbSizer6 = new wxStaticBoxSizer( new wxStaticBox(m_panelLog, wxID_ANY, _("Path:")), wxHORIZONTAL);
    
    sbSizer5->Add(sbSizer6, 0, wxEXPAND, WXC_FROM_DIP(5));
    
    m_tcOptLogFilesDestDir = new wxTextCtrl(m_panelLog, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelLog, wxSize(-1, -1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_tcOptLogFilesDestDir->SetHint(wxT(""));
    #endif
    
    sbSizer6->Add(m_tcOptLogFilesDestDir, 1, wxALL, WXC_FROM_DIP(5));
    
    m_btnOptLogSelLogFilesDir = new wxButton(m_panelLog, wxID_ANY, _("..."), wxDefaultPosition, wxDLG_UNIT(m_panelLog, wxSize(-1, -1)), wxBU_EXACTFIT);
    
    sbSizer6->Add(m_btnOptLogSelLogFilesDir, 0, wxALL, WXC_FROM_DIP(5));
    
    wxStaticBoxSizer* sbSizer7 = new wxStaticBoxSizer( new wxStaticBox(m_panelLog, wxID_ANY, _("Verbosity")), wxHORIZONTAL);
    
    sbSizer4->Add(sbSizer7, 0, wxEXPAND, WXC_FROM_DIP(5));
    
    sbSizer7->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));
    
    m_sliderOptLogVerbosity = new wxSlider(m_panelLog, wxID_ANY, 0, 0, 5, wxDefaultPosition, wxDLG_UNIT(m_panelLog, wxSize(-1, -1)), wxSL_BOTTOM|wxSL_LABELS|wxSL_HORIZONTAL);
    
    sbSizer7->Add(m_sliderOptLogVerbosity, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    sbSizer7->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));
    
    wxStaticBoxSizer* sbSizer8 = new wxStaticBoxSizer( new wxStaticBox(m_panelLog, wxID_ANY, _("Log Options")), wxVERTICAL);
    
    sbSizer4->Add(sbSizer8, 0, wxEXPAND, WXC_FROM_DIP(5));
    
    wxBoxSizer* bSizer3 = new wxBoxSizer(wxHORIZONTAL);
    
    sbSizer8->Add(bSizer3, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText1 = new wxStaticText(m_panelLog, wxID_ANY, _("Delete files older than"), wxDefaultPosition, wxDLG_UNIT(m_panelLog, wxSize(-1, -1)), 0);
    
    bSizer3->Add(m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));
    
    m_spinCtrl1 = new wxSpinCtrl(m_panelLog, wxID_ANY, wxT("0"), wxDefaultPosition, wxDLG_UNIT(m_panelLog, wxSize(-1, -1)), wxSP_ARROW_KEYS);
    m_spinCtrl1->Enable(false);
    m_spinCtrl1->SetRange(0, 10);
    m_spinCtrl1->SetValue(0);
    
    bSizer3->Add(m_spinCtrl1, 0, wxALL|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));
    
    m_staticText2 = new wxStaticText(m_panelLog, wxID_ANY, _("days"), wxDefaultPosition, wxDLG_UNIT(m_panelLog, wxSize(-1, -1)), 0);
    
    bSizer3->Add(m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));
    
    m_checkBoxDailyLogFile = new wxCheckBox(m_panelLog, wxID_ANY, _("New Log file every day"), wxDefaultPosition, wxDLG_UNIT(m_panelLog, wxSize(-1, -1)), 0);
    m_checkBoxDailyLogFile->SetValue(false);
    m_checkBoxDailyLogFile->SetToolTip(_("Create a new log file every day.\nLog file name format wil be log_yyyy_mm_dd.txt"));
    m_checkBoxDailyLogFile->Enable(false);
    
    sbSizer4->Add(m_checkBoxDailyLogFile, 0, wxALL, WXC_FROM_DIP(5));
    
    m_panelFilters = new wxPanel(m_notebookOptions, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebookOptions, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebookOptions->AddPage(m_panelFilters, _("Filters"), false);
    
    wxBoxSizer* boxSizerFilters = new wxBoxSizer(wxHORIZONTAL);
    m_panelFilters->SetSizer(boxSizerFilters);
    
    wxStaticBoxSizer* staticBoxSizer50 = new wxStaticBoxSizer( new wxStaticBox(m_panelFilters, wxID_ANY, _("Filters")), wxVERTICAL);
    
    boxSizerFilters->Add(staticBoxSizer50, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    wxArrayString m_checkListBoxFilterArr;
    m_checkListBoxFilter = new wxCheckListBox(m_panelFilters, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelFilters, wxSize(-1,-1)), m_checkListBoxFilterArr, wxLB_SINGLE);
    
    staticBoxSizer50->Add(m_checkListBoxFilter, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizer62 = new wxBoxSizer(wxHORIZONTAL);
    
    staticBoxSizer50->Add(boxSizer62, 0, wxEXPAND, WXC_FROM_DIP(5));
    
    boxSizer62->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));
    
    m_buttonFilterPrioityUp = new wxButton(m_panelFilters, wxID_ANY, _("^"), wxDefaultPosition, wxDLG_UNIT(m_panelFilters, wxSize(-1,-1)), 0);
    m_buttonFilterPrioityUp->SetToolTip(_("Adjust selected filter  priority up"));
    
    boxSizer62->Add(m_buttonFilterPrioityUp, 0, 0, WXC_FROM_DIP(5));
    
    m_buttonFilterPriorityDown = new wxButton(m_panelFilters, wxID_ANY, _("v"), wxDefaultPosition, wxDLG_UNIT(m_panelFilters, wxSize(-1,-1)), 0);
    m_buttonFilterPriorityDown->SetToolTip(_("Adjust selected filter priority down"));
    
    boxSizer62->Add(m_buttonFilterPriorityDown, 0, 0, WXC_FROM_DIP(5));
    
    boxSizer62->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));
    
    m_textCtrlFilterDescription = new wxTextCtrl(m_panelFilters, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelFilters, wxSize(-1,-1)), wxTE_RICH2|wxTE_RICH|wxTE_READONLY|wxTE_MULTILINE);
    m_textCtrlFilterDescription->SetToolTip(_("Verbose description of filter action"));
    
    staticBoxSizer50->Add(m_textCtrlFilterDescription, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizer207 = new wxBoxSizer(wxVERTICAL);
    
    boxSizerFilters->Add(boxSizer207, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizerFilterButtons = new wxBoxSizer(wxVERTICAL);
    
    boxSizer207->Add(boxSizerFilterButtons, 0, wxALL, WXC_FROM_DIP(5));
    
    boxSizerFilterButtons->Add(0, 20, 1, wxALL, WXC_FROM_DIP(5));
    
    m_buttonFilterAdd = new wxButton(m_panelFilters, wxID_ANY, _("Add ..."), wxDefaultPosition, wxDLG_UNIT(m_panelFilters, wxSize(-1,-1)), 0);
    m_buttonFilterAdd->SetToolTip(_("Add a new filter"));
    
    boxSizerFilterButtons->Add(m_buttonFilterAdd, 0, wxALL, WXC_FROM_DIP(5));
    
    m_buttonFilterEdit = new wxButton(m_panelFilters, wxID_ANY, _("Edit ..."), wxDefaultPosition, wxDLG_UNIT(m_panelFilters, wxSize(-1,-1)), 0);
    m_buttonFilterEdit->SetToolTip(_("Edit selected filter"));
    
    boxSizerFilterButtons->Add(m_buttonFilterEdit, 0, wxALL, WXC_FROM_DIP(5));
    
    m_buttonFilterDelete = new wxButton(m_panelFilters, wxID_ANY, _("Delete"), wxDefaultPosition, wxDLG_UNIT(m_panelFilters, wxSize(-1,-1)), 0);
    m_buttonFilterDelete->SetToolTip(_("Delete selected filter"));
    
    boxSizerFilterButtons->Add(m_buttonFilterDelete, 0, wxALL, WXC_FROM_DIP(5));
    
    wxStaticBoxSizer* staticBoxSizer209 = new wxStaticBoxSizer( new wxStaticBox(m_panelFilters, wxID_ANY, _("Default Status Color")), wxVERTICAL);
    
    boxSizer207->Add(staticBoxSizer209, 0, wxALL, WXC_FROM_DIP(5));
    
    m_colourPickerStatusDefColor = new wxColourPickerCtrl(m_panelFilters, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDLG_UNIT(m_panelFilters, wxSize(-1,-1)), wxCLRP_DEFAULT_STYLE);
    
    staticBoxSizer209->Add(m_colourPickerStatusDefColor, 0, wxALL, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizer183 = new wxBoxSizer(wxHORIZONTAL);
    
    bSizer1->Add(boxSizer183, 0, wxEXPAND, WXC_FROM_DIP(5));
    
    boxSizer183->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));
    
    m_sdbSizer1 = new wxStdDialogButtonSizer();
    
    boxSizer183->Add(m_sdbSizer1, 0, wxALL, WXC_FROM_DIP(5));
    
    m_button190 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_sdbSizer1->AddButton(m_button190);
    
    m_button191 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_sdbSizer1->AddButton(m_button191);
    m_sdbSizer1->Realize();
    
    SetName(wxT("MyDialogOptionsBase"));
    SetMinClientSize(wxSize(-1,1));
    SetSize(-1,-1);
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
    // Connect events
    m_buttonPickMailClient->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnPickEmailClient), NULL, this);
    m_cbOptLogToFile->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnOptDlgLog_Log2FileClick), NULL, this);
    m_cbOptLogUseDefaultPath->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnOptDlgLog_DefPathClick), NULL, this);
    m_btnOptLogSelLogFilesDir->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnOptDlgLog_SelLogFilePathClick), NULL, this);
    m_checkListBoxFilter->Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(MyDialogOptionsBase::OnFilterListSelectionChanging), NULL, this);
    m_buttonFilterPrioityUp->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnFilterUp), NULL, this);
    m_buttonFilterPriorityDown->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnFilterDown), NULL, this);
    m_buttonFilterAdd->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnFilterAdd), NULL, this);
    m_buttonFilterEdit->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnFilterEdit), NULL, this);
    m_buttonFilterDelete->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnFilterDelete), NULL, this);
    
}

MyDialogOptionsBase::~MyDialogOptionsBase()
{
    m_buttonPickMailClient->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnPickEmailClient), NULL, this);
    m_cbOptLogToFile->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnOptDlgLog_Log2FileClick), NULL, this);
    m_cbOptLogUseDefaultPath->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnOptDlgLog_DefPathClick), NULL, this);
    m_btnOptLogSelLogFilesDir->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnOptDlgLog_SelLogFilePathClick), NULL, this);
    m_checkListBoxFilter->Disconnect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(MyDialogOptionsBase::OnFilterListSelectionChanging), NULL, this);
    m_buttonFilterPrioityUp->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnFilterUp), NULL, this);
    m_buttonFilterPriorityDown->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnFilterDown), NULL, this);
    m_buttonFilterAdd->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnFilterAdd), NULL, this);
    m_buttonFilterEdit->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnFilterEdit), NULL, this);
    m_buttonFilterDelete->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialogOptionsBase::OnFilterDelete), NULL, this);
    
}

MyDialogFilterEditBase::MyDialogFilterEditBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC43D5InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    wxBoxSizer* boxSizer85 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer85);
    
    m_panel199 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    
    boxSizer85->Add(m_panel199, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizer191 = new wxBoxSizer(wxVERTICAL);
    m_panel199->SetSizer(boxSizer191);
    
    wxFlexGridSizer* flexGridSizer91 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer91->SetFlexibleDirection( wxBOTH );
    flexGridSizer91->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    flexGridSizer91->AddGrowableCol(1);
    
    boxSizer191->Add(flexGridSizer91, 1, wxEXPAND, WXC_FROM_DIP(2));
    
    m_staticTextFilterName = new wxStaticText(m_panel199, wxID_ANY, _("Filter name"), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), 0);
    
    flexGridSizer91->Add(m_staticTextFilterName, 0, wxALL|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));
    
    m_textCtrlFilterEdit_FilterName = new wxTextCtrl(m_panel199, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), wxTE_RICH2|wxTE_RICH);
    m_textCtrlFilterEdit_FilterName->SetToolTip(_("NAme of the filter; cannot be blank."));
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlFilterEdit_FilterName->SetHint(wxT(""));
    #endif
    
    flexGridSizer91->Add(m_textCtrlFilterEdit_FilterName, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));
    
    m_staticTextFilterStatusText = new wxStaticText(m_panel199, wxID_ANY, _("Filter status text"), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), 0);
    
    flexGridSizer91->Add(m_staticTextFilterStatusText, 0, wxALL|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizer97 = new wxBoxSizer(wxHORIZONTAL);
    
    flexGridSizer91->Add(boxSizer97, 1, wxRIGHT|wxTOP|wxBOTTOM|wxEXPAND, WXC_FROM_DIP(5));
    
    m_textCtrlFilterEdit_FilterStatus = new wxTextCtrl(m_panel199, wxID_ANY, wxT("Filtered"), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), wxTE_RICH2|wxTE_RICH);
    m_textCtrlFilterEdit_FilterStatus->SetToolTip(_("Filter status; it will be shown in the 'Status' field.\nIt cannot be blank."));
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlFilterEdit_FilterStatus->SetHint(wxT(""));
    #endif
    
    boxSizer97->Add(m_textCtrlFilterEdit_FilterStatus, 1, wxALL, WXC_FROM_DIP(5));
    
    m_colourPickerFilterColor = new wxColourPickerCtrl(m_panel199, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), wxCLRP_DEFAULT_STYLE);
    m_colourPickerFilterColor->SetToolTip(_("Select the color for the status string."));
    
    boxSizer97->Add(m_colourPickerFilterColor, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticTextPriority = new wxStaticText(m_panel199, wxID_ANY, _("Filter priority"), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), 0);
    
    flexGridSizer91->Add(m_staticTextPriority, 0, wxALL, WXC_FROM_DIP(5));
    
    m_checkBoxFilterPriority = new wxCheckBox(m_panel199, wxID_ANY, _("This fileter takes priority over friends list"), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), 0);
    m_checkBoxFilterPriority->SetValue(false);
    m_checkBoxFilterPriority->Enable(false);
    
    flexGridSizer91->Add(m_checkBoxFilterPriority, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticTextFilterAction = new wxStaticText(m_panel199, wxID_ANY, _("Action"), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), 0);
    
    flexGridSizer91->Add(m_staticTextFilterAction, 0, wxALL|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizerFilterActions = new wxBoxSizer(wxVERTICAL);
    
    flexGridSizer91->Add(boxSizerFilterActions, 1, wxRIGHT|wxTOP|wxBOTTOM|wxEXPAND, WXC_FROM_DIP(5));
    
    m_checkBoxActionLegit = new wxCheckBox(m_panel199, wxID_ANY, _("Mail is legitimate"), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), 0);
    m_checkBoxActionLegit->SetValue(false);
    m_checkBoxActionLegit->SetToolTip(_("Mail is legitimate"));
    
    boxSizerFilterActions->Add(m_checkBoxActionLegit, 0, wxALL, WXC_FROM_DIP(5));
    
    m_checkBoxActionBlacklist = new wxCheckBox(m_panel199, wxID_ANY, _("Add to blacklist"), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), 0);
    m_checkBoxActionBlacklist->SetValue(false);
    m_checkBoxActionBlacklist->SetToolTip(_("Add sender to blacklist"));
    m_checkBoxActionBlacklist->Enable(false);
    
    boxSizerFilterActions->Add(m_checkBoxActionBlacklist, 0, wxALL, WXC_FROM_DIP(5));
    
    m_checkBoxActionMarkDel = new wxCheckBox(m_panel199, wxID_ANY, _("Mark for deletion"), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), 0);
    m_checkBoxActionMarkDel->SetValue(false);
    m_checkBoxActionMarkDel->SetToolTip(_("Mark message for deletion"));
    
    boxSizerFilterActions->Add(m_checkBoxActionMarkDel, 0, wxALL, WXC_FROM_DIP(5));
    
    m_checkBoxActionMarkBounce = new wxCheckBox(m_panel199, wxID_ANY, _("Mark for bouncing"), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), 0);
    m_checkBoxActionMarkBounce->SetValue(false);
    m_checkBoxActionMarkBounce->SetToolTip(_("Mark message for bouncing"));
    m_checkBoxActionMarkBounce->Enable(false);
    
    boxSizerFilterActions->Add(m_checkBoxActionMarkBounce, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText125 = new wxStaticText(m_panel199, wxID_ANY, _("\nApply filter when"), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), 0);
    
    flexGridSizer91->Add(m_staticText125, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_radioBoxFilterAndOrArr;
    m_radioBoxFilterAndOrArr.Add(_("all of the rules below are satisfied (AND)"));
    m_radioBoxFilterAndOrArr.Add(_("any of the rules are satisfied (OR)"));
    m_radioBoxFilterAndOr = new wxRadioBox(m_panel199, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), m_radioBoxFilterAndOrArr, 1, 0);
    m_radioBoxFilterAndOr->Enable(false);
    m_radioBoxFilterAndOr->SetSelection(0);
    
    flexGridSizer91->Add(m_radioBoxFilterAndOr, 0, wxALL, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizer87 = new wxBoxSizer(wxVERTICAL);
    
    boxSizer191->Add(boxSizer87, 0, 0, WXC_FROM_DIP(2));
    
    wxStaticBoxSizer* staticBoxSizerFilterRules = new wxStaticBoxSizer( new wxStaticBox(m_panel199, wxID_ANY, _("Rules")), wxVERTICAL);
    
    boxSizer87->Add(staticBoxSizerFilterRules, 0, wxEXPAND, WXC_FROM_DIP(2));
    
    wxGridSizer* gridSizer135 = new wxGridSizer(1, 3, 0, 0);
    
    staticBoxSizerFilterRules->Add(gridSizer135, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    wxArrayString m_choiceRuleSourceStringArr;
    m_choiceRuleSourceStringArr.Add(wxT("The 'From' field"));
    m_choiceRuleSourceStringArr.Add(wxT("The 'Subject' field"));
    m_choiceRuleSourceStringArr.Add(wxT("The 'To' field"));
    m_choiceRuleSourceString = new wxChoice(m_panel199, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), m_choiceRuleSourceStringArr, 0);
    m_choiceRuleSourceString->SetToolTip(_("Message source string to test"));
    
    gridSizer135->Add(m_choiceRuleSourceString, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    wxArrayString m_choiceRuleCriteriaArr;
    m_choiceRuleCriteriaArr.Add(wxT("contains"));
    m_choiceRuleCriteriaArr.Add(wxT("does not contain"));
    m_choiceRuleCriteriaArr.Add(wxT("is"));
    m_choiceRuleCriteriaArr.Add(wxT("contains Regex expression"));
    m_choiceRuleCriteriaArr.Add(wxT("does not contains Regex expression"));
    m_choiceRuleCriteria = new wxChoice(m_panel199, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), m_choiceRuleCriteriaArr, 0);
    m_choiceRuleCriteria->SetToolTip(_("Rule to apply"));
    
    gridSizer135->Add(m_choiceRuleCriteria, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_textCtrlRuleMsgText = new wxTextCtrl(m_panel199, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), 0);
    m_textCtrlRuleMsgText->SetToolTip(_("Text or regex to find"));
    #if wxVERSION_NUMBER >= 3000
    m_textCtrlRuleMsgText->SetHint(wxT(""));
    #endif
    
    gridSizer135->Add(m_textCtrlRuleMsgText, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizer143 = new wxBoxSizer(wxHORIZONTAL);
    
    staticBoxSizerFilterRules->Add(boxSizer143, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    boxSizer143->Add(0, 0, 1, 0, WXC_FROM_DIP(5));
    
    m_buttonFilterMoreRules = new wxButton(m_panel199, wxID_ANY, _("More rules"), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), 0);
    m_buttonFilterMoreRules->SetDefault();
    m_buttonFilterMoreRules->Enable(false);
    
    boxSizer143->Add(m_buttonFilterMoreRules, 0, wxRIGHT, WXC_FROM_DIP(5));
    
    m_buttonFilterFewerRules = new wxButton(m_panel199, wxID_ANY, _("Fewer rules"), wxDefaultPosition, wxDLG_UNIT(m_panel199, wxSize(-1,-1)), 0);
    m_buttonFilterFewerRules->Enable(false);
    
    boxSizer143->Add(m_buttonFilterFewerRules, 0, wxLEFT, WXC_FROM_DIP(5));
    
    boxSizer143->Add(0, 0, 1, 0, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizer177 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer85->Add(boxSizer177, 0, wxEXPAND, WXC_FROM_DIP(5));
    
    boxSizer177->Add(0, 0, 1, wxALL, WXC_FROM_DIP(5));
    
    m_stdBtnSizer127 = new wxStdDialogButtonSizer();
    
    boxSizer177->Add(m_stdBtnSizer127, 0, wxALL, WXC_FROM_DIP(5));
    
    m_buttonFilterOK = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer127->AddButton(m_buttonFilterOK);
    
    m_buttonFilterCancel = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer127->AddButton(m_buttonFilterCancel);
    m_stdBtnSizer127->Realize();
    
    SetName(wxT("MyDialogFilterEditBase"));
    SetSize(-1,-1);
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
    // Connect events
    m_colourPickerFilterColor->Connect(wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler(MyDialogFilterEditBase::OnColourChanged), NULL, this);
    m_checkBoxActionLegit->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MyDialogFilterEditBase::OnFilterEditIsLegitimate), NULL, this);
    
}

MyDialogFilterEditBase::~MyDialogFilterEditBase()
{
    m_colourPickerFilterColor->Disconnect(wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler(MyDialogFilterEditBase::OnColourChanged), NULL, this);
    m_checkBoxActionLegit->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MyDialogFilterEditBase::OnFilterEditIsLegitimate), NULL, this);
    
}
