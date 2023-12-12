#include "MainFrameClient.h"

MainFrameClient::MainFrameClient(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    CreateControls();
    BindEventHandlers();
    CreateStatusBar();
}

void MainFrameClient::CreateControls()
{
    panel = new wxPanel(this);
    inputIPTextCtrl = new wxTextCtrl(panel, wxID_ANY, "Nhap IP cua server: ", wxPoint(200, 200), wxDefaultSize, wxTE_PROCESS_ENTER);
    connectButton = new wxButton(panel, wxID_ANY, "Connect", wxPoint(100, 200), wxDefaultSize);
    disconnectButton = new wxButton(panel, wxID_ANY, "Disconnect", wxPoint(400, 200), wxDefaultSize);
    mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(inputIPTextCtrl, 0, wxALIGN_CENTRE | wxALL, 20);
    mainSizer->Add(connectButton, 0, wxALIGN_CENTRE | wxALL, 20);
    mainSizer->Add(disconnectButton, 0, wxALIGN_CENTRE | wxALL, 20);
    panel->SetSizer(mainSizer);
    Centre();
}

void MainFrameClient::BindEventHandlers()
{
    inputIPTextCtrl->Bind(wxEVT_TEXT_ENTER, &MainFrameClient::OnInputEnter, this);
    connectButton->Bind(wxEVT_BUTTON, &MainFrameClient::OnConnectButtonClicked, this);
    disconnectButton->Bind(wxEVT_BUTTON, &MainFrameClient::OnDisconnectButtonClicked, this);
}

void MainFrameClient::OnInputEnter(wxCommandEvent& evt)
{
    OnConnectButtonClicked(evt);
}

void MainFrameClient::OnConnectButtonClicked(wxCommandEvent &evt)
{
    wxString inputIPtmp = inputIPTextCtrl->GetValue();
    std::string inputIP = inputIPtmp.ToStdString();

    runClient = std::thread(&MyClient::Run, &client, inputIP);

	// Network::Shutdown();
}

void MainFrameClient::OnDisconnectButtonClicked(wxCommandEvent &evt)
{
    client.control.join(); 
	client.video.join();
    runClient.join();
    Network::Shutdown(); 
}