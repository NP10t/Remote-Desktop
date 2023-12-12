#include "ConsoleFrame.h"

ConsoleFrame::ConsoleFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    CreateControls();
    BindEventHandlers();
    CreateStatusBar();
}

void ConsoleFrame::CreateControls()
{
    panel = new wxPanel(this, wxID_ANY);
    consoleTextCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    inputTextCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    mainSizer = new wxBoxSizer(wxVERTICAL);

    TextCtrlStreambuf textCtrlStreambuf(consoleTextCtrl);
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(&textCtrlStreambuf);

        // Test: print something to std::cout
    std::cout << "Hello, wxWidgets!" << std::endl;

        // Restore the original cout buffer when done
    std::cout.rdbuf(oldCoutBuffer);

    mainSizer->Add(consoleTextCtrl, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(inputTextCtrl, 0, wxEXPAND | wxALL, 5);
    panel->SetSizer(mainSizer);
    std::cout << "Hello";
    Centre();
}

void ConsoleFrame::BindEventHandlers()
{
    inputTextCtrl->Bind(wxEVT_TEXT_ENTER, &ConsoleFrame::OnInputEnter, this);
}

void ConsoleFrame::OnInputEnter(wxCommandEvent& event) {
    wxString inputText = inputTextCtrl->GetValue();
    
    wxString outputText = wxString::Format("Notification: %s\n", inputText);
    consoleTextCtrl->AppendText(outputText);
    
    inputTextCtrl->Clear();
}
