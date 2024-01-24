#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/choicdlg.h>
#include <wx/icon.h>
#include <wx/listctrl.h>
#include <pcap.h>
#include <thread>
#include <atomic>
#include <wx/menu.h>

// Identificateurs des événements
enum {
    ID_NETWORK_INTERFACE_CHOICE = 10001,
    ID_START_CAPTURE,
    ID_STOP_CAPTURE,
    ID_IP_ADDRESS,
    ID_CLEAR_LIST,
    ID_SET_CAPTURE_FREQUENCY,
    ID_TOGGLE_NIGHT_MODE,
    ID_OPTIONS,
    ID_CUSTOMIZE,
    ID_NEW = wxID_HIGHEST + 1,
    ID_OPEN,
    ID_SAVE,
    ID_SAVEAS,
    ID_UNDO,
    ID_REDO,
    ID_CUT,
    ID_COPY,
    ID_PASTE,
    ID_SELECTALL,
    ID_HELP,
    ID_EXPORT_DATA
};

// Déclaration de la classe MyFrame, héritant de wxFrame
class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void OnClose(wxCloseEvent& event);

private:
    // Déclaration des membres de la classe
    wxListCtrl* listCtrl;
    wxChoice* interfaceChoice;
    wxTextCtrl* ipAddressInput;
    wxButton* startButton;
    wxButton* stopButton;
    wxButton* clearButton;
    wxButton* setFrequencyButton;
    pcap_t* pcapHandle;
    std::thread captureThread;
    std::atomic<bool> stopCapture;
    int captureFrequency;
    bool nightMode;
    wxMenuBar* menuBar;
    wxMenu* menuHelp;
    wxStaticText* statusLabel;
    wxStaticText* networkInfoLabel;
    wxStaticText* versionLabel;
    wxStaticText* nameLabel;
    wxStaticText* threatCountLabel;  // Ajout d'une étiquette pour afficher le nombre de menaces
    wxButton* showThreatCountButton; // Ajout d'un bouton pour afficher le nombre de menaces


    // Déclaration des fonctions membres
    void OnStartCapture(wxCommandEvent& event);
    void OnStopCapture(wxCommandEvent& event);
    void CaptureThreadFunction();
    void OnListItemActivated(wxListEvent& event);
    void OnClearList(wxCommandEvent& event);
    void OnSetCaptureFrequency(wxCommandEvent& event);
    bool IsPacketSuspicious(const u_char* packet);
    void OnSize(wxSizeEvent& event);
    void OnToggleNightMode(wxCommandEvent& event);
    void ToggleNightMode();
    void OnShowHelp(wxCommandEvent& event);
    void OnNew(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);
    void OnRedo(wxCommandEvent& event);
    void OnCut(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);
    void OnSelectAll(wxCommandEvent& event);
    void OnOptions(wxCommandEvent& event);
    void OnCustomize(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void UpdateStatus(bool isRunning);
    void OnShowThreatCount(wxCommandEvent& event);  // Nouvelle fonction pour afficher le nombre de menaces
    void UpdateThreatCount();  // Nouvelle fonction pour mettre à jour et afficher le nombre de menaces

    // Déclaration de la table des événements
    wxDECLARE_EVENT_TABLE();
};