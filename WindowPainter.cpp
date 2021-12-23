#include "WindowPainter.h"

#include "BitmapManager.h"
#include "HResultError.h"

namespace windowadapter {

    WindowPainter::WindowPainter()
        : d2dFactoryPointer{ nullptr }
        , renderTargetPointer{ nullptr }
        , brushPointer{ nullptr }{
    }

    void WindowPainter::init() {
        getD2dFactoryPointer();
        bitmapManager.init();
    }

    void WindowPainter::paint(HWND windowHandle)
    {
        HRESULT result = createGraphicsResources(windowHandle);
        if (SUCCEEDED(result))
        {
            PAINTSTRUCT paintStruct;
            BeginPaint(windowHandle, &paintStruct);

            renderTargetPointer->BeginDraw();

            renderTargetPointer->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
            renderTargetPointer->FillEllipse(ellipse, brushPointer);


            //image draw test
            D2D1_SIZE_F rtSize = renderTargetPointer->GetSize();

            // Create a rectangle same size of current window
            D2D1_RECT_F rectangle = D2D1::RectF(
                0.0f, 0.0f, rtSize.width, rtSize.height
            );

            if (bitmapPointer)
            {
                renderTargetPointer->DrawBitmap(bitmapPointer, rectangle);
            }
            //end image draw test

            result = renderTargetPointer->EndDraw();
            if (FAILED(result) || result == D2DERR_RECREATE_TARGET)
            {
                discardGraphicsResources();
            }
            EndPaint(windowHandle, &paintStruct);
        }
    }

    void WindowPainter::getD2dFactoryPointer() {
        HRESULT result{ D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, 
            &d2dFactoryPointer
        ) };
        if (FAILED(result)) {
            throw new HResultError("Error creating Direct2D factory");
        }
    }

    //TODO: refactor into void function which throws HResultError 
    HRESULT WindowPainter::createGraphicsResources(HWND windowHandle)
    {
        HRESULT result = S_OK;
        if (renderTargetPointer == NULL)
        {
            RECT rc;
            GetClientRect(windowHandle, &rc);

            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

            result = d2dFactoryPointer->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(windowHandle, size),
                &renderTargetPointer
            );
            
            if (SUCCEEDED(result))
            {
                const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
                result = renderTargetPointer->CreateSolidColorBrush(
                    color, 
                    &brushPointer
                );

                if (SUCCEEDED(result))
                {
                    calculateLayout();
                }
            }
        }

        //image get test
        if (!bitmapPointer) {
            bitmapPointer = bitmapManager.getBitmapPointer(
                L"test_image.png", 
                renderTargetPointer
            );
        }

        return result;
    }

    void WindowPainter::calculateLayout()
    {
        if (renderTargetPointer != NULL)
        {
            D2D1_SIZE_F size = renderTargetPointer->GetSize();
            const float x = size.width / 2;
            const float y = size.height / 2;
            const float radius = min(x, y);
            ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
        }
    }

    void WindowPainter::discardGraphicsResources()
    {
        renderTargetPointer = nullptr;
        brushPointer = nullptr;
    }

    void WindowPainter::resize(HWND windowHandle)
    {
        if (renderTargetPointer != NULL)
        {
            RECT rectangle;
            GetClientRect(windowHandle, &rectangle);

            D2D1_SIZE_U size = D2D1::SizeU(rectangle.right, rectangle.bottom);

            renderTargetPointer->Resize(size);
            calculateLayout();
            InvalidateRect(windowHandle, NULL, FALSE);
        }
    }

}