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
#include "GameLoop.h"

using namespace wasp;
using namespace wasp::game;

using windowadapter::getPrimaryMonitorInfo;
using windowadapter::getWindowBorderWidthPadding;
using windowadapter::getWindowBorderHeightPadding;

void update();
void draw(double deltaTime);

#pragma warning(suppress : 28251) //suppress inconsistent annotation warning
int WINAPI wWinMain(HINSTANCE instanceHandle, HINSTANCE, PWSTR, int windowShowMode)
{
    //init COM
    win32adaptor::ComLibraryGuard comLibraryGuard{};
    comLibraryGuard.init(COINIT_APARTMENTTHREADED);

    //init Resources : WIC graphics
    graphics::BitmapConstructor bitmapConstructorPointer{};
    bitmapConstructorPointer.init();

    gameresource::ResourceMasterStorage resourceMasterStorage{
        gameresource::DirectoryStorage{},
        gameresource::ManifestStorage{},
        gameresource::BitmapStorage{&bitmapConstructorPointer}
    };

    resource::ResourceLoader resourceLoader{
        std::array<resource::Loadable*, 3>{
            &resourceMasterStorage.directoryStorage,
            &resourceMasterStorage.manifestStorage,
            &resourceMasterStorage.bitmapStorage
        }
    };
    //resourceLoader.loadFile({ L"res" }); //test image in res
    resourceLoader.loadFile({ config::mainManifestPath });

    //init window and Direct 2D
    windowadapter::MainWindow window{};
    {
        const MONITORINFO primaryMonitorInfo{ getPrimaryMonitorInfo() };
        const RECT& primaryMonitorRect{ primaryMonitorInfo.rcMonitor };
        const int xCenter{ (primaryMonitorRect.right - primaryMonitorRect.left) / 2 };
        const int yCenter{ (primaryMonitorRect.bottom - primaryMonitorRect.top) / 2 };
        const int realWindowWidth
            = config::windowWidth +getWindowBorderWidthPadding();
        const int realWindowHeight
            = config::windowHeight +getWindowBorderHeightPadding();
        const int x{ xCenter - (realWindowWidth / 2) };
        const int y{ yCenter - (realWindowHeight / 2) };

        int temp{ GetSystemMetrics(SM_CYCAPTION) };

        //will init d2d
        window.create(
            instanceHandle,
            config::className,
            config::windowName,
            config::windowStyle,
            0, //extra window style
            x,
            y,
            realWindowWidth,
            realWindowHeight
        );
    }

    //init D2D Bitmaps
    resourceMasterStorage.bitmapStorage.setRenderTargetPointerAndLoadD2DBitmaps(
        window.getWindowPainter().getRenderTargetPointer()
    );

    //image draw test
    window.getWindowPainter().beginDraw();
    window.getWindowPainter().drawSubBitmap(
        { config::graphicsWidth/2, config::graphicsHeight/2 },
        { 
            resourceMasterStorage.bitmapStorage.get(L"timage")->d2dBitmap, 
            45.0f, 
            .8f, 
            .7f 
        },
        {100, 100, 600, 400}
    );
    window.getWindowPainter().drawText(
        { 0, 0 },
        {L"Hello World! Raising say express had chiefly detract demands she. Quiet led own cause three him. Front no party young abode state up. Saved he do fruit woody of to. Met defective are allowance two perceived listening consulted contained. It chicken oh colonel pressed excited suppose to shortly. He improve started no we manners however effects. Prospect"},
        { 300.0f, 500.0f }
    );
    window.getWindowPainter().endDraw();

    ShowWindow(window.getWindowHandle(), windowShowMode);
    
    gameloop::run<
        config::updatesPerSecond, 
        config::maxUpdatesWithoutFrame,
        update,
        draw
    >();

    return 0;
}

void update() {
    MSG msg = { };
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void draw(double deltaTime) {

}