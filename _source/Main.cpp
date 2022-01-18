#ifndef UNICODE
#define UNICODE
#endif

//todo: midi test
#include <ios>
#include <fstream>
#include "Sound\MidiSequence.h"

#include <thread>
#include <chrono>
#include "windowsInclude.h"

#include "Game\Config.h"
#include "Game\GameLoop.h"
#include "Game\GameResource\ResourceMasterStorage.h"
#include "Window\WindowUtil.h"
#include "Window\BaseWindow.h"
#include "Window\MainWindow.h"
#include "Graphics\BitmapConstructor.h"
#include "Input\KeyInputTable.h"
#include "Sound\MidiSequencer.h"
#include "ComLibraryGuard.h"

//debug
#include "ConsoleOutput.h"
#include "Logging.h"

using namespace wasp;
using namespace wasp::game;

using window::getPrimaryMonitorInfo;
using window::getWindowBorderWidthPadding;
using window::getWindowBorderHeightPadding;

void pumpMessages();

#pragma warning(suppress : 28251) //suppress inconsistent annotation warning
int WINAPI wWinMain(HINSTANCE instanceHandle, HINSTANCE, PWSTR, int windowShowMode){
    try {
        debug::initConsoleOutput();

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
                & resourceMasterStorage.manifestStorage,
                & resourceMasterStorage.bitmapStorage
            }
        };
        //resourceLoader.loadFile({ L"res" }); //test image in res
        resourceLoader.loadFile({ config::mainManifestPath });

        //init window and Direct 2D
        window::MainWindow window{};
        {
            const MONITORINFO primaryMonitorInfo{ getPrimaryMonitorInfo() };
            const RECT& primaryMonitorRect{ primaryMonitorInfo.rcMonitor };
            const int xCenter{ 
                (primaryMonitorRect.right - primaryMonitorRect.left) / 2 
            };
            const int yCenter{ 
                (primaryMonitorRect.bottom - primaryMonitorRect.top) / 2 
            };
            const int realWindowWidth{ 
                config::windowWidth + getWindowBorderWidthPadding() 
            };
            const int realWindowHeight{
                config::windowHeight + getWindowBorderHeightPadding() 
            };
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

        gameloop::GameLoop gameLoop{
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
                    { std::to_wstring(1.0 / fps) },
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
        std::ifstream inStream{ L"res\\immortal smoke.mid", std::ios::binary };
        auto midiSequencePointer{ std::make_shared<sound::midi::MidiSequence>() };
        sound::midi::parseLoopedMidiFile(inStream, *midiSequencePointer, 5000, 10000);
        inStream.close();

        sound::midi::MidiOut midiOut{};
        sound::midi::MidiSequencer midiSequencer{&midiOut};
        midiSequencer.start(midiSequencePointer);
        //end midi test

        gameLoop.run();

        return 0;
    }
    #ifdef _DEBUG
    catch (std::exception& exception) {
        debug::log(exception.what());
        #pragma warning(suppress : 4297)  //if debug, we throw exceptions in main
        throw;
    }
    catch (...) {
        debug::log("Exception caught in main of unknown type\n");
        #pragma warning(suppress : 4297)  //if debug, we throw exceptions in main
        throw;
    }
    #else
    catch (...) {
        std::exit(1);
    }
    #endif
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