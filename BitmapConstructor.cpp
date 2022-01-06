#include "BitmapConstructor.h"
#include "HResultError.h"

namespace wasp::graphics {

	using win32adaptor::HResultError;

	BitmapConstructor::BitmapConstructor()
		: wicFactoryPointer{ nullptr } {
	}

	void BitmapConstructor::init() {
		initWicFactory();
	}

	void BitmapConstructor::initWicFactory() {
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

	CComPtr<IWICFormatConverter> BitmapConstructor::getWicFormatConverterPointer(
		const std::wstring& fileName
	) {

		CComPtr<IWICBitmapFrameDecode> bitmapFrameDecodePointer{
			getWicBitmapFrameDecodePointer(fileName)
		};
		CComPtr<IWICFormatConverter> formatConverterPointer{};
		HRESULT result{
			wicFactoryPointer->CreateFormatConverter(&formatConverterPointer)
		};
		if (FAILED(result)) {
			throw HResultError{ "Error creating WIC format converter" };
		}
		initWicFormatConverter(formatConverterPointer, bitmapFrameDecodePointer);
		return formatConverterPointer;
	}

	CComPtr<IWICBitmapFrameDecode> BitmapConstructor::getWicBitmapFrameDecodePointer(
		const std::wstring& fileName
	) {

		CComPtr<IWICBitmapDecoder> bitmapDecoderPointer{ 
			getBitmapDecoderPointer(fileName) 
		};

		// Retrieve the first frame of the image from the decoder
		CComPtr<IWICBitmapFrameDecode> bitmapFrameDecodePointer{};

		HRESULT result{ bitmapDecoderPointer->GetFrame(0, &bitmapFrameDecodePointer) };
		if (FAILED(result)) {
			throw HResultError{ "Error getting frame" };
		}
		return bitmapFrameDecodePointer;
	}

	CComPtr<IWICBitmapDecoder> BitmapConstructor::getBitmapDecoderPointer(
		const std::wstring& fileName
	) {
		CComPtr<IWICBitmapDecoder> bitmapDecoderPointer{};
		HRESULT result{ wicFactoryPointer->CreateDecoderFromFilename(
			fileName.c_str(),                // Image to be decoded
			NULL,                            // Do not prefer a particular vendor
			GENERIC_READ,                    // Desired read access to the file
			WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
			&bitmapDecoderPointer            // Pointer to the decoder
		) };
		if (FAILED(result)) {
			throw HResultError{ "Error creating decoder" };
		}
		return bitmapDecoderPointer;
	}

	CComPtr<ID2D1Bitmap> BitmapConstructor::converWicBitmapToD2D(
		const CComPtr<IWICFormatConverter> formatConverterPointer,
		const CComPtr<ID2D1HwndRenderTarget> renderTargetPointer
	) {
		CComPtr<ID2D1Bitmap> bitmapPointer{};
		HRESULT result{ renderTargetPointer->CreateBitmapFromWicBitmap(
			formatConverterPointer,
			NULL,
			&bitmapPointer
		) };
		if (FAILED(result)) {
			throw HResultError{ "Error creating bitmap from WIC bitmap" };
		}
		return bitmapPointer;
	}

	void BitmapConstructor::initWicFormatConverter(
		const CComPtr<IWICFormatConverter> formatConverterPointer,
		const CComPtr<IWICBitmapFrameDecode> framePointer
	) {
		HRESULT result{
			formatConverterPointer->Initialize(
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