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
        void OnInputEnter(wxCommandEvent& evt);

        std::thread runClient;
        MyClient client;
        int idx_of_controled_server; // no giong voi bien selected_device
        wxPanel* panel;
        wxTextCtrl* inputIPTextCtrl;
        wxButton* connectButton;
        wxButton* disconnectButton;
        wxButton* selectTheFirstDeviceButton;
        wxButton* selectAnotherDeviceButton;
        wxBoxSizer* mainSizer;
};

#endif