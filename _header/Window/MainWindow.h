#pragma once

#include <functional>

#include "BaseWindow.h"
#include "WindowPainter.h"
#include "WindowMode.h"

namespace wasp::window {
    class MainWindow : public BaseWindow<MainWindow>{
    private:
        std::wstring currentWindowModeName{};

        WindowPainter windowPainter{};

        std::function<void()> destroyCallback{};
        std::function<void(WPARAM wParam, LPARAM lParam)> keyDownCallback{};
        std::function<void(WPARAM wParam, LPARAM lParam)> keyUpCallback{};
        std::function<void()> outOfFocusCallback{};

    public:
        MainWindow(
            const WindowMode& initWindowMode,
            HINSTANCE instanceHandle,
            PCWSTR className,
            PCWSTR windowName
        );

        LRESULT handleMessage(
            UINT messageCode, 
            WPARAM wParam, 
            LPARAM lParam
        ) override;

        std::wstring& getCurrentWindowModeName() {
            return currentWindowModeName;
        }

        void changeWindowMode(const WindowMode& windowMode);

        WindowPainter& getWindowPainter() {
            return windowPainter;
        }

        void setDestroyCallback(const std::function<void()>& destroyCallback) {
            this->destroyCallback = destroyCallback;
        }

        void setKeyDownCallback(
            const std::function<void(WPARAM wParam, LPARAM lParam)>& keyDownCallback
        ) {
            this->keyDownCallback = keyDownCallback;
        }

        void setKeyUpCallback(
            const std::function<void(WPARAM wParam, LPARAM lParam)>& keyUpCallback
        ) {
            this->keyUpCallback = keyUpCallback;
        }

        void setOutOfFocusCallback(const std::function<void()>& outOfFocusCallback) {
            this->outOfFocusCallback = outOfFocusCallback;
        }
    };
}