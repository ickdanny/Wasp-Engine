#pragma once

#include <memory>
#include "framework.h"

#include "ResourceStorage.h"
#include "BitmapConstructor.h"
#include "WicBitmapStorage.h"

namespace gameresource {

	class D2DBitmapStorage
		: public resource::ResourceStorage<CComPtr<ID2D1Bitmap>>
	{

		using BitmapConstructor = graphics::BitmapConstructor;
		using ResourceType = resource::Resource<CComPtr<ID2D1Bitmap>>;
		using WicResourceType = resource::Resource<WicBitmapAndChildD2DBitmap>;

	private:
		std::shared_ptr<BitmapConstructor> bitmapConstructorPointer{};
		WicBitmapStorage* wicBitmapStoragePointer{};
		CComPtr<ID2D1HwndRenderTarget> renderTargetPointer{};

	public:
		D2DBitmapStorage(
			const std::shared_ptr<BitmapConstructor>& bitmapConstructorPointer
		)
			: bitmapConstructorPointer{ bitmapConstructorPointer } {
		}

		void reload(const std::wstring& id) override;

		void loadAllWicBitmaps();

		void setWicBitmapStoragePointer(WicBitmapStorage* wicBitmapStoragePointer){
			this->wicBitmapStoragePointer = wicBitmapStoragePointer;
		}

		void setRenderTargetPointer(
			const CComPtr<ID2D1HwndRenderTarget>& renderTargetPointer
		) {
			this->renderTargetPointer = renderTargetPointer;
		}

	private:
		void throwIfCannotConstructBitmaps();
		void loadWicBitmap(std::shared_ptr<WicResourceType>wicResourceSharedPointer);
	};


}