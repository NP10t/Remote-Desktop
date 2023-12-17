#include "MainFrameServer.h"

MainFrameServer::MainFrameServer(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    Network::Initialize();
    CreateControls();
    BindEventHandlers();
    CreateStatusBar();
}

void MainFrameServer::CreateControls()
{
    panel = new wxPanel(this);

    wxString label = wxString::Format("Your IP Address is: %s", server.GetIPv4Address());
    IPStaticText = new wxStaticText(panel, wxID_ANY, label , wxDefaultPosition, wxDefaultSize);

    connectStateStaticText = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);

    connectButton = new wxButton(panel, wxID_ANY, "Open connection", wxPoint(100, 200), wxDefaultSize);
    disconnectButton = new wxButton(panel, wxID_ANY, "Close connection", wxPoint(400, 200), wxDefaultSize);
    mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(IPStaticText, 0, wxALIGN_CENTRE | wxALL, 20);
    mainSizer->Add(connectStateStaticText, 0, wxALIGN_CENTRE | wxALL, 20);
    mainSizer->Add(connectButton, 0, wxALIGN_CENTRE | wxALL, 20);
    mainSizer->Add(disconnectButton, 0, wxALIGN_CENTRE | wxALL, 20);
    panel->SetSizer(mainSizer);
    Centre();
}

void MainFrameServer::BindEventHandlers()
{
    connectButton->Bind(wxEVT_BUTTON, &MainFrameServer::OnConnectButtonClicked, this);
    disconnectButton->Bind(wxEVT_BUTTON, &MainFrameServer::OnDisconnectButtonClicked, this);
    Bind(wxEVT_CLOSE_WINDOW, &MainFrameServer::OnClose, this);
}

void MainFrameServer::OnConnectButtonClicked(wxCommandEvent &evt)
{
    if (flag == 0)
    {
        runServer = std::thread(&MyServer::Run, &server);
        runServer.detach();
        flag = 1;
        connectStateStaticText->SetLabelText("Connecting.....");
    }
}

void MainFrameServer::OnDisconnectButtonClicked(wxCommandEvent &evt)
{
    if (flag == 1)
    {   
        flag = 0;
        connectStateStaticText->SetLabelText("");
        
        if(server.isConnected == false)
            return;

        server.CloseConnection("ban da bam nut dong ket noi");
    }
}

void MainFrameServer::OnClose(wxCloseEvent& evt)
{
    if (flag == 1)
    {   
        flag = 0;
        connectStateStaticText->SetLabelText("");
        
        if(server.isConnected == true) 
            server.CloseConnection("ban da bam nut dong ket noi");
            
    }
    Network::Shutdown();
    Destroy();
}