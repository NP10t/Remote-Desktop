#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrameClient* mainFrameClient = new MainFrameClient("CLient ", wxPoint(50, 50), wxSize(1400, 1000));
    mainFrameClient->Center();
    mainFrameClient->Show();

    return true;
}

// virtual bool App::OnInit()
//     {
//         MyFrame* frame = new MyFrame();
//         frame->Show();
//         return true;
//     }