#ifndef UNICODE
#define UNICODE
#endif

#include "framework.h" //includes window.h and others

#include "Config.h"
#include "WindowUtil.h"
#include "ComLibraryGuard.h"
#include "BitmapConstructor.h"
#include "BaseWindow.h"
#include "MainWindow.h"
#include "ResourceMasterStorage.h"

void runMessageLoop();

#pragma warning(suppress : 28251) //suppress inconsistent annotation warning
int WINAPI wWinMain(HINSTANCE instanceHandle, HINSTANCE, PWSTR, int windowShowMode)
{
    //init COM
    comadapter::ComLibraryGuard comLibraryGuard{};
    comLibraryGuard.init(COINIT_APARTMENTTHREADED);

    //init Resources : WIC graphics
    std::shared_ptr<graphics::BitmapConstructor> bitmapConstructorPointer{
        std::make_shared<graphics::BitmapConstructor>()
    };
    bitmapConstructorPointer->init();

    gameresource::ResourceMasterStorage resourceMasterStorage{
        gameresource::WicBitmapStorage{bitmapConstructorPointer},
        gameresource::D2DBitmapStorage{bitmapConstructorPointer}
    };

    resource::ResourceLoader resourceLoader{
        std::array<Loadable*, 1>{&resourceMasterStorage.wicBitmapStorage}
    };
    resourceLoader.loadFile({ L"test_image.png" });

    //init window and Direct 2D
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

    //init D2D Bitmaps
    resourceMasterStorage.d2dBitmapStorage.setWicBitmapStoragePointer(
        &resourceMasterStorage.wicBitmapStorage
    );
    resourceMasterStorage.d2dBitmapStorage.setRenderTargetPointer(
        window.getWindowPainter().getRenderTargetPointer()
    );
    resourceMasterStorage.d2dBitmapStorage.loadAllWicBitmaps();

    //image draw test
    window.getWindowPainter().setBitmapPointer(
        *resourceMasterStorage.d2dBitmapStorage.get(L"test_image")
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