#include "MyFrame.h"

// Définition de la classe d'application MyApp
class MyApp : public wxApp {
public:
    virtual bool OnInit();  // Fonction d'initialisation de l'application
};

// Implémentation de l'application wxWidgets
wxIMPLEMENT_APP(MyApp);

// Fonction d'initialisation de l'application
bool MyApp::OnInit() {
    // Création de la fenêtre principale
    MyFrame* frame = new MyFrame("Guard IT", wxPoint(50, 50), wxSize(1500, 600));

    // Affichage de la fenêtre
    frame->Show(true);

    // Indique que l'initialisation s'est déroulée avec succès
    return true;
}