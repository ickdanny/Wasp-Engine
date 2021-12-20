#include "BitmapManager.h"
#include "HResultError.h"

namespace graphics {

	BitmapManager::BitmapManager()
		: wicFactoryPointer{ nullptr } {
	}

	void BitmapManager::init() {
		initWicFactory();
	}

	void BitmapManager::initWicFactory() {
		HRESULT result{
			CoCreateInstance(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&wicFactoryPointer)
			)
		};
		if (FAILED(result)) {
			throw HResultError{ "Error creating WIC imaging factory" };
		}
	}

	CComPtr<ID2D1Bitmap> BitmapManager::getBitmapPointer(const std::wstring& fileName, CComPtr<ID2D1HwndRenderTarget> renderTargetPointer) {

		CComPtr<IWICBitmapFrameDecode> framePointer{ getFramePointer(fileName) };
		CComPtr<IWICFormatConverter> wicFormatConverterPointer{ getWicFormatConverterPointer() };
		initWicFormatConverter(wicFormatConverterPointer, framePointer);

		CComPtr<ID2D1Bitmap> bitmapPointer{};
		HRESULT result{ renderTargetPointer->CreateBitmapFromWicBitmap(wicFormatConverterPointer, NULL, &bitmapPointer) };
		if (FAILED(result)) {
			throw HResultError{ "Error creating bitmap from WIC bitmap" };
		}
		return bitmapPointer;
	}

	CComPtr<IWICFormatConverter> BitmapManager::getWicFormatConverterPointer() {

		CComPtr<IWICFormatConverter> wicFormatConverterPointer{};
		HRESULT result{
			wicFactoryPointer->CreateFormatConverter(&wicFormatConverterPointer)
		};
		if (FAILED(result)) {
			throw HResultError{ "Error creating WIC format converter" };
		}
		return wicFormatConverterPointer;
	}

	CComPtr<IWICBitmapFrameDecode> BitmapManager::getFramePointer(const std::wstring& fileName) {

		CComPtr<IWICBitmapDecoder> imageDecoderPointer{ getImageDecoderPointer(fileName) };

		// Retrieve the first frame of the image from the decoder
		CComPtr<IWICBitmapFrameDecode> framePointer{};

		HRESULT result{ imageDecoderPointer->GetFrame(0, &framePointer) };
		if (FAILED(result)) {
			throw HResultError{ "Error getting frame" };
		}
		return framePointer;
	}

	CComPtr<IWICBitmapDecoder> BitmapManager::getImageDecoderPointer(const std::wstring& fileName) {
		CComPtr<IWICBitmapDecoder> imageDecoderPointer{};
		HRESULT result{ wicFactoryPointer->CreateDecoderFromFilename(
			fileName.c_str(),                // Image to be decoded
			NULL,                            // Do not prefer a particular vendor
			GENERIC_READ,                    // Desired read access to the file
			WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
			&imageDecoderPointer             // Pointer to the decoder
		) };
		if (FAILED(result)) {
			throw HResultError{ "Error creating decoder" };
		}
		return imageDecoderPointer;
	}

	void BitmapManager::initWicFormatConverter(
		CComPtr<IWICFormatConverter> wicFormatConverterPointer, 
		CComPtr<IWICBitmapFrameDecode> framePointer
	) {
		HRESULT result{
			wicFormatConverterPointer->Initialize(
				framePointer,                    // Input bitmap to convert
				GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
				WICBitmapDitherTypeNone,         // Specified dither pattern
				NULL,                            // Specify a particular palette 
				0.f,                             // Alpha threshold
				WICBitmapPaletteTypeCustom       // Palette translation type
			)
		};
		if (FAILED(result)) {
			throw HResultError{ "Error initiating WIC format converter with frame" };
		}
	}

}