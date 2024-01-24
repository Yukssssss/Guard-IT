#include "MyFrame.h"

void MyFrame::OnShowHelp(wxCommandEvent& event)
{
    wxString helpText =
        "Guide de Démarrage Rapide pour l'Application Guard IT\n\n"
        "Bienvenue dans Guard IT, une application de surveillance réseau simple mais puissante. "
        "Suivez ces étapes pour utiliser les fonctionnalités principales de l'application.\n\n"
        "1 - Sélectionner la carte réseau\n"
        "Au lancement de l'application, vous verrez une liste déroulante appelée \"Choix de l'interface réseau\". "
        "Sélectionnez la carte réseau que vous souhaitez surveiller dans cette liste.\n\n"
        "2 - Entrer l'adresse IP\n"
        "Juste en dessous de la liste déroulante, vous trouverez une zone de texte intitulée \"Adresse IP\". "
        "Entrez l'adresse IP que vous souhaitez filtrer ou surveiller.\n\n"
        "3 - Démarrer la capture\n"
        "Cliquez sur le bouton \"Start Capture\" pour commencer la surveillance du trafic sur la carte réseau "
        "sélectionnée avec le filtre d'adresse IP spécifié.\n\n"
        "4 - Arrêter la capture\n"
        "À tout moment, vous pouvez cliquer sur le bouton \"Stop Capture\" pour arrêter la surveillance du trafic.\n\n"
        "5 - Observer le trafic en temps réel\n"
        "La liste déroulante \"Traffic Log\" affiche les informations sur le trafic en temps réel. Chaque ligne "
        "représente un paquet capturé, avec des détails tels que le numéro de paquet, l'heure, la longueur, les adresses "
        "IP source et de destination, etc.\n\n"
        "6 - Filtrer par protocole\n"
        "Les colonnes \"Protocole\" et \"Menace\" vous indiquent le protocole du paquet et s'il est considéré comme une "
        "menace. Les paquets suspectés de scan de port ou de taille anormale seront mis en évidence.\n\n"
        "7 - Détails des paquets\n"
        "Double-cliquez sur une ligne dans la liste pour afficher les détails complets du paquet, y compris l'heure, la "
        "longueur, les adresses IP et MAC source/destination, le protocole, etc.\n\n"
        "8 - Nettoyer la liste\n"
        "Si nécessaire, cliquez sur le bouton \"Clear List\" pour effacer tous les éléments de la liste.\n\n"
        "9 - Ajuster la fréquence de capture\n"
        "Utilisez le bouton \"Set Frequency\" pour définir la fréquence de capture en millisecondes.\n\n"
        "Astuces supplémentaires\n"
        "- Assurez-vous que la carte réseau sélectionnée est correcte.\n"
        "- Utilisez le bouton \"Set Frequency\" pour ajuster la fréquence de capture selon vos besoins.\n"
        "- Soyez attentif aux paquets suspects marqués en rouge dans la liste.";

    wxMessageBox(helpText, "Guide de Démarrage Rapide", wxOK | wxICON_INFORMATION, this);
}


void MyFrame::OnNew(wxCommandEvent& event)
{
    // Code pour le menu "New"
}

void MyFrame::OnOpen(wxCommandEvent& event)
{
    // Code pour le menu "Open"
}

void MyFrame::OnSave(wxCommandEvent& event)
{
    // Code pour le menu "Save"
}

void MyFrame::OnSaveAs(wxCommandEvent& event)
{
    // Code pour le menu "Save As"
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close();
}

void MyFrame::OnUndo(wxCommandEvent& event)
{
    // Code pour le menu "Undo"
}

void MyFrame::OnRedo(wxCommandEvent& event)
{
    // Code pour le menu "Redo"
}

void MyFrame::OnCut(wxCommandEvent& event)
{
    // Code pour le menu "Cut"
}

void MyFrame::OnCopy(wxCommandEvent& event)
{
    // Code pour le menu "Copy"
}

void MyFrame::OnPaste(wxCommandEvent& event)
{
    // Code pour le menu "Paste"
}

void MyFrame::OnSelectAll(wxCommandEvent& event)
{
    // Code pour le menu "Select All"
}

void MyFrame::OnOptions(wxCommandEvent& event)
{
    // Code pour le menu "Options"
}

void MyFrame::OnCustomize(wxCommandEvent& event)
{
    // Code pour le menu "Customize"
}


void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxString aboutText =
        "Guard IT - Application de Surveillance Réseau\n"
        "Version Beta\n\n"
        "Développé par :\n"
        "   - PETIT Romain\n"
        "   - DIJOUX Baptiste \n"
        "   - MONNIER Sohail \n"
        "   - WEYMIENS Hugo\n"
        "   - BAZIN Nicolas\n\n"
        "Étudiants de l'ESAIP en Bachelor 2 Cybersécurité\n";

    wxMessageBox(aboutText, "À propos de Guard IT", wxOK | wxICON_INFORMATION, this);
}

void MyFrame::ToggleNightMode()
{
    nightMode = !nightMode;

    // Ajustez ici les éléments d'interface pour le mode nuit ou le mode jour

    // Par exemple, changez la couleur de fond de la fenêtre principale
    if (nightMode) {
        SetBackgroundColour(wxColour(50, 50, 50)); // Couleur de fond pour le mode nuit

    }
    else {
        SetBackgroundColour(*wxWHITE); // Couleur de fond pour le mode jour
    }

    // Changez la couleur du texte dans les éléments d'interface appropriés
    wxColour textColour;
    if (nightMode) {
        textColour = *wxWHITE; // Couleur de texte pour le mode nuit
    }
    else {
        textColour = *wxBLACK; // Couleur de texte pour le mode jour
    }

    // Changez la couleur du texte pour les éléments d'interface
    nameLabel->SetForegroundColour(textColour);
    versionLabel->SetForegroundColour(textColour);
    statusLabel->SetForegroundColour(textColour);
    threatCountLabel->SetForegroundColour(textColour);

    // Changez la couleur de fond des boutons
    wxColour buttonBgColour;
    if (nightMode) {
        buttonBgColour = wxColour(70, 70, 70); // Couleur de fond pour le mode nuit
    }
    else {
        buttonBgColour = *wxWHITE; // Couleur de fond pour le mode jour
    }

    // Changez la couleur de texte des boutons
    wxColour buttonTextColour;
    if (nightMode) {
        buttonTextColour = *wxWHITE; // Couleur de texte pour le mode nuit
    }
    else {
        buttonTextColour = *wxBLACK; // Couleur de texte pour le mode jour
    }

    // Appliquez les changements aux boutons
    startButton->SetBackgroundColour(buttonBgColour);
    startButton->SetForegroundColour(buttonTextColour);

    stopButton->SetBackgroundColour(buttonBgColour);
    stopButton->SetForegroundColour(buttonTextColour);

    clearButton->SetBackgroundColour(buttonBgColour);
    clearButton->SetForegroundColour(buttonTextColour);

    setFrequencyButton->SetBackgroundColour(buttonBgColour);
    setFrequencyButton->SetForegroundColour(buttonTextColour);

    showThreatCountButton->SetBackgroundColour(buttonBgColour);
    showThreatCountButton->SetForegroundColour(buttonTextColour);



    // Rafraîchissez l'affichage
    Refresh();
}


void MyFrame::OnToggleNightMode(wxCommandEvent& event)
{
    ToggleNightMode();
}