#ifndef _MAINFRAME_H_
#define _MAINFRAME_H_

#include <wx/frame.h>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <string>
#include "MyServer.h"
#include <future>
#include <thread>

class MainFrameServer: public wxFrame
{
    public:
        MainFrameServer(const wxString& title, const wxPoint& pos, const wxSize& size);
    private:
        void CreateControls();
        void BindEventHandlers();

        void OnConnectButtonClicked(wxCommandEvent& evt);
        void OnDisconnectButtonClicked(wxCommandEvent& evt);

        std::thread runServer;
		MyServer server;
        wxPanel* panel;
        wxStaticText* IPStaticText;
        wxButton* connectButton;
        wxButton* disconnectButton;
        wxBoxSizer* mainSizer;
};

#endif