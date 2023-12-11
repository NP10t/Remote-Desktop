#ifndef _APP_H_
#define _APP_H_

#include <wx/wx.h>
#include <iostream>
#include "MyClient.h"
#include "../UI/ConsoleFrame.h"
#include "MainFrameClient.h"

class App : public wxApp
{
    public:
        bool OnInit();
};

#endif