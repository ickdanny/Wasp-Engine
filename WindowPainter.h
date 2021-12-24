#pragma once

#include "framework.h"

namespace windowadapter {
    class WindowPainter{
    private:
        CComPtr<ID2D1Factory> d2dFactoryPointer{};
        CComPtr<ID2D1HwndRenderTarget> renderTargetPointer{};

        CComPtr<ID2D1Bitmap> bitmapPointer{};

    public:
        WindowPainter();
        ~WindowPainter() = default;

        void init(HWND windowHandle);

        void paint(HWND windowHandle);
        void resize(HWND windowHandle);

        CComPtr<ID2D1HwndRenderTarget> getRenderTargetPointer() {
            return renderTargetPointer;
        }

        void setBitmapPointer(CComPtr<ID2D1Bitmap> bitmapPointer) {
            this->bitmapPointer = bitmapPointer;
        }

    private:
        void getD2dFactoryPointer();
        void createGraphicsResources(HWND windowHandle);

        // Recalculate drawing layout when the size of the window changes.
        void calculateLayout();
        void discardGraphicsResources();
    };
}