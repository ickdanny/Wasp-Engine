#include "WindowPainter.h"

#include "Config.h"
#include "HResultError.h"

namespace windowadapter {

	WindowPainter::WindowPainter()
		: d2dFactoryPointer{ nullptr }
		, renderTargetPointer{ nullptr } {
	}

	void WindowPainter::init(HWND windowHandle) {
		getD2dFactoryPointer();
		getGraphicsResources(windowHandle);
	}

	void WindowPainter::paint(HWND windowHandle)
	{
		getGraphicsResources(windowHandle);
		PAINTSTRUCT paintStruct;
		BeginPaint(windowHandle, &paintStruct);

		renderTargetPointer->BeginDraw();

		renderTargetPointer->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));

		//draw buffer onto window
		if (bufferRenderTargetPointer) {
			D2D1_SIZE_F windowSize = renderTargetPointer->GetSize();

			// Create a rectangle same size of current window
			D2D1_RECT_F rectangle = D2D1::RectF(
				0.0f, 0.0f, windowSize.width, windowSize.height
			);

			renderTargetPointer->DrawBitmap(
				getBufferBitmap(),
				rectangle
			);
		}

		HRESULT result = renderTargetPointer->EndDraw();
		if (FAILED(result) || result == D2DERR_RECREATE_TARGET)
		{
			discardGraphicsResources();
		}
		EndPaint(windowHandle, &paintStruct);
	}

	CComPtr<ID2D1Bitmap> WindowPainter::getBufferBitmap() {
		CComPtr<ID2D1Bitmap> toRet{};
		HRESULT result{ bufferRenderTargetPointer->GetBitmap(&toRet) };
		if (FAILED(result)) {
			throw HResultError{ "Error retrieving buffer bitmap from pointer" };
		}
		return toRet;
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

	void WindowPainter::getGraphicsResources(HWND windowHandle)
	{
		if (renderTargetPointer == NULL) {
			getRenderTargetPointer(windowHandle);
			makeBufferRenderTargetPointer();
		}
	}

	void WindowPainter::getRenderTargetPointer(HWND windowHandle) {
		RECT clientRect;
		GetClientRect(windowHandle, &clientRect);

		D2D1_SIZE_U size = D2D1::SizeU(clientRect.right, clientRect.bottom);

		HRESULT result{ d2dFactoryPointer->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(windowHandle, size),
			&renderTargetPointer
		) };

		if (FAILED(result)) {
			throw new HResultError("Error creating render target");
		}
	}


	void WindowPainter::makeBufferRenderTargetPointer() {

		float oldDpiX{};
		float oldDpiY{};
		renderTargetPointer->GetDpi(&oldDpiX, &oldDpiY);

		D2D1_SIZE_F windowSize = renderTargetPointer->GetSize();

		float newDpiX{ config::windowWidth * oldDpiX / windowSize.width };
		float newDpiY{ config::windowHeight * oldDpiY / windowSize.height };

		D2D1_SIZE_U size{ D2D1::SizeU(config::windowWidth, config::windowHeight) };
		HRESULT result{ renderTargetPointer->CreateCompatibleRenderTarget(
			/*D2D1_SIZE_F{
				static_cast<float>(size.width),
				static_cast<float>(size.height)
			},*/
			//D2D1_SIZE_F{640.0f, 480.0f},
			//D2D1_SIZE_F{1.0f, 1.0f},
			D2D1_SIZE_F{newDpiX, newDpiY},
			size,
			&bufferRenderTargetPointer
		) };

		if (FAILED(result)) {
			throw HResultError{ "Error creating buffer render target" };
		}
	}

	void WindowPainter::discardGraphicsResources()
	{
		renderTargetPointer = nullptr;
		bufferRenderTargetPointer = nullptr;
	}

	void WindowPainter::resize(HWND windowHandle)
	{
		if (renderTargetPointer != NULL)
		{
			RECT rectangle;
			GetClientRect(windowHandle, &rectangle);

			D2D1_SIZE_U size = D2D1::SizeU(rectangle.right, rectangle.bottom);

			renderTargetPointer->Resize(size);
			InvalidateRect(windowHandle, NULL, FALSE); //what in the fuck
		}
	}

	void WindowPainter::beginDraw() {
		bufferRenderTargetPointer->BeginDraw();
		bufferRenderTargetPointer->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
	}

	static D2D1::Matrix3x2F makeRotationMatrix(float rotationDegrees, D2D1_POINT_2F center) {
		return D2D1::Matrix3x2F::Rotation(
			rotationDegrees,
			center
		);
	}

	static D2D1::Matrix3x2F makeScaleMatrix(float scale, D2D1_POINT_2F center) {
		return D2D1::Matrix3x2F::Scale(
			scale,
			scale,
			center
		);
	}

	void WindowPainter::drawBitmap(
		const geometry::Point2& center,
		const graphics::BitmapDrawInstruction& bitmapDrawInstruction
	) {
		//assume beginDraw has already been called

		ID2D1Bitmap& bitmap{ *bitmapDrawInstruction.getBitmap() };
		D2D1_SIZE_F originalSize = bitmap.GetSize();

		//only rotation or both rotation and scale
		if (bitmapDrawInstruction.requiresRotation()) {
			D2D1_POINT_2F d2dCenter{ center.x, center.y };
			D2D1::Matrix3x2F transform = makeRotationMatrix(
				bitmapDrawInstruction.getRotationDegrees(),
				d2dCenter
			);
			//both rotation and scale
			if (bitmapDrawInstruction.requiresScale()) {
				transform = transform * makeScaleMatrix(
					bitmapDrawInstruction.getScale(),
					d2dCenter
				);
				float scaledWidth{ 
					originalSize.width * bitmapDrawInstruction.getScale() 
				};
				float scaledHeight{ 
					originalSize.height * bitmapDrawInstruction.getScale() 
				};
				const geometry::Point2& upperLeft{
					center.x - (scaledWidth / 2),
					center.y - (scaledHeight / 2)
				};
				makeTransformBitmapDrawCall(
					bitmap,
					transform,
					upperLeft,
					scaledWidth,
					scaledHeight,
					bitmapDrawInstruction.getOpacity()
				);
			}
			//only rotation
			else {
				const geometry::Point2& upperLeft{
					center.x - (originalSize.width / 2),
					center.y - (originalSize.height / 2)
				};
				makeTransformBitmapDrawCall(
					bitmap,
					transform,
					upperLeft,
					originalSize.width,
					originalSize.height,
					bitmapDrawInstruction.getOpacity()
				);
			}
		}
		//only scale
		else if (bitmapDrawInstruction.requiresScale()) {
			D2D1_POINT_2F d2dCenter{ center.x, center.y };
			D2D1::Matrix3x2F transform = makeScaleMatrix(
				bitmapDrawInstruction.getScale(),
				d2dCenter
			);
			float scaledWidth{
					originalSize.width * bitmapDrawInstruction.getScale()
			};
			float scaledHeight{
				originalSize.height * bitmapDrawInstruction.getScale()
			};
			const geometry::Point2& upperLeft{
				center.x - (scaledWidth / 2),
				center.y - (scaledHeight / 2)
			};
			makeTransformBitmapDrawCall(
				bitmap,
				transform,
				upperLeft,
				scaledWidth,
				scaledHeight,
				bitmapDrawInstruction.getOpacity()
			);
		}
		//normal draw call
		else {
			const geometry::Point2& upperLeft{
				center.x - (originalSize.width / 2),
				center.y - (originalSize.height / 2)
			};
			makeBitmapDrawCall(
				bitmap,
				upperLeft,
				originalSize.width,
				originalSize.height,
				bitmapDrawInstruction.getOpacity()
			);
		}
	}

	inline void WindowPainter::makeBitmapDrawCall(
		ID2D1Bitmap& bitmap,
		const geometry::Point2& upperLeft,
		float scaledWidth,
		float scaledHeight,
		float opacity
	) {
		bufferRenderTargetPointer->DrawBitmap(
			&bitmap,
			D2D1::RectF(
				upperLeft.x,
				upperLeft.y,
				upperLeft.x + scaledWidth,
				upperLeft.y + scaledHeight),
			opacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
		);
	}

	inline void WindowPainter::makeTransformBitmapDrawCall(
		ID2D1Bitmap& bitmap,
		const D2D1::Matrix3x2F& transform,
		const geometry::Point2& upperLeft,
		float scaledWidth,
		float scaledHeight,
		float opacity
	) {
		bufferRenderTargetPointer->SetTransform(transform);
		makeBitmapDrawCall(bitmap, upperLeft, scaledWidth, scaledHeight, opacity);
		bufferRenderTargetPointer->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	void WindowPainter::endDraw() {
		bufferRenderTargetPointer->EndDraw();
	}
}