#ifndef UNICODE
#define UNICODE
#endif

//todo: midi test
#include <ios>
#include <fstream>
#include "MidiSequence.h"

#include <thread>
#include "framework.h" //includes window.h and others

#include "Config.h"
#include "WindowUtil.h"
#include "ComLibraryGuard.h"
#include "BitmapConstructor.h"
#include "BaseWindow.h"
#include "MainWindow.h"
#include "ResourceMasterStorage.h"
#include "KeyInputTable.h"
#include "MidiSequencer.h"
#include "GameLoop.h"

#ifdef _DEBUG
#include "Debug.h"
#endif

using namespace wasp;
using namespace wasp::game;

using window::getPrimaryMonitorInfo;
using window::getWindowBorderWidthPadding;
using window::getWindowBorderHeightPadding;

void pumpMessages();

#pragma warning(suppress : 28251) //suppress inconsistent annotation warning
int WINAPI wWinMain(HINSTANCE instanceHandle, HINSTANCE, PWSTR, int windowShowMode){

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
    window::MainWindow window{};
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

    //init input
    input::KeyInputTable keyInputTable{};
    window.setKeyDownCallback([&](WPARAM wParam, LPARAM lParam) {
        keyInputTable.handleKeyDown(wParam, lParam);
    });
    window.setKeyUpCallback([&](WPARAM wParam, LPARAM lParam) {
        keyInputTable.handleKeyUp(wParam, lParam);
    });
    window.setOutOfFocusCallback([&] {keyInputTable.allKeysOff(); });

    ShowWindow(window.getWindowHandle(), windowShowMode);

    static int updateCount{ 0 };

    gameloop::GameLoop gameLoop {
        config::updatesPerSecond,
        config::maxUpdatesWithoutFrame,
        //update function
        [&] {
            ++updateCount;
            keyInputTable.tickOver();
            pumpMessages();
        },
        //draw function
        [&](double dt) {
            static bool waitingForVsync{false};

            static constexpr double smoothing{ 0.9 };

            static int frameCount{ 0 };

            static std::chrono::steady_clock::time_point lastDraw{};
            static std::chrono::steady_clock::time_point thisDraw{};

            static double timeToDraw{ 1.0 };
            static double fps{ 1.0 };

            if (waitingForVsync) {
                return;
            }

            window.getWindowPainter().beginDraw();
            window.getWindowPainter().drawSubBitmap(
                { config::graphicsWidth / 2, config::graphicsHeight / 2 },
                {
                    resourceMasterStorage.bitmapStorage.get(L"timage")->d2dBitmap,
                    45.0f,
                    .8f,
                    .7f
                },
                { 100, 100, 600, 400 }
            );
            window.getWindowPainter().drawText(
                { 20.0, 10.0 },
                { std::to_wstring(updateCount) },
                { 300.0f, 500.0f }
            );
            window.getWindowPainter().drawText(
                { 20.0, 30.0 },
                { std::to_wstring(frameCount++) },
                { 300.0f, 500.0f }
            );
            window.getWindowPainter().drawText(
                { 20.0f, 50.0f },
                { std::to_wstring(1.0/fps) },
                { 400.0f, 300.0f }
            );
            window.getWindowPainter().drawText(
                { 20.0f, 70.0f },
                { std::to_wstring(static_cast<int>(keyInputTable[input::KeyValues::K_Z])) },
                { 400.0f, 300.0f }
            );
            window.getWindowPainter().endDraw();

            waitingForVsync = true;

            std::thread render{ [&] {
                window.getWindowPainter().paint(window.getWindowHandle());
                lastDraw = thisDraw;
                thisDraw = std::chrono::steady_clock::now();

                auto duration{ thisDraw - lastDraw };
                timeToDraw = duration.count()
                    * static_cast<double>(std::chrono::steady_clock::period::num)
                    / std::chrono::steady_clock::period::den;

                fps = (fps * smoothing) + (timeToDraw * (1.0 - smoothing));
                waitingForVsync = false;
             } };
            render.detach();
        }
    };

    window.setDestroyCallback([&] {gameLoop.stop(); });

    //midi test
    std::ifstream inStream{ L"res\\example6.mid", std::ios::binary };
    sound::midi::MidiSequence sequence{};
    inStream >> sequence;
    inStream.close();

    sound::midi::MidiSequencer midiSequencer{};
    std::thread soundTest{ [&] {midiSequencer.test(sequence); } };
    soundTest.detach();
    //end midi test

    gameLoop.run();

    return 0;
}

void pumpMessages() {
    MSG msg = { };
    while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
    {
        int result{ GetMessage(&msg, NULL, 0, 0) };

        if (result == -1) {
            throw std::runtime_error("Error failed to get message");
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}