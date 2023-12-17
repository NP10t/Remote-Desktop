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
    // Tao khung input va khung list
    inputIPTextCtrl = new wxTextCtrl(panel, wxID_ANY, "127.0.0.1", wxPoint(200, 200), wxDefaultSize, wxTE_PROCESS_ENTER);
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

    panel->SetSizer(mainSizer);
    Centre();
}

void MainFrameClient::BindEventHandlers()
{
    inputIPTextCtrl->Bind(wxEVT_TEXT_ENTER, &MainFrameClient::OnInputEnter, this);
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

}

void MainFrameClient::OnInputEnter(wxCommandEvent& evt)
{
    OnConnectButtonClicked(evt);
}

void MainFrameClient::OnConnectButtonClicked(wxCommandEvent &evt)
{
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
}

void MainFrameClient::OnDisconnectButtonClicked(wxCommandEvent &evt)
{
    wxArrayInt selections;
    int cnt = IPListBox->GetSelections(selections);

    for (int i = cnt - 1; i >= 0; --i) {
        IPListBox->Delete(selections[i]);
    }
    client.CloseConnection(client.selected_device, "Ban da bam nut disconnect\n");
}
