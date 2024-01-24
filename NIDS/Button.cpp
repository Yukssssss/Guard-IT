#include "MyFrame.h"

// Définition de la table des événements pour le frame
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_BUTTON(ID_START_CAPTURE, MyFrame::OnStartCapture)
EVT_BUTTON(ID_STOP_CAPTURE, MyFrame::OnStopCapture)
EVT_LIST_ITEM_ACTIVATED(wxID_ANY, MyFrame::OnListItemActivated)
EVT_BUTTON(ID_CLEAR_LIST, MyFrame::OnClearList)
EVT_BUTTON(ID_SET_CAPTURE_FREQUENCY, MyFrame::OnSetCaptureFrequency)
EVT_CLOSE(MyFrame::OnClose)
EVT_SIZE(MyFrame::OnSize)
EVT_MENU(ID_TOGGLE_NIGHT_MODE, MyFrame::OnToggleNightMode)
EVT_MENU(wxID_HELP, MyFrame::OnShowHelp)
EVT_MENU(ID_NEW, MyFrame::OnNew)
EVT_MENU(ID_OPEN, MyFrame::OnOpen)
EVT_MENU(ID_SAVE, MyFrame::OnSave)
EVT_MENU(ID_SAVEAS, MyFrame::OnSaveAs)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
// Gestionnaire d'événement pour le menu "Edit"
EVT_MENU(ID_UNDO, MyFrame::OnUndo)
EVT_MENU(ID_REDO, MyFrame::OnRedo)
EVT_MENU(ID_CUT, MyFrame::OnCut)
EVT_MENU(ID_COPY, MyFrame::OnCopy)
EVT_MENU(ID_PASTE, MyFrame::OnPaste)
EVT_MENU(ID_SELECTALL, MyFrame::OnSelectAll)



// Gestionnaire d'événement pour le menu "Tools"
EVT_MENU(ID_OPTIONS, MyFrame::OnOptions)
EVT_MENU(ID_CUSTOMIZE, MyFrame::OnCustomize)

// Gestionnaire d'événement pour le menu "Help"

EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)

wxEND_EVENT_TABLE()

