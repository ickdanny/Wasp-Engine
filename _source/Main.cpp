#ifndef UNICODE
#define UNICODE
#endif

#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include "windowsInclude.h"

#include "Game\Config.h"
#include "Game\GameLoop.h"
#include "Game\Resources\ResourceMasterStorage.h"
#include "Game\WindowModes.h"
#include "Window\WindowUtil.h"
#include "Window\BaseWindow.h"
#include "Window\MainWindow.h"
#include "Graphics\BitmapConstructor.h"
#include "Graphics\RenderScheduler.h"
#include "Input\KeyInputTable.h"
#include "Sound\MidiHub.h"
#include "Adaptor\ComLibraryGuard.h"
#include "Game/Game.h"
#include "Game/Settings.h"

//debug
#include "ConsoleOutput.h"
#include "Logging.h"

using namespace wasp;
using namespace wasp::game;

using window::getPrimaryMonitorInfo;
using window::getWindowBorderWidthPadding;
using window::getWindowBorderHeightPadding;

//forward declarations
void pumpMessages();

#pragma warning(suppress : 28251) //suppress inconsistent annotation warning
int WINAPI wWinMain(HINSTANCE instanceHandle, HINSTANCE, PWSTR, int windowShowMode){
    try {
        debug::initConsoleOutput();

        //read settings
        Settings settings{ 
            settings::readOrCreateSettingsFromFile(config::mainConfigPath) 
        };

        //init COM
        windowsadaptor::ComLibraryGuard comLibraryGuard{ COINIT_APARTMENTTHREADED };

        //init Resources : WIC graphics
        resources::ResourceMasterStorage resourceMasterStorage{};

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
            settings.fullscreen ? windowmodes::fullscreen : windowmodes::windowed,
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
        window.setKeyDownCallback(
            [&](WPARAM wParam, LPARAM lParam) {
                keyInputTable.handleKeyDown(wParam, lParam);
            }
        );
        window.setKeyUpCallback(
            [&](WPARAM wParam, LPARAM lParam) {
                keyInputTable.handleKeyUp(wParam, lParam);
            }
        );
        window.setOutOfFocusCallback(
            [&] {keyInputTable.allKeysOff(); }
        );

        //init rendering
        graphics::RendererScheduler renderer{
            &window, 
            &resourceMasterStorage.bitmapStorage,
            config::graphicsWidth,
            config::graphicsHeight
        };

        //init midi
        sound::midi::MidiHub midiHub{ settings.muted };

        //init game
        Game game{ 
            &resourceMasterStorage, 
            &window.getWindowPainter(), 
            &keyInputTable,
            &midiHub
        };

        graphics::RendererScheduler::RenderCallback renderCallback{
            [&](float deltaTime) {
                game.render(deltaTime);
            }
        };

        //init gameloop
        GameLoop gameLoop{
            config::updatesPerSecond,
            config::maxUpdatesWithoutFrame,
            //update function
            [&] {
                game.update();
                pumpMessages();
            },
            //draw function
            [&](float deltaTime) {
                renderer.render(deltaTime, renderCallback);
            }
        };

        auto stopGameLoopCallback{ [&] { gameLoop.stop(); } };

        window.setDestroyCallback(stopGameLoopCallback);
        game.setExitCallback(stopGameLoopCallback);

        //make the game visible and begin running
        window.show(windowShowMode);
        gameLoop.run();

        //after the game has ended, write settings and exit
        settings::writeSettingsToFile(settings, config::mainConfigPath);

        return 0;
    }
    #ifdef _DEBUG
    catch (std::exception& exception) {
        debug::log(exception.what());
        #pragma warning(suppress : 4297)  //if debug, we throw exceptions in main
        throw;
    }
    catch (std::string& str) {
        debug::log(str);
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
            throw std::runtime_error{ "Error message pump failed to get message" };
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}