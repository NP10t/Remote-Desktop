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
    wxLogStatus(inputIPtmp);
	if (Network::Initialize())
	{
		MyClient client;
		if (client.Connect(IPEndpoint(inputIP.c_str(), 6112))) //ket noi den server co dia chi ip... cong... Lam nut bat su kien thi goi ham nay de ket noi, Ket noi moi se duoc luu lai o buffer bla bla NP lam het r
		{
			client.Frame(); //(gui den nguoi lam nut bat su kien): ham nay chi gan goi 1 lan, khi ket noi den server lan dau tien thi goi ham nay, tu do ve sau ko can nua
			client.control.join(); // ham Frame se tao ra 2 thread control va video, nen can co control.join va video.join
			client.video.join(); // ca 3 dong nay di voi nhau, chi can goi 1 lan, ko phai cu co ket noi moi la goi them 1 lan, NO. goi 1 lan thoi
		}
	}
	Network::Shutdown();
}

void MainFrameClient::OnDisconnectButtonClicked(wxCommandEvent &evt)
{
    Network::Shutdown(); 
}