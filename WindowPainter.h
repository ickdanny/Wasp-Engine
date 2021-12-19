#pragma once

#include "framework.h"
#include <d2d1.h>
#pragma comment(lib, "d2d1")

namespace windowwrapper {
    class WindowPainter
    {
    private:
        ID2D1Factory* factoryPointer{};
        ID2D1HwndRenderTarget* renderTargetPointer{};
        ID2D1SolidColorBrush* brushPointer{};
        D2D1_ELLIPSE ellipse{};

    public:
        WindowPainter()
            : factoryPointer{ NULL }, renderTargetPointer{ NULL }, brushPointer{ NULL } {
        }

        ~WindowPainter() {
            cleanUp();
        }

        HRESULT init();
        void paint(HWND windowHandle);
        void resize(HWND windowHandle);
        void cleanUp();

    private:
        HRESULT createGraphicsResources(HWND windowHandle);

        // Recalculate drawing layout when the size of the window changes.
        void calculateLayout();
        void discardGraphicsResources();
    };
}