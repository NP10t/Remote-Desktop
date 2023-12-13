#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrameClient* mainFrameClient = new MainFrameClient("CLient ", wxPoint(50, 50), wxSize(500, 300));
    mainFrameClient->Center();
    mainFrameClient->Show();
    return true;
}