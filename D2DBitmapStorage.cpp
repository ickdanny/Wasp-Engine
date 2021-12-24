#include "D2DBitmapStorage.h"

namespace gameresource {

	void D2DBitmapStorage::reload(const std::wstring& id) {
		throwIfCannotConstructBitmaps();
		auto found{ resourceMap.find(id) };
		if (found != resourceMap.end()) {
			auto elementPointer{ std::get<1>(*found) };
			const resource::ResourceOriginVariant origin{
				elementPointer->getOrigin()
			};
			if (origin.index() != 3) {
				throw std::runtime_error("Error D2D not internal origin");
			}

			const std::wstring& parentID{
				elementPointer->getParentPointer()->getID()
			};

			auto wicBitmap{ wicBitmapStoragePointer->get(parentID)->wicBitmap };
			CComPtr<ID2D1Bitmap> d2dBitmap{
				bitmapConstructorPointer->converWicBitmapToD2D(
					wicBitmap, renderTargetPointer
				)
			};
			elementPointer->setData(
				std::make_shared<CComPtr<ID2D1Bitmap>>(d2dBitmap)
			);
		}
	}

	void D2DBitmapStorage::loadAllWicBitmaps() {
		throwIfCannotConstructBitmaps();
		wicBitmapStoragePointer->forEach(
			[&](std::shared_ptr<WicResourceType> wicResourceSharedPointer) {
				loadWicBitmap(wicResourceSharedPointer);
			}
		);
	}

	void D2DBitmapStorage::throwIfCannotConstructBitmaps() {
		if (
			!bitmapConstructorPointer
		 || !wicBitmapStoragePointer
		 || !renderTargetPointer
		) {
			throw std::runtime_error("Error cannot create D2D Bitmaps");
		}
	}

	void D2DBitmapStorage::loadWicBitmap(
		std::shared_ptr<WicResourceType>
		wicResourceSharedPointer
	){
		const std::wstring& id{ wicResourceSharedPointer->getID() };
		auto found{ resourceMap.find(id) };
		if (found != resourceMap.end()) {
			throw std::runtime_error("Error D2D Bitmap already exists");
		}

		const auto& wicBitmap{ 
			wicResourceSharedPointer->getDataPointerCopy()->wicBitmap 
		};

		CComPtr<ID2D1Bitmap> d2dBitmap{
			bitmapConstructorPointer->converWicBitmapToD2D(
				wicBitmap, renderTargetPointer
			)
		};
		
		std::shared_ptr<resource::Resource<CComPtr<ID2D1Bitmap>>> 
			resourceSharedPointer{
			std::make_shared<ResourceType>(
				id,
				resource::InternalOrigin{},
				std::make_shared<CComPtr<ID2D1Bitmap>>(d2dBitmap)
			)
		};

		resourceSharedPointer->setStoragePointer(this);
		resourceSharedPointer->setParentPointer(wicResourceSharedPointer.get());
		//add this d2d to the parent
		wicResourceSharedPointer->getDataPointerCopy()->d2dChildResourcePointer
			= resourceSharedPointer.get();

		resourceMap.insert({ id, resourceSharedPointer });
	}
}