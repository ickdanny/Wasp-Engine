#pragma once

#include "framework.h"
#include <string>

namespace graphics {
	class BitmapManager {
	private:
		CComPtr<IWICImagingFactory> wicFactoryPointer{};

	public:
		BitmapManager();

		void init();

		CComPtr<ID2D1Bitmap> getBitmapPointer(const std::wstring& fileName, CComPtr<ID2D1HwndRenderTarget> renderTargetPointer);

	private:
		void initWicFactory();
		CComPtr<IWICFormatConverter> getWicFormatConverterPointer();
		CComPtr<IWICBitmapFrameDecode> getFramePointer(const std::wstring& fileName);
		CComPtr<IWICBitmapDecoder> getImageDecoderPointer(const std::wstring& fileName);
		void initWicFormatConverter(CComPtr<IWICFormatConverter> wicFormatConverterPointer, CComPtr<IWICBitmapFrameDecode> framePointer);
	};
}