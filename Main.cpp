#ifndef UNICODE
#define UNICODE
#endif

#include "framework.h" //includes window.h and others

#include "Config.h"
#include "WindowUtil.h"
#include "ComLibraryGuard.h"
#include "BitmapManager.h"
#include "BaseWindow.h"
#include "MainWindow.h"

void runMessageLoop();

#pragma warning(suppress:C28251)
int WINAPI wWinMain(HINSTANCE instanceHandle, HINSTANCE, PWSTR, int windowShowMode)
{
    //init COM
    comadapter::ComLibraryGuard comLibraryGuard{};
    comLibraryGuard.init(COINIT_APARTMENTTHREADED);

    //init Resources


    
    //init window, WIC graphics, and Direct2D
    windowadapter::MainWindow window{};
    {
        const MONITORINFO primaryMonitorInfo{ windowadapter::getPrimaryMonitorInfo() };
        const RECT& primaryMonitorRect{ primaryMonitorInfo.rcMonitor };
        const int xCenter{ (primaryMonitorRect.right - primaryMonitorRect.left) / 2 };
        const int yCenter{ (primaryMonitorRect.bottom - primaryMonitorRect.top) / 2 };
        const int x{ xCenter - (config::windowWidth / 2) };
        const int y{ yCenter - (config::windowHeight / 2) };

        window.create(
            instanceHandle,
            config::className,
            config::windowName,
            config::windowStyle,
            0, //extra window style
            x,
            y,
            config::windowWidth,
            config::windowHeight
        );
    }

    ShowWindow(window.getWindowHandle(), windowShowMode);
    
    runMessageLoop();

    return 0;
}

void runMessageLoop() {

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}