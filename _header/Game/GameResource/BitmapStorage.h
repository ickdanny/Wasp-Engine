#pragma once

#include <memory>
#include "windowsInclude.h"

#include "Resource\ResourceStorage.h"
#include "Resource\ResourceBase.h"
#include "Graphics\BitmapConstructor.h"

#pragma warning(disable : 4250) //suppress inherit via dominance

namespace wasp::game::gameresource {

	struct WicAndD2DBitmaps {
		CComPtr<IWICFormatConverter> wicBitmap{};
		CComPtr<ID2D1Bitmap> d2dBitmap{};
	};

	class BitmapStorage
		: public resource::ResourceStorage<WicAndD2DBitmaps>
		, public resource::FileLoadable
		, public resource::ManifestLoadable
	{
	private:
		using BitmapConstructor = graphics::BitmapConstructor;
		using ResourceType = resource::Resource<WicAndD2DBitmaps>;

		BitmapConstructor bitmapConstructor{};
		CComPtr<ID2D1HwndRenderTarget> renderTargetPointer{};

	public:
		BitmapStorage() 
			: FileLoadable{ {L"png"} }
			, ManifestLoadable{ {L"image"} } {
		}

		void reload(const std::wstring& id) override;

		resource::ResourceBase* loadFromFile(
			const resource::FileOrigin& fileOrigin,
			const resource::ResourceLoader& resourceLoader
		) override;

		resource::ResourceBase* loadFromManifest(
			const resource::ManifestOrigin& manifestOrigin,
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