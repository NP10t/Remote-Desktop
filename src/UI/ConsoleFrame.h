#ifndef _CONSOLE_FRAME_
#define _CONSOLE_FRAME_

#include <wx/frame.h>
#include <wx/wx.h>
#include <string>
#include "TextCtrlStreambuf.h"

class ConsoleFrame : public wxFrame 
{
    public:
        ConsoleFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
        wxTextCtrl* consoleTextCtrl;
    private:
        void OnInputEnter(wxCommandEvent& event);
        void CreateControls();
        void BindEventHandlers();

        wxPanel* panel;
        wxTextCtrl* inputTextCtrl;
        wxBoxSizer* mainSizer;
};

#endif