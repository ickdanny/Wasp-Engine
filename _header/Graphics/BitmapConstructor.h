#pragma once

#include "windowsInclude.h"
#include "windowsD2DInclude.h"
#include <wincodec.h>			//WIC
#include <atlbase.h>			//CComPtr

#include <string>

namespace wasp::graphics {
	class BitmapConstructor {
	private:
		CComPtr<IWICImagingFactory> wicFactoryPointer{};

	public:
		BitmapConstructor();

		CComPtr<IWICFormatConverter> getWicFormatConverterPointer(
			const std::wstring& fileName
		);

		CComPtr<ID2D1Bitmap> convertWicBitmapToD2D(
			const CComPtr<IWICFormatConverter> formatConverterPointer,
			const CComPtr<ID2D1HwndRenderTarget> renderTargetPointer
		);

	private:
		void init();
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