#include "MainFrameClient.h"

MainFrameClient::MainFrameClient(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    CreateControls();
    Network::Initialize();
    BindEventHandlers();
    CreateStatusBar();
}

void MainFrameClient::CreateControls()
{
    panel = new wxPanel(this);
    inputIPTextCtrl = new wxTextCtrl(panel, wxID_ANY, "127.0.0.1", wxPoint(200, 200), wxDefaultSize, wxTE_PROCESS_ENTER);
    connectButton = new wxButton(panel, wxID_ANY, "Connect", wxPoint(100, 200), wxDefaultSize);
    disconnectButton = new wxButton(panel, wxID_ANY, "Disconnect", wxPoint(400, 200), wxDefaultSize);
    selectAnotherDeviceButton = new wxButton(panel, wxID_ANY, "Chon may khac", wxPoint(500, 200), wxDefaultSize);
    selectTheFirstDeviceButton = new wxButton(panel, wxID_ANY, "Chon may dau tien", wxPoint(600, 200), wxDefaultSize);
    mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(inputIPTextCtrl, 0, wxALIGN_CENTRE | wxALL, 20);
    mainSizer->Add(connectButton, 0, wxALIGN_CENTRE | wxALL, 20);
    mainSizer->Add(disconnectButton, 0, wxALIGN_CENTRE | wxALL, 20);
    mainSizer->Add(selectAnotherDeviceButton, 0, wxALIGN_CENTRE | wxALL, 20);
    mainSizer->Add(selectTheFirstDeviceButton, 0, wxALIGN_CENTRE | wxALL, 20);
    panel->SetSizer(mainSizer);
    Centre();
}

void MainFrameClient::BindEventHandlers()
{
    inputIPTextCtrl->Bind(wxEVT_TEXT_ENTER, &MainFrameClient::OnInputEnter, this);
    connectButton->Bind(wxEVT_BUTTON, &MainFrameClient::OnConnectButtonClicked, this);
    disconnectButton->Bind(wxEVT_BUTTON, &MainFrameClient::OnDisconnectButtonClicked, this);
    selectAnotherDeviceButton->Bind(wxEVT_BUTTON, &MainFrameClient::selectAnotherDeviceClicked, this);
    selectTheFirstDeviceButton->Bind(wxEVT_BUTTON, &MainFrameClient::selectTheFirstDeviceClicked, this);
}

void MainFrameClient::selectAnotherDeviceClicked(wxCommandEvent& evt) {
    client.selected_device = -1; //ko chon may nao ca
}

void MainFrameClient::selectTheFirstDeviceClicked(wxCommandEvent& evt) {
    client.selected_device = 0;
    client.Frame(client.selected_device); // tao ra 2 thread
}

void MainFrameClient::OnInputEnter(wxCommandEvent& evt)
{
    OnConnectButtonClicked(evt);
}

void MainFrameClient::OnConnectButtonClicked(wxCommandEvent &evt)
{
    wxString inputIPtmp = inputIPTextCtrl->GetValue();
    std::string inputIP = inputIPtmp.ToStdString();
    // idx_of_controled_server = 0; // no giong voi bien selected_device
    client.selected_device = -1;
    client.Connect(IPEndpoint(inputIP.c_str(), 6112));
    
    // client.Frame(client.selected_device);
}

void MainFrameClient::OnDisconnectButtonClicked(wxCommandEvent &evt)
{
    client.CloseConnection(client.selected_device, "do tao bam nut disconnect day, deo phai error.\n");
    // if(0){ // neu muon dong chuong trinh
    // //     client.control.join(); 
    // //     client.video.join();
    // //     runClient.join();
        // Network::Shutdown(); 
    // }
}