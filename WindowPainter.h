#pragma once

#include "framework.h"
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include "BitmapManager.h"

namespace windowadapter {
    class WindowPainter{
    private:
        ID2D1Factory* d2dFactoryPointer{};
        ID2D1HwndRenderTarget* renderTargetPointer{};
        ID2D1SolidColorBrush* brushPointer{};
        D2D1_ELLIPSE ellipse{};

        graphics::BitmapManager bitmapManager{};
        ID2D1Bitmap* bitmapPointer{};

    public:
        WindowPainter();
        ~WindowPainter();

        void init();
        void paint(HWND windowHandle); // TODO: pass image to draw here
        void resize(HWND windowHandle);
        void cleanUp();

    private:
        void getD2dFactoryPointer();
        HRESULT createGraphicsResources(HWND windowHandle);

        // Recalculate drawing layout when the size of the window changes.
        void calculateLayout();
        void discardGraphicsResources();
    };
}