#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrameClient* mainFrameClient = new MainFrameClient("CLient ", wxPoint(50, 50), wxSize(500, 300));
    mainFrameClient->Center();
    mainFrameClient->Show();
    ConsoleFrame* consoleFrame = new ConsoleFrame("Console", wxPoint(200, 0), wxSize(500, 300));
    consoleFrame->Show(true);

    return true;
}