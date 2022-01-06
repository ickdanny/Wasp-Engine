#include "BitmapStorage.h"

#include "FileUtil.h"

namespace wasp::game::gameresource {

	void BitmapStorage::reload(const std::wstring& id) {
		if (resourceLoaderPointer) {
			auto found{ resourceMap.find(id) };
			if (found != resourceMap.end()) {
				ResourceType& resource{
					*(std::get<1>(*found))
				};
				const resource::ResourceOriginVariant origin{
					resource.getOrigin()
				};
				switch (origin.index()) {
					case 0: {
						resource::FileOrigin const* fileTest{
							std::get_if<resource::FileOrigin>(&origin) 
						};
						if (fileTest) {
							resourceMap.erase(found);
							loadFromFile(*fileTest, *resourceLoaderPointer);
						}
						break;
					}
					case 1: {
						resource::ManifestOrigin const* manifestTest{
							std::get_if<resource::ManifestOrigin>(&origin)
						};
						if (manifestTest) {
							resourceMap.erase(found);
							loadFromManifest(*manifestTest, *resourceLoaderPointer);
						}
						break;
					}
				}
			}
		}
		else {
			throw std::runtime_error{ "Error trying to reload without loader" };
		}
	}

	resource::ResourceBase* BitmapStorage::loadFromFile(
		const resource::FileOrigin& fileOrigin,
		const resource::ResourceLoader& resourceLoader
	) {
		CComPtr<IWICFormatConverter> wicBitmap{
			bitmapConstructorPointer->getWicFormatConverterPointer(fileOrigin.fileName)
		};

		CComPtr<ID2D1Bitmap> d2dBitmap{};
		if (renderTargetPointer) {
			d2dBitmap = bitmapConstructorPointer->converWicBitmapToD2D(
				wicBitmap, renderTargetPointer
			);
		}

		const std::wstring& id{ file::getFileName(fileOrigin.fileName) };
		if (resourceMap.find(id) != resourceMap.end()) {
			throw std::runtime_error{ "Error loaded pre-existing id" };
		}

		std::shared_ptr<resource::Resource<WicAndD2DBitmaps>> resourceSharedPointer{
			std::make_shared<resource::Resource<WicAndD2DBitmaps>>(
				id,
				fileOrigin,
				std::make_shared<WicAndD2DBitmaps>(
					WicAndD2DBitmaps{wicBitmap, {d2dBitmap}}
				)
			)
		};

		resourceSharedPointer->setStoragePointer(this);

		resourceMap.insert({ id, resourceSharedPointer });
		return resourceSharedPointer.get(); //C26816 pointer to memory on stack?
	}

	resource::ResourceBase* BitmapStorage::loadFromManifest(
		const resource::ManifestOrigin& manifestOrigin,
		const resource::ResourceLoader& resourceLoader
	) {
		const std::wstring& fileName{ manifestOrigin.manifestArguments[1] };
		CComPtr<IWICFormatConverter> wicBitmap{
			bitmapConstructorPointer->getWicFormatConverterPointer(fileName)
		};

		CComPtr<ID2D1Bitmap> d2dBitmap{};
		if (renderTargetPointer) {
			d2dBitmap = bitmapConstructorPointer->converWicBitmapToD2D(
				wicBitmap, renderTargetPointer
			);
		}

		const std::wstring& id{ file::getFileName(fileName) };
		if (resourceMap.find(id) != resourceMap.end()) {
			throw std::runtime_error{ "Error loaded pre-existing id" };
		}

		std::shared_ptr<resource::Resource<WicAndD2DBitmaps>> resourceSharedPointer{
			std::make_shared<resource::Resource<WicAndD2DBitmaps>>(
				id,
				manifestOrigin,
				std::make_shared<WicAndD2DBitmaps>(
					WicAndD2DBitmaps{wicBitmap, {d2dBitmap}}
				)
			)
		};

		resourceSharedPointer->setStoragePointer(this);

		resourceMap.insert({ id, resourceSharedPointer });
		return resourceSharedPointer.get(); //C26816 pointer to memory on stack?
	}
	
	void BitmapStorage::setRenderTargetPointerAndLoadD2DBitmaps(
		const CComPtr<ID2D1HwndRenderTarget>& renderTargetPointer
	){
		this->renderTargetPointer = renderTargetPointer;
		throwIfCannotConstructD2DBitmaps();
		forEach(
			[&](std::shared_ptr<ResourceType> resourceSharedPointer) {
				loadD2DBitmap(*resourceSharedPointer);
			}
		);
	}

	void BitmapStorage::loadD2DBitmap(ResourceType& resource) {
		auto& data{ *resource.getDataPointerCopy() }; //C26815 dangling pointer?
		data.d2dBitmap = bitmapConstructorPointer->converWicBitmapToD2D(
			data.wicBitmap, renderTargetPointer
		);
	}

	void BitmapStorage::throwIfCannotConstructD2DBitmaps() {
		if (!bitmapConstructorPointer|| !renderTargetPointer) {
			throw std::runtime_error("Error cannot create D2D Bitmaps");
		}
	}
}