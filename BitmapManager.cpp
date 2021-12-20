#include "framework.h"
#include "BitmapManager.h"
#include "ComUtility.h"
#include "HResultError.h"

namespace graphics {

	BitmapManager::BitmapManager()
		: wicFactoryPointer{ NULL } {
	}

	BitmapManager::~BitmapManager() {
		cleanUp();
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


	void BitmapManager::cleanUp() {
		comadapter::safeRelease(&wicFactoryPointer);
	}

	//todo: RAII this shit https://docs.microsoft.com/en-us/archive/msdn-magazine/2015/february/windows-with-c-com-smart-pointers-revisited
	ID2D1Bitmap* BitmapManager::getBitmapPointer(const std::wstring& fileName, ID2D1HwndRenderTarget* renderTargetPointer) {
		IWICBitmapFrameDecode* framePointer{ getFramePointer(fileName) };
		IWICFormatConverter* wicFormatConverterPointer{ getWicFormatConverterPointer() };
		initWicFormatConverter(wicFormatConverterPointer, framePointer);

		ID2D1Bitmap* bitmapPointer{ NULL };
		HRESULT result{ renderTargetPointer->CreateBitmapFromWicBitmap(wicFormatConverterPointer, NULL, &bitmapPointer) };
		if (FAILED(result)) {
			throw HResultError{ "Error creating bitmap from WIC bitmap" };
		}
		return bitmapPointer;
	}

	IWICFormatConverter* BitmapManager::getWicFormatConverterPointer() {

		IWICFormatConverter* wicFormatConverterPointer{ NULL };
		HRESULT result{
			wicFactoryPointer->CreateFormatConverter(&wicFormatConverterPointer)
		};
		if (FAILED(result)) {
			throw HResultError{ "Error creating WIC format converter" };
		}
		return wicFormatConverterPointer;
	}

	IWICBitmapFrameDecode* BitmapManager::getFramePointer(const std::wstring& fileName) {

		IWICBitmapDecoder* imageDecoderPointer{ getImageDecoderPointer(fileName) };

		// Retrieve the first frame of the image from the decoder
		IWICBitmapFrameDecode* framePointer{ NULL };

		HRESULT result{ imageDecoderPointer->GetFrame(0, &framePointer) };
		if (FAILED(result)) {
			throw HResultError{ "Error getting frame" };
		}
		return framePointer;
	}

	IWICBitmapDecoder* BitmapManager::getImageDecoderPointer(const std::wstring& fileName) {
		IWICBitmapDecoder* imageDecoderPointer{ NULL };
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

	void BitmapManager::initWicFormatConverter(IWICFormatConverter* wicFormatConverterPointer, IWICBitmapFrameDecode* framePointer) {
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