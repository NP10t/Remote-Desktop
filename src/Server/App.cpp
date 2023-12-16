#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
<<<<<<< HEAD
=======
    ConsoleFrame* consoleFrame = new ConsoleFrame("Console", wxPoint(200, 0), wxSize(500, 300));
    consoleFrame->Show(true);
>>>>>>> remote-origin/nguyenquangthinh
    MainFrameServer* mainFrameServer = new MainFrameServer("Server ", wxPoint(50, 50), wxSize(500, 300));
    mainFrameServer->Center();
    mainFrameServer->Show();

    return true;
}