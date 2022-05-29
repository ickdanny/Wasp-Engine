#ifndef UNICODE
#define UNICODE
#endif

#include <thread>
#include <chrono>
#include "windowsInclude.h"

#include "Game\Config.h"
#include "Game\GameLoop.h"
#include "Game\Resources\ResourceMasterStorage.h"
#include "Game\WindowModes.h"
#include "Window\WindowUtil.h"
#include "Window\BaseWindow.h"
#include "Window\MainWindow.h"
#include "Graphics\BitmapConstructor.h"
#include "Graphics\Renderer.h" //todo: probably should refactor this so renderer doesn't rely on game classes, maybe move wic/d2d bitmap out?
#include "Input\KeyInputTable.h"
#include "Sound\MidiSequencer.h"
#include "Adaptor\ComLibraryGuard.h"
#include "Game/Game.h"

//debug
#include "ConsoleOutput.h"
#include "Logging.h"

#include "ECS/DataStorage.h"
#include "Game/Scenes.h"
#include "Game/Components.h"

using namespace wasp;
using namespace wasp::game;

using window::getPrimaryMonitorInfo;
using window::getWindowBorderWidthPadding;
using window::getWindowBorderHeightPadding;

void pumpMessages();

void dummyFunc();

#pragma warning(suppress : 28251) //suppress inconsistent annotation warning
int WINAPI wWinMain(HINSTANCE instanceHandle, HINSTANCE, PWSTR, int windowShowMode){
    try {
        debug::initConsoleOutput();

        dummyFunc();

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

        graphics::Renderer renderer{
            &window, 
            &resourceMasterStorage.bitmapStorage,
            config::graphicsWidth,
            config::graphicsHeight
        };

        //init game
        Game game{ resourceMasterStorage };

        window.show(windowShowMode);

        static int updateCount{ 0 };

        GameLoop gameLoop{
            config::updatesPerSecond,
            config::maxUpdatesWithoutFrame,
            //update function
            [&] {
                game.update();
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
            [&](float dt) {
                renderer.render(dt);
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
            throw std::runtime_error("Error failed to get message");
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void dummyFunc() {
    using namespace ecs;
    using namespace ecs::component;
    using namespace ecs::entity;

    DataStorage dataStorage{ 100, 100 };
    Group* group{ dataStorage.getGroup<int, float, double>() };
    
    EntityHandle handle{
        dataStorage.addEntity(
            AddEntityOrder{ std::tuple<int, float>(-14, 2.0f) }
        )
    };

    debug::log(static_cast<std::string>(handle));

    bool success{
        dataStorage.addComponent(
            AddComponentOrder{handle, 3.0}
        )
    };

    debug::log(std::to_string(success));

    auto groupIterator{ group->groupIterator<float, double>() };
    if (groupIterator.isValid()) {
        auto [f, d] = *groupIterator;
        debug::log(std::to_string(f));
        debug::log(std::to_string(d));
    }

    debug::log(std::to_string(dataStorage.getComponent<int>(handle)));

    auto sceneList{ std::move(makeSceneList()) };
}