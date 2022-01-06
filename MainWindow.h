#pragma once

#include "BaseWindow.h"
#include "WindowPainter.h"

namespace wasp::windowadapter {
    class MainWindow : public BaseWindow<MainWindow>{
    private:
        WindowPainter windowPainter{};
    public:
        MainWindow();

        LRESULT handleMessage(
            UINT messageCode, 
            WPARAM wParam, 
            LPARAM lParam
        ) override;

        WindowPainter& getWindowPainter() {
            return windowPainter;
        }
    };
}