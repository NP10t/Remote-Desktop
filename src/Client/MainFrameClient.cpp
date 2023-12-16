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
<<<<<<< HEAD
    // Tao khung input va khung list
    inputIPTextCtrl = new wxTextCtrl(panel, wxID_ANY, "Nhap IP cua server: ", wxPoint(200, 200), wxDefaultSize, wxTE_PROCESS_ENTER);
    IPListBox = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE | wxLB_EXTENDED);

    // Tao button
    connectButton = new wxButton(panel, wxID_ANY, "CONNECT", wxPoint(100, 200), wxDefaultSize);
    disconnectButton = new wxButton(panel, wxID_ANY, "DISCONNECT", wxPoint(400, 200), wxDefaultSize);
    playButton = new wxButton(panel, wxID_ANY, "PLAY", wxPoint(500, 200), wxDefaultSize);
    pauseButton = new wxButton(panel, wxID_ANY, "PAUSE", wxPoint(600, 200), wxDefaultSize);

    // Tao sizer
    mainSizer = new wxBoxSizer(wxHORIZONTAL);
    secondSizer = new wxBoxSizer(wxVERTICAL);
    thirdSizer = new wxBoxSizer(wxVERTICAL);

    // Dieu chinh layout
    mainSizer->Add(secondSizer, 1, wxEXPAND | wxALL, 10);
    mainSizer->Add(thirdSizer, 1, wxALIGN_CENTRE | wxALL, 10);

    secondSizer->Add(IPListBox, 1, wxEXPAND | wxALL, 20);
    secondSizer->Add(inputIPTextCtrl, 0, wxEXPAND | wxALL, 20);

    thirdSizer->Add(connectButton, 0, wxALIGN_CENTRE | wxALL, 20);
    thirdSizer->Add(disconnectButton, 0, wxALIGN_CENTRE | wxALL, 20);
    thirdSizer->Add(playButton, 0, wxALIGN_CENTRE | wxALL, 20);
    thirdSizer->Add(pauseButton, 0, wxALIGN_CENTRE | wxALL, 20);

=======
    inputIPTextCtrl = new wxTextCtrl(panel, wxID_ANY, "192.168.226.50", wxPoint(200, 200), wxDefaultSize, wxTE_PROCESS_ENTER);
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
>>>>>>> remote-origin/nguyenquangthinh
    panel->SetSizer(mainSizer);
    Centre();
}

void MainFrameClient::BindEventHandlers()
{
    inputIPTextCtrl->Bind(wxEVT_TEXT_ENTER, &MainFrameClient::OnInputEnter, this);
<<<<<<< HEAD
    IPListBox->Bind(wxEVT_LISTBOX, &MainFrameClient::OnIPListBoxClick, this);
    connectButton->Bind(wxEVT_BUTTON, &MainFrameClient::OnConnectButtonClicked, this);
    disconnectButton->Bind(wxEVT_BUTTON, &MainFrameClient::OnDisconnectButtonClicked, this);
    playButton->Bind(wxEVT_BUTTON, &MainFrameClient::OnPlayButtonClicked, this);
    pauseButton->Bind(wxEVT_BUTTON, &MainFrameClient::OnPauseButtonClicked, this);
}

void MainFrameClient::OnIPListBoxClick(wxCommandEvent& evt)
{
    wxArrayInt selections;
    IPListBox->GetSelections(selections);
    selectedIndex = selections.Item(0);
}

void MainFrameClient::OnPlayButtonClicked(wxCommandEvent& evt) {
    if (selectedIndex != wxNOT_FOUND)
    {
        client.selected_device = selectedIndex;
        client.selected_device_connected = true; 
        client.Frame(selectedIndex); 
    }
}

void MainFrameClient::OnPauseButtonClicked(wxCommandEvent& evt) {
    client.selected_device = -1; //ko chon may nao ca

    client.selected_device_connected = true;
    client.Frame(client.selected_device);
}

=======
    connectButton->Bind(wxEVT_BUTTON, &MainFrameClient::OnConnectButtonClicked, this);
    disconnectButton->Bind(wxEVT_BUTTON, &MainFrameClient::OnDisconnectButtonClicked, this);
    selectAnotherDeviceButton->Bind(wxEVT_BUTTON, &MainFrameClient::selectAnotherDeviceClicked, this);
    selectTheFirstDeviceButton->Bind(wxEVT_BUTTON, &MainFrameClient::selectTheFirstDeviceClicked, this);
}

void MainFrameClient::selectAnotherDeviceClicked(wxCommandEvent& evt) {
    client.selected_device = 1; //ko chon may nao ca
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

>>>>>>> remote-origin/nguyenquangthinh
void MainFrameClient::OnInputEnter(wxCommandEvent& evt)
{
    OnConnectButtonClicked(evt);
}

void MainFrameClient::OnConnectButtonClicked(wxCommandEvent &evt)
{
<<<<<<< HEAD
    wxString inputIPText = inputIPTextCtrl->GetValue();

    if (!inputIPText.IsEmpty())
    {
        long checkIndex = IPListBox->FindString(inputIPText);
        if (checkIndex == wxNOT_FOUND)
        {
            in_addr addr; 
            int result = inet_pton(AF_INET, (inputIPText.ToStdString()).c_str(), &addr);
            if (result == 1)
            {
                client.selected_device = -1;
                client.Initialize(IPEndpoint((inputIPText.ToStdString()).c_str(), 6112));
                IPListBox->Append(inputIPText);
            }
        }
    }
=======
    wxString inputIPtmp = inputIPTextCtrl->GetValue();
    std::string inputIP = inputIPtmp.ToStdString();
    client.selected_device = -1; //vua vao thi ko chon may nao ca (day la hien tai thoi, tuy theo y tuong lam giao dien cua Tan, Tan cu dieu chinh, co gi hoi NP)
    client.Connect(IPEndpoint(inputIP.c_str(), 6112));
>>>>>>> remote-origin/nguyenquangthinh
}

void MainFrameClient::OnDisconnectButtonClicked(wxCommandEvent &evt)
{
<<<<<<< HEAD
    wxArrayInt selections;
    int cnt = IPListBox->GetSelections(selections);

    for (int i = cnt - 1; i >= 0; --i) {
        IPListBox->Delete(selections[i]);
    }
    client.CloseConnection(client.selected_device, "Ban da bam nut disconnect\n");
}
=======
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
>>>>>>> remote-origin/nguyenquangthinh
