#pragma once

#include "framework.h"

#include "IBitmapDrawer.h"

namespace windowadapter {
    class WindowPainter : public graphics::IBitmapDrawer{
    private:
        CComPtr<ID2D1Factory> d2dFactoryPointer{};
        CComPtr<ID2D1HwndRenderTarget> renderTargetPointer{};

        CComPtr<ID2D1BitmapRenderTarget> bufferRenderTargetPointer{};

    public:
        WindowPainter();
        ~WindowPainter() = default;

        void init(HWND windowHandle);

        void paint(HWND windowHandle);
        void resize(HWND windowHandle);

        CComPtr<ID2D1HwndRenderTarget> getRenderTargetPointer() {
            return renderTargetPointer;
        }

        void beginDraw() override;

        void drawBitmap(
            const geometry::Point2& center,
            const graphics::BitmapDrawInstruction& bitmapDrawInstruction
        ) override;

        void endDraw() override;

    private:
        inline void makeBitmapDrawCall(
            ID2D1Bitmap& bitmap,
            const geometry::Point2& upperLeft,
            float scaledWidth,
            float scaledHeight,
            float opacity
        );

        inline void makeTransformBitmapDrawCall(
            ID2D1Bitmap& bitmap,
            const D2D1::Matrix3x2F& transform,
            const geometry::Point2& upperLeft,
            float scaledWidth,
            float scaledHeight,
            float opacity
        );

        CComPtr<ID2D1Bitmap> getBufferBitmap();

        void getD2dFactoryPointer();

        void getGraphicsResources(HWND windowHandle);
        void getRenderTargetPointer(HWND windowHandle);
        void makeBufferRenderTargetPointer();

        void discardGraphicsResources();
    };
}