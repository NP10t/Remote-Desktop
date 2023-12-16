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
<<<<<<< HEAD
        void OnClose(wxCloseEvent& event);
=======
>>>>>>> remote-origin/nguyenquangthinh

        std::thread runServer;
		MyServer server;
        wxPanel* panel;
        wxStaticText* IPStaticText;
<<<<<<< HEAD
        wxStaticText* connectStateStaticText;

        wxButton* connectButton;
        wxButton* disconnectButton;
        wxBoxSizer* mainSizer;

        int flag = 0;
        //kiem tra xem thu da bam onconnect button de tao server chua. Ban dau = 0
        //sau khi bam onconnect = 1, bam lai onconnect thi khong co gi xay ra
        //sau khi bam disconnect thi = 0.
=======
        wxButton* connectButton;
        wxButton* disconnectButton;
        wxBoxSizer* mainSizer;
>>>>>>> remote-origin/nguyenquangthinh
};

#endif