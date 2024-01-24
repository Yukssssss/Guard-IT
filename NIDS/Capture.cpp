#include "MyFrame.h"
#include <wx/filedlg.h>




// Fonction exécutée par le thread de capture
void MyFrame::CaptureThreadFunction()
{
    // Boucle de capture et traitement des paquets
    while (!stopCapture) {
        // Utilisation de pcap_dispatch pour capturer et traiter les paquets
        pcap_dispatch(pcapHandle, 0, [](u_char* user, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
            // Récupération du pointeur vers le contrôle de liste (ListCtrl) depuis l'utilisateur
            wxListCtrl* listCtrl = reinterpret_cast<wxListCtrl*>(user);
            // Entrée dans la section critique pour assurer la sécurité des opérations GUI
            wxMutexGuiEnter();

            // Définition des couleurs pour les types de paquets
            wxColour violetColor("#f1c0e8");
            wxColour blueColor("#90dbf4");
            wxColour redColor("#fd151b");

            // Insertion d'une nouvelle ligne dans la liste avec le numéro de paquet
            long index = listCtrl->InsertItem(listCtrl->GetItemCount(), wxString::Format("%ld", listCtrl->GetItemCount() + 1));

            // Mise à jour des informations de la ligne avec les données du paquet
            listCtrl->SetItem(index, 1, wxString::Format("%ld.%06ld", pkthdr->ts.tv_sec, pkthdr->ts.tv_usec));
            listCtrl->SetItem(index, 2, wxString::Format("%d", pkthdr->len));
            listCtrl->SetItem(index, 3, wxString::Format("%d.%d.%d.%d", packet[26], packet[27], packet[28], packet[29]));
            listCtrl->SetItem(index, 4, wxString::Format("%d.%d.%d.%d", packet[30], packet[31], packet[32], packet[33]));
            listCtrl->SetItem(index, 5, wxString::Format("%02X:%02X:%02X:%02X:%02X:%02X", packet[6], packet[7], packet[8], packet[9], packet[10], packet[11]));
            listCtrl->SetItem(index, 6, wxString::Format("%02X:%02X:%02X:%02X:%02X:%02X", packet[0], packet[1], packet[2], packet[3], packet[4], packet[5]));

            wxString protocolAcronym;
            wxColour backgroundColor; // Nouvelle variable pour stocker la couleur de fond

            listCtrl->SetItem(index, 7, protocolAcronym);

            // Applique la couleur de fond à la nouvelle ligne si elle est définie
            if (backgroundColor.IsOk()) {
                wxListItem item;
                item.SetId(index);
                item.SetColumn(0);
                item.SetBackgroundColour(backgroundColor);
                listCtrl->SetItem(item);
            }

            // Sélection du protocole en fonction du type de paquet
            switch (packet[23]) {
            case 0x06:
                protocolAcronym = "TCP";
                break;
            case 0x11:
                protocolAcronym = "UDP";
                break;
            case 0x01:
                protocolAcronym = "ICMP";
                break;
            case 0x02:
                protocolAcronym = "IGMP";
                break;
            case 0x08:
                protocolAcronym = "EIGRP";
                break;
            case 0x29:
                protocolAcronym = "IPv6 Route Header";
                break;
            case 0x2F:
                protocolAcronym = "GRE";
                break;
            case 0x32:
                protocolAcronym = "RSVP";
                break;
            case 0x3A:
                protocolAcronym = "ICMPv6";
                break;
            case 0x3B:
                protocolAcronym = "IPv6 No Next Header";
                break;
            case 0x3C:
                protocolAcronym = "IPv6 Destination Options";
                break;
            case 0x59:
                protocolAcronym = "OSPF";
                break;
            case 0x84:
                protocolAcronym = "SCTP";
                break;
            default:
                protocolAcronym = "Other";
                break;
            }

            // Mise à jour de la colonne du protocole avec l'acronyme
            listCtrl->SetItem(index, 7, protocolAcronym);

            // Extraction des ports source et destination du paquet
            int sourcePort = (packet[34] << 8) | packet[35];
            int destinationPort = (packet[36] << 8) | packet[37];

            // Vérification des conditions pour définir la menace et la couleur de fond
            if ((sourcePort == 0 && destinationPort > 0) || (destinationPort == 0 && sourcePort > 0)) {
                listCtrl->SetItem(index, 8, "Scan de port");
                backgroundColor = violetColor;
            }

            // Définition de la couleur de fond pour chaque colonne de la ligne
            else if (pkthdr->len > 1000) {
                listCtrl->SetItem(index, 8, "Suspect");
                backgroundColor = redColor;
            }
            else {
                listCtrl->SetItem(index, 8, "RAS");
                backgroundColor = blueColor;
            }

            // Définition de la couleur de fond pour chaque colonne de la ligne
            for (int i = 0; i < listCtrl->GetColumnCount(); i++) {
                wxListItem item;
                item.SetId(index);
                item.SetColumn(i);
                item.SetBackgroundColour(backgroundColor);
                listCtrl->SetItem(item);
            }

            // Assure la visibilité de la ligne dans la fenêtre
            listCtrl->EnsureVisible(index);

            // Sortie de la section critique
            wxMutexGuiLeave();
            }, reinterpret_cast<u_char*>(listCtrl));


        // Pause avant la prochaine itération
        std::this_thread::sleep_for(std::chrono::milliseconds(captureFrequency));
    }
}

