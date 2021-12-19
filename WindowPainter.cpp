#include "WindowPainter.h"
#include "ComWrapper.h"

namespace windowwrapper {

    HRESULT WindowPainter::init() {
        return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factoryPointer);
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

            result = renderTargetPointer->EndDraw();
            if (FAILED(result) || result == D2DERR_RECREATE_TARGET)
            {
                discardGraphicsResources();
            }
            EndPaint(windowHandle, &paintStruct);
        }
    }

    HRESULT WindowPainter::createGraphicsResources(HWND windowHandle)
    {
        HRESULT result = S_OK;
        if (renderTargetPointer == NULL)
        {
            RECT rc;
            GetClientRect(windowHandle, &rc);

            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

            result = factoryPointer->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(windowHandle, size),
                &renderTargetPointer);

            if (SUCCEEDED(result))
            {
                const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
                result = renderTargetPointer->CreateSolidColorBrush(color, &brushPointer);

                if (SUCCEEDED(result))
                {
                    calculateLayout();
                }
            }
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
        comwrapper::safeRelease(&renderTargetPointer);
        comwrapper::safeRelease(&brushPointer);
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

    void WindowPainter::cleanUp() {
        discardGraphicsResources();
        comwrapper::safeRelease(&factoryPointer);
    }
}