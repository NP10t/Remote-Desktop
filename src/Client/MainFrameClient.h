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
<<<<<<< HEAD
        int selectedIndex;
=======
>>>>>>> remote-origin/nguyenquangthinh
    private:
        void CreateControls();
        void BindEventHandlers();

        void OnConnectButtonClicked(wxCommandEvent& evt);
        void OnDisconnectButtonClicked(wxCommandEvent& evt);
<<<<<<< HEAD
        void OnPlayButtonClicked(wxCommandEvent& evt);
        void OnPauseButtonClicked(wxCommandEvent& evt);
        void OnInputEnter(wxCommandEvent& evt);
        void OnIPListBoxClick(wxCommandEvent& evt);
=======
        void selectTheFirstDeviceClicked(wxCommandEvent& evt);
        void selectAnotherDeviceClicked(wxCommandEvent& evt);
        void OnInputEnter(wxCommandEvent& evt);
>>>>>>> remote-origin/nguyenquangthinh

        std::thread runClient;
        MyClient client;
        wxPanel* panel;
<<<<<<< HEAD

        wxTextCtrl* inputIPTextCtrl;
        wxListBox* IPListBox;

        wxButton* connectButton;
        wxButton* disconnectButton;
        wxButton* playButton;
        wxButton* pauseButton;

        wxBoxSizer* mainSizer;
        wxBoxSizer* secondSizer;
        wxBoxSizer* thirdSizer;
=======
        wxTextCtrl* inputIPTextCtrl;
        wxButton* connectButton;
        wxButton* disconnectButton;
        wxButton* selectTheFirstDeviceButton;
        wxButton* selectAnotherDeviceButton;
        wxBoxSizer* mainSizer;
>>>>>>> remote-origin/nguyenquangthinh
};

#endif