// Gestionnaire d'événement pour l'activation d'un élément de la liste
void MyFrame::OnListItemActivated(wxListEvent& event)
{
    // Récupération de l'index de l'élément sélectionné dans la liste
    long itemIndex = event.GetIndex();

    // Récupération des informations du paquet à partir de la liste
    wxString packetNumber = listCtrl->GetItemText(itemIndex, 0);
    wxString time = listCtrl->GetItemText(itemIndex, 1);
    wxString length = listCtrl->GetItemText(itemIndex, 2);
    wxString sourceIP = listCtrl->GetItemText(itemIndex, 3);
    wxString destinationIP = listCtrl->GetItemText(itemIndex, 4);
    wxString sourceMAC = listCtrl->GetItemText(itemIndex, 5);
    wxString destinationMAC = listCtrl->GetItemText(itemIndex, 6);
    wxString protocol = listCtrl->GetItemText(itemIndex, 7);

    // Formatage des détails du paquet pour affichage dans une boîte de dialogue
    wxString details = wxString::Format("Packet #: %s\nTime: %s\nLength: %s\nSource IP: %s\nDestination IP: %s\nSource MAC: %s\nDestination MAC: %s\nProtocol: %s",
        packetNumber, time, length, sourceIP, destinationIP, sourceMAC, destinationMAC, protocol);

    // Affichage des détails du paquet dans une boîte de dialogue d'information
    wxMessageBox(details, "Packet Details", wxOK | wxICON_INFORMATION);
}




void MyFrame::OnSize(wxSizeEvent& event)
{
    // Redimensionnez les colonnes en fonction de la nouvelle largeur de la fenêtre
    int newWidth = event.GetSize().GetWidth();

    // Calculer la nouvelle largeur des colonnes, par exemple, divisez la largeur de la fenêtre par le nombre de colonnes
    int columnWidth = newWidth / listCtrl->GetColumnCount();

    // Appliquer la nouvelle largeur à chaque colonne
    for (int i = 0; i < listCtrl->GetColumnCount(); i++) {
        listCtrl->SetColumnWidth(i, columnWidth);
    }

    event.Skip();
}



void MyFrame::UpdateStatus(bool isRunning)
{
    wxString statusText = isRunning ? wxT("Status: Running") : wxT("Status: Stopped");

    // Changer la couleur du texte en rouge si le statut est "Running"
    if (isRunning) {
        statusLabel->SetForegroundColour(wxColour("#FF0000"));  // Couleur rouge
    } else {
        // Remettre la couleur par défaut si le statut est "Stopped"
        statusLabel->SetForegroundColour(wxNullColour);  // Couleur par défaut
    }

    statusLabel->SetLabel(statusText);
}




