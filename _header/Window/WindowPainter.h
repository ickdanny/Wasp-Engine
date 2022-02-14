#pragma once

#include "windowsInclude.h"
#include "windowsDWriteInclude.h"
//#include <utility>

#include "Graphics\IBitmapDrawer.h"
#include "Graphics\ITextDrawer.h"

namespace wasp::window {
    class WindowPainter 
        : public graphics::IBitmapDrawer
        , public graphics::ITextDrawer
    {
    private:
        CComPtr<ID2D1Factory> d2dFactoryPointer{};
        CComPtr<ID2D1HwndRenderTarget> renderTargetPointer{};

        CComPtr<ID2D1BitmapRenderTarget> bufferRenderTargetPointer{};
        CComPtr<IDWriteTextFormat> textFormatPointer{};
        CComPtr<ID2D1SolidColorBrush> textBrushPointer{};

        //todo: text color and fill color? pass a struct instead?
        int graphicsWidth{};
        int graphicsHeight{};
        wchar_t const* fontName{};
        float fontSize{};
        DWRITE_FONT_WEIGHT fontWeight{};
        DWRITE_FONT_STYLE fontStyle{};
        DWRITE_FONT_STRETCH fontStretch{};
        DWRITE_TEXT_ALIGNMENT textAlignment{};
        DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment{};

    public:
        WindowPainter(
            int graphicsWidth,
            int graphicsHeight,
            wchar_t const* fontName,
            float fontSize,
            DWRITE_FONT_WEIGHT fontWeight,
            DWRITE_FONT_STYLE fontStyle,
            DWRITE_FONT_STRETCH fontStretch,
            DWRITE_TEXT_ALIGNMENT textAlignment,
            DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment
        );

        ~WindowPainter() = default;

        void init(HWND windowHandle);

        void paint(HWND windowHandle);
        void resize(HWND windowHandle);

        CComPtr<ID2D1HwndRenderTarget> getRenderTargetPointer() {
            return renderTargetPointer;
        }

        void beginDraw() override;

        void drawBitmap(
            const geometry::Point2 center,
            const graphics::BitmapDrawInstruction& bitmapDrawInstruction
        ) override;

        void drawSubBitmap(
            const geometry::Point2 center,
            const graphics::BitmapDrawInstruction& bitmapDrawInstruction,
            const geometry::Rectangle& sourceRectangle
        ) override;

        void drawText(
            const geometry::Point2 pos,
            const std::wstring& text,
            const std::pair<float, float> bounds
        ) override;

        void endDraw() override;

    private:
        inline void makeBitmapDrawCall(
            ID2D1Bitmap& bitmap,
            const geometry::Point2 upperLeft,
            float scaledWidth,
            float scaledHeight,
            float opacity
        );

        inline void makeTransformBitmapDrawCall(
            ID2D1Bitmap& bitmap,
            const D2D1::Matrix3x2F& transform,
            const geometry::Point2 upperLeft,
            float scaledWidth,
            float scaledHeight,
            float opacity
        );

        inline void makeSubBitmapDrawCall(
            ID2D1Bitmap& bitmap,
            const geometry::Point2 upperLeft,
            float scaledWidth,
            float scaledHeight,
            float opacity,
            const geometry::Rectangle& sourceRectangle
        );

        inline void makeTransformSubBitmapDrawCall(
            ID2D1Bitmap& bitmap,
            const D2D1::Matrix3x2F& transform,
            const geometry::Point2 upperLeft,
            float scaledWidth,
            float scaledHeight,
            float opacity,
            const geometry::Rectangle& sourceRectangle
        );

        CComPtr<ID2D1Bitmap> getBufferBitmap();

        void getDeviceIndependentResources();
        void getD2dFactoryPointer();
        void getTextFormatPointer();
        CComPtr<IDWriteFactory> getDirectWriteFactoryPointer();

        void getDeviceDependentResources(HWND windowHandle);
        void getRenderTargetPointer(HWND windowHandle);
        void makeBufferRenderTargetPointer();
        void makeTextBrushPointer();

        void discardDeviceDependentResources();
    };
}