//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxMsAboutDlg.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "wxMsAboutDlgBase.h"


// Declare the bitmap loading function
extern void wxCCE80InitBitmapResources();

static bool bBitmapLoaded = false;


wxMsAboutDlgBase::wxMsAboutDlgBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCCE80InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    wxBoxSizer* boxSizer3 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer3);
    
    wxBoxSizer* boxSizer5 = new wxBoxSizer(wxVERTICAL);
    
    boxSizer3->Add(boxSizer5, 0, wxEXPAND, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizer113 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer5->Add(boxSizer113, 0, wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_staticBitmapAbout = new wxStaticBitmap(this, wxID_STATIC, wxXmlResource::Get()->LoadBitmap(wxT("wxMS-1")), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0 );
    
    boxSizer113->Add(m_staticBitmapAbout, 0, wxRIGHT|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));
    
    boxSizer113->Add(0, 0, 1, 0, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizer11 = new wxBoxSizer(wxVERTICAL);
    
    boxSizer113->Add(boxSizer11, 0, wxLEFT, WXC_FROM_DIP(5));
    
    m_staticTextProgName = new wxStaticText(this, wxID_ANY, _("wxMS"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    wxFont m_staticTextProgNameFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Tahoma"));
    m_staticTextProgName->SetFont(m_staticTextProgNameFont);
    
    boxSizer11->Add(m_staticTextProgName, 0, wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_staticTextAppNameLine2 = new wxStaticText(this, wxID_ANY, _("Mail Screener Utility"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    boxSizer11->Add(m_staticTextAppNameLine2, 0, wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_staticTextCopyright = new wxStaticText(this, wxID_ANY, _("Copyright: A. Wiegert  %c %s"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    boxSizer11->Add(m_staticTextCopyright, 0, wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_staticTextVersion = new wxStaticText(this, wxID_ANY, _("Version:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    boxSizer11->Add(m_staticTextVersion, 0, wxLEFT|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_notebookData = new wxNotebook(this, ID_NOTEBOOK, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxBK_DEFAULT);
    m_notebookData->SetName(wxT("m_notebookData"));
    
    boxSizer3->Add(m_notebookData, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelDevelopers = new wxPanel(m_notebookData, ID_PANEL_DEVELOPERS, wxDefaultPosition, wxDLG_UNIT(m_notebookData, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebookData->AddPage(m_panelDevelopers, _("Developers"), false);
    
    wxBoxSizer* boxSizer23 = new wxBoxSizer(wxVERTICAL);
    m_panelDevelopers->SetSizer(boxSizer23);
    
    m_staticText25 = new wxStaticText(m_panelDevelopers, wxID_ANY, _("\nArnold Wiegert"), wxDefaultPosition, wxDLG_UNIT(m_panelDevelopers, wxSize(-1,-1)), 0);
    
    boxSizer23->Add(m_staticText25, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_panelArtists = new wxPanel(m_notebookData, ID_PANEL_ARTISTS, wxDefaultPosition, wxDLG_UNIT(m_notebookData, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebookData->AddPage(m_panelArtists, _("Artists"), false);
    
    wxBoxSizer* boxSizer29 = new wxBoxSizer(wxVERTICAL);
    m_panelArtists->SetSizer(boxSizer29);
    
    m_staticTextArtists = new wxStaticText(m_panelArtists, wxID_ANY, _("This program uses icons from the Gnome and Ubuntu projects."), wxDefaultPosition, wxDLG_UNIT(m_panelArtists, wxSize(-1,-1)), 0);
    
    boxSizer29->Add(m_staticTextArtists, 0, wxALL, WXC_FROM_DIP(5));
    
    m_panelCredits = new wxPanel(m_notebookData, ID_PANEL_CREDITS, wxDefaultPosition, wxDLG_UNIT(m_notebookData, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebookData->AddPage(m_panelCredits, _("Credits"), false);
    
    wxBoxSizer* boxSizer35 = new wxBoxSizer(wxVERTICAL);
    m_panelCredits->SetSizer(boxSizer35);
    
    m_staticText37 = new wxStaticText(m_panelCredits, wxID_ANY, _("wxMS uses the following software:"), wxDefaultPosition, wxDLG_UNIT(m_panelCredits, wxSize(-1,-1)), 0);
    
    boxSizer35->Add(m_staticText37, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_hyperLinkWxWidgets = new wxHyperlinkCtrl(m_panelCredits, ID_HYPERLINKCTRL_WXWIDGETS, _("the wxWidgets framework"), wxT("http://www.wxwidgets.org/"), wxDefaultPosition, wxDLG_UNIT(m_panelCredits, wxSize(-1,-1)), wxHL_DEFAULT_STYLE);
    m_hyperLinkWxWidgets->SetNormalColour(wxColour(wxT("#0000FF")));
    m_hyperLinkWxWidgets->SetHoverColour(wxColour(wxT("#0000FF")));
    m_hyperLinkWxWidgets->SetVisitedColour(wxColour(wxT("#FF0000")));
    
    boxSizer35->Add(m_hyperLinkWxWidgets, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_hyperLinkLibCurl = new wxHyperlinkCtrl(m_panelCredits, wxID_ANY, _("libCurl"), wxT("https://curl.haxx.se/libcurl/"), wxDefaultPosition, wxDLG_UNIT(m_panelCredits, wxSize(-1,-1)), wxHL_DEFAULT_STYLE);
    m_hyperLinkLibCurl->SetNormalColour(wxColour(wxT("#0000FF")));
    m_hyperLinkLibCurl->SetHoverColour(wxColour(wxT("#0000FF")));
    m_hyperLinkLibCurl->SetVisitedColour(wxColour(wxT("#FF0000")));
    
    boxSizer35->Add(m_hyperLinkLibCurl, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_hyperLinkMimeticLib = new wxHyperlinkCtrl(m_panelCredits, wxID_ANY, _("Mimetic Library"), wxT("http://www.codesink.org/mimetic_mime_library.html"), wxDefaultPosition, wxDLG_UNIT(m_panelCredits, wxSize(-1,-1)), wxHL_DEFAULT_STYLE);
    m_hyperLinkMimeticLib->SetNormalColour(wxColour(wxT("#0000FF")));
    m_hyperLinkMimeticLib->SetHoverColour(wxColour(wxT("#0000FF")));
    m_hyperLinkMimeticLib->SetVisitedColour(wxColour(wxT("#FF0000")));
    
    boxSizer35->Add(m_hyperLinkMimeticLib, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_panelLicense = new wxPanel(m_notebookData, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebookData, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebookData->AddPage(m_panelLicense, _("License"), false);
    
    wxBoxSizer* boxSizer51 = new wxBoxSizer(wxVERTICAL);
    m_panelLicense->SetSizer(boxSizer51);
    
    m_staticText131 = new wxStaticText(m_panelLicense, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelLicense, wxSize(-1,-1)), 0);
    
    boxSizer51->Add(m_staticText131, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText127 = new wxStaticText(m_panelLicense, wxID_ANY, _("This is open source software, distributed under the wxWidget license."), wxDefaultPosition, wxDLG_UNIT(m_panelLicense, wxSize(-1,-1)), 0);
    
    boxSizer51->Add(m_staticText127, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_staticText129 = new wxStaticText(m_panelLicense, wxID_ANY, _("See the include file wxMS-License.txt"), wxDefaultPosition, wxDLG_UNIT(m_panelLicense, wxSize(-1,-1)), 0);
    
    boxSizer51->Add(m_staticText129, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_panelSystemInfo = new wxPanel(m_notebookData, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebookData, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebookData->AddPage(m_panelSystemInfo, _("System"), false);
    
    wxBoxSizer* boxSizer63 = new wxBoxSizer(wxVERTICAL);
    m_panelSystemInfo->SetSizer(boxSizer63);
    
    wxStaticBoxSizer* staticBoxSizerBuild = new wxStaticBoxSizer( new wxStaticBox(m_panelSystemInfo, wxID_ANY, _("Build Information")), wxVERTICAL);
    
    boxSizer63->Add(staticBoxSizerBuild, 0, wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticTextwxWidgetsVersion = new wxStaticText(m_panelSystemInfo, wxID_ANY, _("test"), wxDefaultPosition, wxDLG_UNIT(m_panelSystemInfo, wxSize(-1,-1)), wxALIGN_CENTRE);
    
    staticBoxSizerBuild->Add(m_staticTextwxWidgetsVersion, 0, wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_staticTextLibcurlVersion = new wxStaticText(m_panelSystemInfo, wxID_ANY, _("text"), wxDefaultPosition, wxDLG_UNIT(m_panelSystemInfo, wxSize(-1,-1)), wxALIGN_CENTRE);
    
    staticBoxSizerBuild->Add(m_staticTextLibcurlVersion, 0, wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_staticTextMimeticVersion = new wxStaticText(m_panelSystemInfo, wxID_ANY, _("test"), wxDefaultPosition, wxDLG_UNIT(m_panelSystemInfo, wxSize(-1,-1)), wxALIGN_CENTRE);
    
    staticBoxSizerBuild->Add(m_staticTextMimeticVersion, 0, wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_staticTextCompiler = new wxStaticText(m_panelSystemInfo, wxID_ANY, _("text"), wxDefaultPosition, wxDLG_UNIT(m_panelSystemInfo, wxSize(-1,-1)), 0);
    
    staticBoxSizerBuild->Add(m_staticTextCompiler, 0, wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    wxStaticBoxSizer* staticBoxSizerOS = new wxStaticBoxSizer( new wxStaticBox(m_panelSystemInfo, wxID_ANY, _("System details")), wxVERTICAL);
    
    boxSizer63->Add(staticBoxSizerOS, 0, wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticTextOS = new wxStaticText(m_panelSystemInfo, wxID_ANY, _("test"), wxDefaultPosition, wxDLG_UNIT(m_panelSystemInfo, wxSize(-1,-1)), 0);
    
    staticBoxSizerOS->Add(m_staticTextOS, 0, wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    m_staticTextOsPort = new wxStaticText(m_panelSystemInfo, wxID_ANY, _("text"), wxDefaultPosition, wxDLG_UNIT(m_panelSystemInfo, wxSize(-1,-1)), 0);
    
    staticBoxSizerOS->Add(m_staticTextOsPort, 0, wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    wxStaticBoxSizer* staticBoxSizerConfigDir = new wxStaticBoxSizer( new wxStaticBox(m_panelSystemInfo, wxID_ANY, _("Configuration file")), wxVERTICAL);
    
    boxSizer63->Add(staticBoxSizerConfigDir, 0, wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticTextConfigDir = new wxStaticText(m_panelSystemInfo, wxID_ANY, _("test text"), wxDefaultPosition, wxDLG_UNIT(m_panelSystemInfo, wxSize(-1,-1)), wxALIGN_CENTRE);
    
    staticBoxSizerConfigDir->Add(m_staticTextConfigDir, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizer121 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer3->Add(boxSizer121, 1, wxEXPAND, WXC_FROM_DIP(5));
    
    wxBoxSizer* boxSizerOK = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer121->Add(boxSizerOK, 1, 0, WXC_FROM_DIP(5));
    
    boxSizerOK->Add(0, 0, 1, 0, WXC_FROM_DIP(5));
    
    m_buttonOK = new wxButton(this, wxID_OK, _("OK"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    boxSizerOK->Add(m_buttonOK, 0, wxBOTTOM, WXC_FROM_DIP(5));
    
    boxSizerOK->Add(0, 0, 1, 0, WXC_FROM_DIP(5));
    
    SetName(wxT("wxMsAboutDlgBase"));
    SetSize(-1,-1);
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
}

wxMsAboutDlgBase::~wxMsAboutDlgBase()
{
}
