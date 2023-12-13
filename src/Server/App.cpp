#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrameServer* mainFrameServer = new MainFrameServer("Server ", wxPoint(50, 50), wxSize(500, 300));
    mainFrameServer->Center();
    mainFrameServer->Show();

    return true;
}