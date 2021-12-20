#pragma once

#include "framework.h"

#include <string>


namespace graphics {
	class BitmapManager {
	private:
		IWICImagingFactory* wicFactoryPointer{};

	public:
		BitmapManager();
		~BitmapManager();

		void init();
		void cleanUp();

		ID2D1Bitmap* getBitmapPointer(const std::wstring& fileName, ID2D1HwndRenderTarget* renderTargetPointer);

	private:
		void initWicFactory();
		IWICFormatConverter* getWicFormatConverterPointer();
		IWICBitmapFrameDecode* getFramePointer(const std::wstring& fileName);
		IWICBitmapDecoder* getImageDecoderPointer(const std::wstring& fileName);
		void initWicFormatConverter(IWICFormatConverter* wicFormatConverterPointer, IWICBitmapFrameDecode* framePointer);
	};

}