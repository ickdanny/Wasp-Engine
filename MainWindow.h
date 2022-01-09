#pragma once

#include <functional>

#include "BaseWindow.h"
#include "WindowPainter.h"

namespace wasp::windowadapter {
    class MainWindow : public BaseWindow<MainWindow>{
    private:
        WindowPainter windowPainter{};
        std::function<void()> destroyCallback{};
        std::function<void(WPARAM wParam, LPARAM lParam)> keyDownCallback{};
        std::function<void(WPARAM wParam, LPARAM lParam)> keyUpCallback{};

    public:
        MainWindow() = default;

        LRESULT handleMessage(
            UINT messageCode, 
            WPARAM wParam, 
            LPARAM lParam
        ) override;

        WindowPainter& getWindowPainter() {
            return windowPainter;
        }

        void setQuitGameCallback(const std::function<void()>& quitGameCallback) {
            this->destroyCallback = quitGameCallback;
        }
    };
}