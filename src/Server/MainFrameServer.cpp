#include "MainFrameServer.h"

MainFrameServer::MainFrameServer(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    CreateControls();
    BindEventHandlers();
    CreateStatusBar();
}

void MainFrameServer::CreateControls()
{
    panel = new wxPanel(this);
    IPStaticText = new wxStaticText(panel, wxID_ANY, "Your IP Address is: ", wxDefaultPosition, wxDefaultSize);
    connectButton = new wxButton(panel, wxID_ANY, "Connect", wxPoint(100, 200), wxDefaultSize);
    disconnectButton = new wxButton(panel, wxID_ANY, "Disconnect", wxPoint(400, 200), wxDefaultSize);
    mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(IPStaticText, 0, wxALIGN_CENTRE | wxALL, 20);
    mainSizer->Add(connectButton, 0, wxALIGN_CENTRE | wxALL, 20);
    mainSizer->Add(disconnectButton, 0, wxALIGN_CENTRE | wxALL, 20);
    panel->SetSizer(mainSizer);
    Centre();
}

void MainFrameServer::BindEventHandlers()
{
    connectButton->Bind(wxEVT_BUTTON, &MainFrameServer::OnConnectButtonClicked, this);
    disconnectButton->Bind(wxEVT_BUTTON, &MainFrameServer::OnDisconnectButtonClicked, this);
}

void MainFrameServer::OnConnectButtonClicked(wxCommandEvent &evt)
{
    std::string IPString = "127.0.0.1";

    runServer = std::thread(&MyServer::Run, &server, IPString);
}

void MainFrameServer::OnDisconnectButtonClicked(wxCommandEvent &evt)
{
    for (auto& thread : server.threads) 
    {
		thread.join();
    }
    runServer.join();
    Network::Shutdown(); 
}