// Gestionnaire d'événement pour le bouton de démarrage de la capture
void MyFrame::OnStartCapture(wxCommandEvent& event)
{
    // Récupération des informations de l'interface réseau sélectionnée
    wxString selectedInterfaceDescription = interfaceChoice->GetStringSelection();
    wxString* selectedInterfaceName = static_cast<wxString*>(interfaceChoice->GetClientData(interfaceChoice->GetSelection()));
    wxString ipAddress = ipAddressInput->GetValue();

    // Vérification si l'adresse IP et l'interface réseau sont spécifiées
    if (selectedInterfaceDescription.empty() || ipAddress.empty()) {
        wxMessageBox("Please select a network interface and enter an IP address.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Désactivation du bouton "Start" pour éviter les clics multiples
    startButton->Disable();

    // Ouverture de la capture avec pcap_open_live
    char errbuf[PCAP_ERRBUF_SIZE];
    pcapHandle = pcap_open_live(selectedInterfaceName->ToStdString().c_str(), BUFSIZ, 1, 1000, errbuf);

    if (pcapHandle == nullptr) {
        wxMessageBox(wxString::Format("Error opening network interface: %s", errbuf), "Error", wxOK | wxICON_ERROR);
        // Réactivation du bouton "Start" en cas d'erreur
        startButton->Enable();
        return;
    }

    // Configuration du filtre BPF en fonction de l'adresse IP
    wxString filter = wxString::Format("host %s", ipAddress);

    struct bpf_program fp;
    if (pcap_compile(pcapHandle, &fp, filter.char_str(), 0, PCAP_NETMASK_UNKNOWN) == -1) {
        wxMessageBox("Error compiling filter", "Error", wxOK | wxICON_ERROR);
        pcap_close(pcapHandle);
        // Réactivation du bouton "Start" en cas d'erreur
        startButton->Enable();
        return;
    }

    if (pcap_setfilter(pcapHandle, &fp) == -1) {
        wxMessageBox("Error setting filter", "Error", wxOK | wxICON_ERROR);
        pcap_close(pcapHandle);
        // Réactivation du bouton "Start" en cas d'erreur
        startButton->Enable();
        return;
    }
    // Lancement du thread de capture
    stopCapture = false;
    captureThread = std::thread(&MyFrame::CaptureThreadFunction, this);

    UpdateStatus(true);
}


// Gestionnaire d'événement pour le bouton d'arrêt de la capture
void MyFrame::OnStopCapture(wxCommandEvent& event)
{
    // Arrêt de la capture en appelant pcap_breakloop et fermeture de la session pcap
    if (pcapHandle != nullptr) {
        stopCapture = true;
        pcap_breakloop(pcapHandle);
        pcap_close(pcapHandle);
        pcapHandle = nullptr;

    }

    // Attente de la fin du thread de capture
    if (captureThread.joinable()) {
        captureThread.join();
    }

    // Réactivation du bouton "Start"
    startButton->Enable();

    UpdateStatus(false);
}


// Gestionnaire d'événement pour le bouton de nettoyage de la liste
void MyFrame::OnClearList(wxCommandEvent& event)
{
    // Suppression de tous les éléments de la liste
    listCtrl->DeleteAllItems();
    UpdateThreatCount();
}


// Gestionnaire d'événement pour le bouton de modification de la fréquence de capture
void MyFrame::OnSetCaptureFrequency(wxCommandEvent& event)
{
    // Demande à l'utilisateur de saisir la nouvelle fréquence de capture
    wxString frequencyStr = wxGetTextFromUser("Enter capture frequency (in milliseconds):", "Set Capture Frequency", wxString::Format("%d", captureFrequency), this);

    // Vérifie si l'utilisateur a annulé la saisie
    if (frequencyStr.empty()) {
        return;
    }

    // Conversion de la chaîne en entier pour la nouvelle fréquence
    long newFrequency;
    if (frequencyStr.ToLong(&newFrequency) && newFrequency > 0) {
        // Mise à jour de la fréquence de capture avec la nouvelle valeur
        captureFrequency = static_cast<int>(newFrequency);
    }
    else {
        // Affichage d'un message d'erreur en cas de saisie invalide
        wxMessageBox("Invalid capture frequency. Please enter a positive integer.", "Error", wxOK | wxICON_ERROR);
    }
}

void MyFrame::OnClose(wxCloseEvent& event)
{
    // Vérifie si la capture est en cours
    if (pcapHandle != nullptr || captureThread.joinable()) {
        wxMessageBox("Please stop the capture before exiting.", "Error", wxOK | wxICON_ERROR);
        // Empêche la fermeture de l'application
        event.Veto();
    }
    else {
        int answer = wxMessageBox("Are you sure you want to exit?", "Confirmation", wxYES_NO | wxICON_QUESTION, this);

        if (answer == wxYES) {
            // Si l'utilisateur clique sur 'Yes', permet à l'application de se fermer
            event.Skip();
        }
        else {
            // Si l'utilisateur clique sur 'No', empêche la fermeture de l'application
            event.Veto();
        }
    }
}

void MyFrame::OnShowThreatCount(wxCommandEvent& event)
{
    UpdateThreatCount();
}

void MyFrame::UpdateThreatCount()
{
    // Ajoutez ici la logique pour calculer le nombre de menaces
    // Vous pouvez utiliser les données de la listeCtrl ou d'autres informations pertinentes

    // Par exemple, vous pouvez compter le nombre de lignes marquées comme menaces
    int threatCount = 0;
    for (long i = 0; i < listCtrl->GetItemCount(); i++) {
        wxString threatStatus = listCtrl->GetItemText(i, 8);  // Assurez-vous que la colonne 8 correspond à la colonne de menace
        if (threatStatus == "Suspect" || threatStatus == "Scan de port") {
            threatCount++;
        }
    }

    // Mettez à jour le texte de l'étiquette avec le nombre de menaces
    threatCountLabel->SetLabel(wxString::Format("Threat Count: %d", threatCount));
}