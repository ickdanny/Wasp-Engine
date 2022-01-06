#pragma once

#include "framework.h"
#include <string>

namespace wasp::graphics {
	class BitmapConstructor {
	private:
		CComPtr<IWICImagingFactory> wicFactoryPointer{};

	public:
		BitmapConstructor();

		void init();

		CComPtr<IWICFormatConverter> getWicFormatConverterPointer(
			const std::wstring& fileName
		);

		CComPtr<ID2D1Bitmap> converWicBitmapToD2D(
			const CComPtr<IWICFormatConverter> formatConverterPointer,
			const CComPtr<ID2D1HwndRenderTarget> renderTargetPointer
		);

	private:
		void initWicFactory();
		CComPtr<IWICBitmapFrameDecode> getWicBitmapFrameDecodePointer(
			const std::wstring& fileName
		);
		CComPtr<IWICBitmapDecoder> getBitmapDecoderPointer(
			const std::wstring& fileName
		);
		void initWicFormatConverter(
			const CComPtr<IWICFormatConverter> wicFormatConverterPointer, 
			const CComPtr<IWICBitmapFrameDecode> framePointer
		);
	};
}