#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrameServer* mainFrameServer = new MainFrameServer("Server ", wxPoint(50, 50), wxSize(1400, 1000));
    mainFrameServer->Center();
    mainFrameServer->Show();

    return true;
}