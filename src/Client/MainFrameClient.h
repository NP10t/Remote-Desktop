#ifndef _MAINFRAMECLIENT_H_
#define _MAINFRAMECLIENT_H_

#include <wx/frame.h>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <string>
#include "MyClient.h"

class MainFrameClient: public wxFrame
{
    public:
        MainFrameClient(const wxString& title, const wxPoint& pos, const wxSize& size);
    private:
        void CreateControls();
        void BindEventHandlers();

        void OnConnectButtonClicked(wxCommandEvent& evt);
        void OnDisconnectButtonClicked(wxCommandEvent& evt);
        void OnInputEnter(wxCommandEvent& evt);

        wxPanel* panel;
        wxTextCtrl* inputIPTextCtrl;
        wxButton* connectButton;
        wxButton* disconnectButton;
        wxBoxSizer* mainSizer;
};

#endif