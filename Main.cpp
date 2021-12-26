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
    graphics::BitmapConstructor bitmapConstructorPointer{};
    bitmapConstructorPointer.init();

    gameresource::ResourceMasterStorage resourceMasterStorage{
        gameresource::DirectoryStorage{},
        gameresource::BitmapStorage{&bitmapConstructorPointer}
    };

    resource::ResourceLoader resourceLoader{
        std::array<Loadable*, 2>{
            &resourceMasterStorage.directoryStorage,
            &resourceMasterStorage.bitmapStorage
        }
    };
    resourceLoader.loadFile({ L"res" }); //test image in res

    //init window and Direct 2D
    windowadapter::MainWindow window{};
    {
        const MONITORINFO primaryMonitorInfo{ windowadapter::getPrimaryMonitorInfo() };
        const RECT& primaryMonitorRect{ primaryMonitorInfo.rcMonitor };
        const int xCenter{ (primaryMonitorRect.right - primaryMonitorRect.left) / 2 };
        const int yCenter{ (primaryMonitorRect.bottom - primaryMonitorRect.top) / 2 };
        const int x{ xCenter - (config::windowWidth / 2) };
        const int y{ yCenter - (config::windowHeight / 2) };

        //will init d2d
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
    resourceMasterStorage.bitmapStorage.setRenderTargetPointerAndLoadD2DBitmaps(
        window.getWindowPainter().getRenderTargetPointer()
    );

    //image draw test
    window.getWindowPainter().beginDraw();
    window.getWindowPainter().drawBitmap(
        { config::windowWidth/2, config::windowHeight/2 },
        { 
            resourceMasterStorage.bitmapStorage.get(L"test_image")->d2dBitmap, 
            //45.0f, 
            //.5f, 
            //.8f 
        }
    );
    window.getWindowPainter().endDraw();

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