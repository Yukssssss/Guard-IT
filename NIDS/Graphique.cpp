#include "MyFrame.h"

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size), pcapHandle(nullptr), stopCapture(false), captureFrequency(1), nightMode(false)
{
    // Chargement de l'icône
    wxIcon icon;
    icon.LoadFile("../NIDS/icon.ico", wxBITMAP_TYPE_ICO);
    SetIcon(icon);

    // Création des éléments de l'interface utilisateur
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    interfaceChoice = new wxChoice(panel, ID_NETWORK_INTERFACE_CHOICE);
    ipAddressInput = new wxTextCtrl(panel, ID_IP_ADDRESS, wxEmptyString, wxDefaultPosition, wxDefaultSize);

    listCtrl = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    listCtrl->InsertColumn(1, "Packet #");
    listCtrl->InsertColumn(2, "Time");
    listCtrl->InsertColumn(3, "Length");
    listCtrl->InsertColumn(4, "Source IP");
    listCtrl->InsertColumn(5, "Destination IP");
    listCtrl->InsertColumn(6, "Source MAC");
    listCtrl->InsertColumn(7, "Destination MAC");
    listCtrl->InsertColumn(8, "Protocol");
    listCtrl->InsertColumn(9, "Menace");


    wxColour dayButtonColor = wxColour("#FFFFFF");   // Couleur claire pour le mode jour
    wxColour dayButtonTextColor = wxColour("#000000"); // Texte noir pour le mode jour

    wxColour nightButtonColor = wxColour("#333333"); // Couleur sombre pour le mode nuit
    wxColour nightButtonTextColor = wxColour("#FFFFFF"); // Texte blanc pour le mode n

    // Configuration des boutons et de la disposition
    startButton = new wxButton(panel, ID_START_CAPTURE, wxT("Start Capture"));
    stopButton = new wxButton(panel, ID_STOP_CAPTURE, wxT("Stop Capture"));
    clearButton = new wxButton(panel, ID_CLEAR_LIST, wxT("Clear List"));
    setFrequencyButton = new wxButton(panel, ID_SET_CAPTURE_FREQUENCY, wxT("Set Frequency"));
    showThreatCountButton = new wxButton(panel, wxID_ANY, wxT("Show Threat Count"));
    threatCountLabel = new wxStaticText(panel, wxID_ANY, wxT("Threat Count: 0"));

    wxGridSizer* buttonSizer = new wxGridSizer(2, 3, 5, 5);
    buttonSizer->Add(startButton, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(stopButton, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(clearButton, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(setFrequencyButton, 1, wxEXPAND | wxALL, 5);
    buttonSizer->Add(showThreatCountButton, 1, wxEXPAND | wxALL, 5);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(interfaceChoice, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(ipAddressInput, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(listCtrl, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(threatCountLabel, 0, wxEXPAND | wxALL, 5);


    showThreatCountButton->Bind(wxEVT_BUTTON, &MyFrame::OnShowThreatCount, this);

    wxFont smallFont = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    wxBoxSizer* footerSizer = new wxBoxSizer(wxHORIZONTAL);

    nameLabel = new wxStaticText(panel, wxID_ANY, wxT("Application: Guard IT"), wxDefaultPosition, wxDefaultSize, 0);
    nameLabel->SetFont(smallFont);
    footerSizer->Add(nameLabel, 0, wxEXPAND | wxALL, 5);

    versionLabel = new wxStaticText(panel, wxID_ANY, wxT("Version: 0.12"), wxDefaultPosition, wxDefaultSize, 0);
    versionLabel->SetFont(smallFont);
    footerSizer->Add(versionLabel, 0, wxEXPAND | wxALL, 5);

    // Add a spacer to push the statusLabel to the right
    footerSizer->AddStretchSpacer();

    statusLabel = new wxStaticText(panel, wxID_ANY, wxT("Status: Stopped"), wxDefaultPosition, wxDefaultSize, 0);
    statusLabel->SetFont(smallFont);
    UpdateStatus(false);
    footerSizer->Add(statusLabel, 0, wxEXPAND | wxALL, 5);

    mainSizer->Add(footerSizer, 0, wxEXPAND | wxALL, 5);

    panel->SetSizer(mainSizer);





    menuBar = new wxMenuBar;

    wxMenu* menuFile = new wxMenu;
    menuFile->Append(wxID_NEW, wxT("&New"));
    menuFile->Append(wxID_OPEN, wxT("&Open"));
    menuFile->Append(wxID_SAVE, wxT("&Save"));
    menuFile->Append(wxID_SAVEAS, wxT("Save &As"));
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, wxT("&Quit"));

    // Edit menu
    wxMenu* menuEdit = new wxMenu;
    menuEdit->Append(wxID_UNDO, wxT("&Undo"));
    menuEdit->Append(wxID_REDO, wxT("&Redo"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_CUT, wxT("Cu&t"));
    menuEdit->Append(wxID_COPY, wxT("&Copy"));
    menuEdit->Append(wxID_PASTE, wxT("&Paste"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_SELECTALL, wxT("Select &All"));

    // View menu
    wxMenu* menuView = new wxMenu;
    menuView->Append(ID_TOGGLE_NIGHT_MODE, wxT("Toggle Night Mode\tCtrl+N"));

    // Tools menu
    wxMenu* menuTools = new wxMenu;
    menuTools->Append(ID_OPTIONS, wxT("&Options"));
    menuTools->Append(ID_CUSTOMIZE, wxT("&Customize"));

    // Help menu
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_HELP, wxT("&Help"));
    menuHelp->Append(wxID_ABOUT, wxT("&About"));

    // Add menus to the menu bar
    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(menuEdit, wxT("&Edit"));
    menuBar->Append(menuView, wxT("&View"));
    menuBar->Append(menuTools, wxT("&Tools"));
    menuBar->Append(menuHelp, wxT("&Help"));

    // Assigner la barre de menus à la fenêtre principale
    SetMenuBar(menuBar);


    // Initialisation de la liste des interfaces réseau
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t* allDevs;
    if (pcap_findalldevs(&allDevs, errbuf) == 0) {
        // Remplissage du choix des interfaces réseau
        for (pcap_if_t* dev = allDevs; dev != nullptr; dev = dev->next) {
            wxString description = wxString::FromUTF8(dev->description);
            interfaceChoice->Append(description, new wxString(dev->name));
        }
        pcap_freealldevs(allDevs);
    }
    else {
        wxMessageBox(wxString::Format("Error finding network interfaces: %s", errbuf), "Error", wxOK | wxICON_ERROR);
    }

    Centre();
}
