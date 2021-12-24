#include "WindowPainter.h"

#include "HResultError.h"

namespace windowadapter {

	WindowPainter::WindowPainter()
		: d2dFactoryPointer{ nullptr }
		, renderTargetPointer{ nullptr } {
	}

	void WindowPainter::init(HWND windowHandle) {
		getD2dFactoryPointer();
		createGraphicsResources(windowHandle);
	}

	void WindowPainter::paint(HWND windowHandle)
	{
		createGraphicsResources(windowHandle);
			PAINTSTRUCT paintStruct;
			BeginPaint(windowHandle, &paintStruct);

			renderTargetPointer->BeginDraw();

			renderTargetPointer->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));

			//image draw test
			if (bitmapPointer) {
				D2D1_SIZE_F rtSize = renderTargetPointer->GetSize();

				// Create a rectangle same size of current window
				D2D1_RECT_F rectangle = D2D1::RectF(
					0.0f, 0.0f, rtSize.width, rtSize.height
				);

				renderTargetPointer->DrawBitmap(bitmapPointer, rectangle);
			}
			//end image draw test

			HRESULT result = renderTargetPointer->EndDraw();
			if (FAILED(result) || result == D2DERR_RECREATE_TARGET)
			{
				discardGraphicsResources();
			}
			EndPaint(windowHandle, &paintStruct);
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

	void WindowPainter::createGraphicsResources(HWND windowHandle)
	{
		if (renderTargetPointer == NULL){
			RECT rc;
			GetClientRect(windowHandle, &rc);

			D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

			HRESULT result{ d2dFactoryPointer->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(windowHandle, size),
				&renderTargetPointer
			)};

			if (FAILED(result)) {
				throw new HResultError("Error creating render target");
			}
			calculateLayout();
		}
	}

	void WindowPainter::calculateLayout()
	{
		if (renderTargetPointer != NULL)
		{
			//todo: wtf
		}
	}

	void WindowPainter::discardGraphicsResources()
	{
		renderTargetPointer = nullptr;
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