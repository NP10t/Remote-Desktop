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
    private:
        void CreateControls();
        void BindEventHandlers();

        void OnConnectButtonClicked(wxCommandEvent& evt);
        void OnDisconnectButtonClicked(wxCommandEvent& evt);
        void selectTheFirstDeviceClicked(wxCommandEvent& evt);
        void selectAnotherDeviceClicked(wxCommandEvent& evt);
        void OnAddButtonClicked(wxCommandEvent& evt);
        void OnDeleteButtonClicked(wxCommandEvent& evt);
        void OnInputEnter(wxCommandEvent& evt);

        std::thread runClient;
        MyClient client;
        wxPanel* panel;

        wxTextCtrl* inputIPTextCtrl;
        wxListBox* IPListBox;

        wxButton* connectButton;
        wxButton* disconnectButton;
        wxButton* selectTheFirstDeviceButton;
        wxButton* selectAnotherDeviceButton;
        wxButton* addButton;
        wxButton* deleteButton;

        wxBoxSizer* mainSizer;
        wxBoxSizer* secondSizer;
        wxBoxSizer* thirdSizer;
};

#endif