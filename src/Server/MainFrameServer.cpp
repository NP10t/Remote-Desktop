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
    connectButton = new wxButton(panel, wxID_ANY, "Open connection", wxPoint(100, 200), wxDefaultSize);
    disconnectButton = new wxButton(panel, wxID_ANY, "Close connection", wxPoint(400, 200), wxDefaultSize);
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
    std::string IPString = "::";
    runServer = std::thread(&MyServer::Run, &server, IPString);
    runServer.detach();
}

void MainFrameServer::OnDisconnectButtonClicked(wxCommandEvent &evt)
{
    server.CloseConnection("ban da bam nut dong ket noi");
    // runServer.join();
    // Network::Shutdown(); 
}

/* khi ma Tan lam nut dong chuong trinh:
-dong chuong trinh thi goi Network::Shutdown(); ngoai ra con lam gi ko thi NP ko biet. A biet r, cho dong het tat ca cac cua so
-Cac thread video va control ko can join nua. 2 thread ay duoc detach co nghia la xong thi no tu dong dung lai. neu no ko dung thi chac la viet them cai dieu kien dung (return)
*/