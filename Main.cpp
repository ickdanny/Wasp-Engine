#ifndef UNICODE
#define UNICODE
#endif

#include "framework.h" //includes window.h and others
#include "Config.h"
#include "ComLibraryGuard.h"
#include "BitmapManager.h"
#include "BaseWindow.h"
#include "MainWindow.h"

void runMessageLoop();

int WINAPI wWinMain(HINSTANCE instanceHandle, HINSTANCE, PWSTR, int windowShowMode)
{
    //init COM
    comadapter::ComLibraryGuard comLibraryGuard{};
    comLibraryGuard.init(COINIT_APARTMENTTHREADED);

    //init WIC graphics
    graphics::BitmapManager bitmapManager{};
    bitmapManager.init();

    //init window and Direct2D
    windowadapter::MainWindow window{};
    window.create(
        instanceHandle,
        config::className,
        config::windowName,
        config::windowStyle,
        0, //extra window style
        config::windowX,
        config::windowY,
        config::windowWidth,
        config::windowHeight
    );

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