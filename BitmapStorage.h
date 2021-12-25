#pragma once

#include <memory>
#include "framework.h"

#include "ResourceStorage.h"
#include "IResource.h"
#include "BitmapConstructor.h"

namespace gameresource {

	struct WicAndD2DBitmaps {
		CComPtr<IWICFormatConverter> wicBitmap{};
		CComPtr<ID2D1Bitmap> d2dBitmap{};
	};

	class BitmapStorage
		: public resource::ResourceStorage<WicAndD2DBitmaps>
		, public resource::FileLoadable
	{
		using BitmapConstructor = graphics::BitmapConstructor;
		using ResourceType = resource::Resource<WicAndD2DBitmaps>;
	private:
		BitmapConstructor* bitmapConstructorPointer{};
		CComPtr<ID2D1HwndRenderTarget> renderTargetPointer{};

	public:
		BitmapStorage(BitmapConstructor* bitmapConstructorPointer) 
			: FileLoadable{ {L"png"} }
			, bitmapConstructorPointer{ bitmapConstructorPointer } {
		}

		void reload(const std::wstring& id) override;

		resource::IResource* loadFromFile(
			const resource::FileOrigin& fileOrigin,
			const resource::ResourceLoader& resourceLoader
		) override;

		//d2d related
		void setRenderTargetPointerAndLoadD2DBitmaps(
			const CComPtr<ID2D1HwndRenderTarget>& renderTargetPointer
		);
		
	private:
		void loadD2DBitmap(ResourceType& resource);
		void throwIfCannotConstructD2DBitmaps();
	};
}