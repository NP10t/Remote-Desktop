#ifndef _APP_H_
#define _APP_H_

#include <wx/wx.h>
#include <iostream>
#include "MainFrameServer.h"
#include "MyServer.h"
#include "../UI/ConsoleFrame.h"

class App : public wxApp
{
    public:
        bool OnInit();
};

#endif