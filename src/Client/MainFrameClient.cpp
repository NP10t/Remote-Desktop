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
    client.selected_device = 1; //ko chon may nao ca
    if(client.connections.size() == 1){
        MessageBox(NULL, TEXT("may thu 2 ko ton tai"), TEXT("troll nhau ah"), MB_ICONERROR | MB_OK);
        return;
    }

    client.selected_device_connected = true;
    client.Frame(client.selected_device);
}

void MainFrameClient::selectTheFirstDeviceClicked(wxCommandEvent& evt) {
    client.selected_device = 0;
    client.selected_device_connected = true; // sau khi disconnect xong thi bien nay se la false
                                        //moi lan chon dieu khien may nao do thi co 2 truong hop:
                                        //mot la vua disconnect xong (luc nay bien selected_device_connected = false) => gan lai bang true
                                        //hai la vua tam dung dieu khien may nay de dieu khien may khac (luc nay bien selected_device_connected = true) => gan tiep = true cung cha sao
    client.Frame(client.selected_device); // sau khi chon may nao do de dieu khien thi nho goi frame.
}

void MainFrameClient::OnInputEnter(wxCommandEvent& evt)
{
    OnConnectButtonClicked(evt);
}

void MainFrameClient::OnConnectButtonClicked(wxCommandEvent &evt)
{
    wxString inputIPtmp = inputIPTextCtrl->GetValue();
    std::string inputIP = inputIPtmp.ToStdString();
    client.selected_device = -1; //vua vao thi ko chon may nao ca (day la hien tai thoi, tuy theo y tuong lam giao dien cua Tan, Tan cu dieu chinh, co gi hoi NP)
    client.Connect(IPEndpoint(inputIP.c_str(), 6112));
}

void MainFrameClient::OnDisconnectButtonClicked(wxCommandEvent &evt)
{
    client.CloseConnection(client.selected_device, "Ban da bam nut disconnect\n");
    // if(0){ // neu muon dong chuong trinh
    // //     client.control.join(); 
    // //     client.video.join();
    // //     runClient.join();
        // Network::Shutdown(); 
    // }
}

/* khi ma Tan lam nut dong chuong trinh:
-dong chuong trinh thi goi Network::Shutdown(); ngoai ra con lam gi ko thi NP ko biet. A biet r, cho dong het tat ca cac cua so
-Cac thread video va control ko can join nua. 2 thread ay duoc detach co nghia la xong thi no tu dong dung lai. neu no ko dung thi chac la viet them cai dieu kien dung (return)
*/