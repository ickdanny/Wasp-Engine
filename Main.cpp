#ifndef UNICODE
#define UNICODE
#endif 

#include "framework.h" //includes window.h and others
#include "Config.h"
#include "BaseWindow.h"
#include "MainWindow.h"

void runMessageLoop();

int WINAPI wWinMain(HINSTANCE instanceHandle, HINSTANCE, PWSTR, int windowShowMode)
{

    windowwrapper::MainWindow window{};
    if (!window.create(instanceHandle, config::className, config::windowName, WS_OVERLAPPEDWINDOW)) {
        return 0;
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