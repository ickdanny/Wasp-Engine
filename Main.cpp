#ifndef UNICODE
#define UNICODE
#endif 

#include "framework.h" //includes window.h and others
#include "BaseWindow.h"
#include "MainWindow.h"

void runMessageLoop();

int WINAPI wWinMain(HINSTANCE instanceHandle, HINSTANCE, PWSTR, int windowShowMode)
{
    const wchar_t className[] = L"WSWP";
    const wchar_t windowName[] = L"WSWP";

    windowwrapper::MainWindow window{};
    if (!window.create(instanceHandle, className, windowName, WS_OVERLAPPEDWINDOW)) {
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