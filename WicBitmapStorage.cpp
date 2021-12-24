#include "WicBitmapStorage.h"

#include "FileUtil.h"

namespace gameresource {

	void WicBitmapResource::removeChild(resource::IResource* child){
		if (dataPointer->d2dChildResourcePointer == child) {
			dataPointer->d2dChildResourcePointer = nullptr;
		}
		else {
			throw std::runtime_error("Error removeChild called on non child");
		}
	}

	static auto getChildResourcePointer(
		resource::Resource<WicBitmapAndChildD2DBitmap>& resource
	) {
		auto element = resource.getDataPointerCopy();
		return element->d2dChildResourcePointer;
	}

	static void unloadChild(
		resource::Resource<WicBitmapAndChildD2DBitmap>& resource
	) {
		auto d2dChildResourcePointer{ getChildResourcePointer(resource) };

		if (d2dChildResourcePointer) {
			if (d2dChildResourcePointer->isLoaded()) {
				const std::wstring& childID{ d2dChildResourcePointer->getID() };
				d2dChildResourcePointer->getStoragePointer()->unload(childID);
			}
		}
	}

	static void reloadChild(
		resource::Resource<WicBitmapAndChildD2DBitmap>& resource
	) {
		auto d2dChildResourcePointer{ getChildResourcePointer(resource) };

		if (d2dChildResourcePointer) {
			const std::wstring& childID{ d2dChildResourcePointer->getID() };
			d2dChildResourcePointer->getStoragePointer()->reload(childID);
		}
	}

	static void removeChild(
		resource::Resource<WicBitmapAndChildD2DBitmap>& resource
	) {
		auto d2dChildResourcePointer{ getChildResourcePointer(resource) };

		if (d2dChildResourcePointer) {
			const std::wstring& childID{ d2dChildResourcePointer->getID() };
			d2dChildResourcePointer->getStoragePointer()->remove(childID);
		}
	}

	void WicBitmapStorage::unload(const std::wstring& id){
		auto found{ resourceMap.find(id) };
		if (found != resourceMap.end()) {
			resource::Resource<WicBitmapAndChildD2DBitmap>& resource{
				*(std::get<1>(*found))
			};
			
			unloadChild(resource);
			resource.unloadData();
		}
	}

	void WicBitmapStorage::reload(const std::wstring& id){
		if (resourceLoaderPointer) {
			auto found{ resourceMap.find(id) };
			if (found != resourceMap.end()) {
				const resource::ResourceOriginVariant origin{
					std::get<1>(*found)->getOrigin()
				};
				switch (origin.index()) {
					case 0: {
						resource::FileOrigin const* fileTest{
							std::get_if<resource::FileOrigin>(&origin) };
						if (fileTest) {
							resourceLoaderPointer->loadFile(*fileTest);
							reloadChild(*(std::get<1>(*found)));
						}
						break;
					}
					case 1: {
						resource::ManifestOrigin const* manifestTest{
							std::get_if<resource::ManifestOrigin>(&origin)
						};
						if (manifestTest) {
							resourceLoaderPointer->loadManifestEntry(*manifestTest);
							reloadChild(*(std::get<1>(*found)));
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

	void WicBitmapStorage::remove(const std::wstring& id){
		auto found{ resourceMap.find(id) };
		if (found != resourceMap.end()) {
			resource::Resource<WicBitmapAndChildD2DBitmap>& resource{
				*(std::get<1>(*found))
			};

			removeChild(resource);
			resourceMap.erase(id);
		}
	}

	resource::IResource* WicBitmapStorage::loadFromFile(
		const resource::FileOrigin& fileOrigin, 
		const resource::ResourceLoader& resourceLoader
	){
		CComPtr<IWICFormatConverter> wicBitmap{
			bitmapConstructorPointer->getWicFormatConverterPointer(fileOrigin.fileName)
		};

		const std::wstring& id{ file::getFileName(fileOrigin.fileName) };
		if (resourceMap.find(id) != resourceMap.end()) {
			throw std::runtime_error{ "Error loaded pre-existing id" };
		}

		std::shared_ptr<WicBitmapResource> resourceSharedPointer{
			std::make_shared<WicBitmapResource>(
				id,
				fileOrigin,
				std::make_shared<WicBitmapAndChildD2DBitmap>(
					WicBitmapAndChildD2DBitmap{wicBitmap, {}}
				)
			)
		};

		resourceSharedPointer->setStoragePointer(this);

		resourceMap.insert({ id, resourceSharedPointer });
		return resourceSharedPointer.get();
	}
}