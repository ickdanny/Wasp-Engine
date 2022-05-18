#ifndef UNICODE
#define UNICODE
#endif

#include <thread>
#include <chrono>
#include "windowsInclude.h"

#include "Game\Config.h"
#include "Game\GameLoop.h"
#include "Game\GameResource\ResourceMasterStorage.h"
#include "Game\WindowModes.h"
#include "Window\WindowUtil.h"
#include "Window\BaseWindow.h"
#include "Window\MainWindow.h"
#include "Graphics\BitmapConstructor.h"
#include "Graphics\Renderer.h" //todo: probably should refactor this so renderer doesn't rely on game classes, maybe move wic/d2d bitmap out?
#include "Input\KeyInputTable.h"
#include "Sound\MidiSequencer.h"
#include "Adaptor\ComLibraryGuard.h"

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
        windowsadaptor::ComLibraryGuard comLibraryGuard{ COINIT_APARTMENTTHREADED };

        //init Resources : WIC graphics
        gameresource::ResourceMasterStorage resourceMasterStorage{};

        resource::ResourceLoader resourceLoader{
            std::array<resource::Loadable*, 4>{
                &resourceMasterStorage.directoryStorage,
                &resourceMasterStorage.manifestStorage,
                &resourceMasterStorage.bitmapStorage,
                &resourceMasterStorage.midiSequenceStorage
            }
        };
        resourceLoader.loadFile({ config::mainManifestPath });

        //init window and Direct 2D
        window::MainWindow window{
            windowmodes::windowed,
            instanceHandle,
            config::className,
            config::windowName,
            config::graphicsWidth,
            config::graphicsHeight,
            config::fillColor,
            config::textColor,
            config::fontName,
            config::fontSize,
            config::fontWeight,
            config::fontStyle,
            config::fontStretch,
            config::textAlignment,
            config::paragraphAlignment
        };

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

        graphics::Renderer renderer{
            &window, 
            &resourceMasterStorage.bitmapStorage,
            config::graphicsWidth,
            config::graphicsHeight
        };

        window.show(windowShowMode);

        static int updateCount{ 0 };

        std::mutex updateDrawMutex{};

        GameLoop gameLoop{
            config::updatesPerSecond,
            config::maxUpdatesWithoutFrame,
            //update function
            [&] {
                const std::lock_guard updateDrawLock{updateDrawMutex};
                ++updateCount;
                keyInputTable.tickOver();
                pumpMessages();

                //fullscreen test
                if (updateCount == 200) {
                    debug::log("fullscreened");
                    window.changeWindowMode(windowmodes::fullscreen);
                }
                if (updateCount == 300) {
                    debug::log("windowed");
                    window.changeWindowMode(windowmodes::windowed);
                }
                //end fullscreen test
            },
            //draw function
            [&](double dt) {
                renderer.render(dt, &updateDrawMutex);
            }
        };

        window.setDestroyCallback([&] {gameLoop.stop(); });

        //midi test
        sound::midi::MidiOut midiOut{};
        sound::midi::MidiSequencer midiSequencer{&midiOut};
        midiSequencer.start(
            resourceMasterStorage.midiSequenceStorage.get(L"immortal smoke")
        );
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