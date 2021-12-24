#pragma once

#include <memory>
#include "framework.h"

#include "ResourceStorage.h"
#include "IResource.h"
#include "BitmapConstructor.h"

namespace gameresource {

	struct WicBitmapAndChildD2DBitmap {
		CComPtr<IWICFormatConverter> wicBitmap;
		resource::IResource* d2dChildResourcePointer;
	};

	class WicBitmapResource : public resource::Resource<WicBitmapAndChildD2DBitmap> {
	public:
		WicBitmapResource(
			const std::wstring& id,
			const resource::ResourceOriginVariant& origin
		)
			: resource::Resource<WicBitmapAndChildD2DBitmap>{ id, origin } {
		}

		WicBitmapResource(
			const std::wstring& id,
			const resource::ResourceOriginVariant& origin,
			std::shared_ptr<WicBitmapAndChildD2DBitmap> dataPointer
		)
			: resource::Resource<WicBitmapAndChildD2DBitmap>{ 
				id, origin, dataPointer 
			} {
		}
	protected:
		void removeChild(resource::IResource* child) override;
	};

	class WicBitmapStorage
		: public resource::ResourceStorage<WicBitmapAndChildD2DBitmap>
		, public resource::FileLoadable
	{
		using BitmapConstructor = graphics::BitmapConstructor;
	private:
		std::shared_ptr<BitmapConstructor> bitmapConstructorPointer{};

	public:
		WicBitmapStorage(
			const std::shared_ptr<BitmapConstructor>& bitmapConstructorPointer
		) 
			: FileLoadable{ {L"png"} }
			, bitmapConstructorPointer{ bitmapConstructorPointer } {
		}

		void unload(const std::wstring& id) override;
		void reload(const std::wstring& id) override;
		void remove(const std::wstring& id) override;

		resource::IResource* loadFromFile(
			const resource::FileOrigin& fileOrigin,
			const resource::ResourceLoader& resourceLoader
		) override;
	};
}