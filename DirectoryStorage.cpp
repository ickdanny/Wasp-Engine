#include "DirectoryStorage.h"

#include "IResource.h"

namespace gameresource {

	void DirectoryStorage::reload(const std::wstring& id){
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
							remove(id);
							loadFromFile(*fileTest, *resourceLoaderPointer);
						}
						break;
					}
					case 1: {
						resource::ManifestOrigin const* manifestTest{
							std::get_if<resource::ManifestOrigin>(&origin)
						};
						if (manifestTest) {
							//todo: manifest directory loading
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

	resource::IResource* DirectoryStorage::loadFromFile(
		const resource::FileOrigin& fileOrigin, 
		const resource::ResourceLoader& resourceLoader
	){

		std::shared_ptr childListPointer{ std::make_shared<ChildList>() };
		file::forEachDirectoryEntry(
			fileOrigin.fileName,
			[&](const std::wstring& fileName) {
				childListPointer->push_back(resourceLoader.loadFile({ fileName }));
			}
		);

		const std::wstring& id{ fileOrigin.fileName };
		if (resourceMap.find(id) != resourceMap.end()) {
			throw std::runtime_error{ "Error loaded pre-existing id" };
		}

		std::shared_ptr<ChildListResource> resourceSharedPointer{
			std::make_shared<ChildListResource>(
				id,
				fileOrigin,
				childListPointer
			)
		};

		for (resource::IResource* childPointer : *childListPointer) {
			childPointer->setParentPointer(&*resourceSharedPointer);
		}

		resourceSharedPointer->setStoragePointer(this);

		resourceMap.insert({ id, resourceSharedPointer });
		return resourceSharedPointer.get();
	}
}