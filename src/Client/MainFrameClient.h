#ifndef _MAINFRAMECLIENT_H_
#define _MAINFRAMECLIENT_H_

#include <wx/frame.h>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <string>
#include "MyClient.h"
#include <future>
#include <thread>

class MainFrameClient: public wxFrame
{
    public:
        MainFrameClient(const wxString& title, const wxPoint& pos, const wxSize& size);
        int selectedIndex;
    private:
        void CreateControls();
        void BindEventHandlers();

        void OnConnectButtonClicked(wxCommandEvent& evt);
        void OnDisconnectButtonClicked(wxCommandEvent& evt);
        void OnPlayButtonClicked(wxCommandEvent& evt);
        void OnPauseButtonClicked(wxCommandEvent& evt);
        void OnInputEnter(wxCommandEvent& evt);
        void OnIPListBoxClick(wxCommandEvent& evt);

        std::thread runClient;
        MyClient client;
        wxPanel* panel;

        wxTextCtrl* inputIPTextCtrl;
        wxListBox* IPListBox;

        wxButton* connectButton;
        wxButton* disconnectButton;
        wxButton* playButton;
        wxButton* pauseButton;

        wxBoxSizer* mainSizer;
        wxBoxSizer* secondSizer;
        wxBoxSizer* thirdSizer;
};

#endif