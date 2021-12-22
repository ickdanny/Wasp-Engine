#pragma once

#include "framework.h"

#include "BitmapManager.h"

namespace windowadapter {
    class WindowPainter{
    private:
        CComPtr<ID2D1Factory> d2dFactoryPointer{};
        CComPtr<ID2D1HwndRenderTarget> renderTargetPointer{};
        CComPtr<ID2D1SolidColorBrush> brushPointer{};
        D2D1_ELLIPSE ellipse{};

        graphics::BitmapManager bitmapManager{};
        CComPtr<ID2D1Bitmap> bitmapPointer{};

    public:
        WindowPainter();
        ~WindowPainter() = default;

        void init();
        void paint(HWND windowHandle); // TODO: pass image to draw here
        void resize(HWND windowHandle);

    private:
        void getD2dFactoryPointer();
        HRESULT createGraphicsResources(HWND windowHandle);

        // Recalculate drawing layout when the size of the window changes.
        void calculateLayout();
        void discardGraphicsResources();
    };
}