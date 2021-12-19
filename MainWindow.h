#pragma once

#include "BaseWindow.h"

namespace windowwrapper {
    class MainWindow : public BaseWindow<MainWindow>
    {
    public:
        LRESULT handleMessage(UINT messageCode, WPARAM wParam, LPARAM lParam) override;
    };
}