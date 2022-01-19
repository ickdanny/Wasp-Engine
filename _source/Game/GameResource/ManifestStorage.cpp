#include "Game\GameResource\ManifestStorage.h"

#include <fstream>
#include <sstream>

#include "Resource\ResourceBase.h"

namespace wasp::game::gameresource {

	//helper methods
	namespace {
		resource::ManifestArguments splitManifestLine(std::wstring& line) {
			resource::ManifestArguments toRet{};
			std::wstringstream stringStream{ line };
			std::wstring arg{};

			while (std::getline(stringStream, arg, L']')) {
				toRet.push_back(arg);
			}

			return toRet;
		}

		resource::ResourceBase* loadManifestEntry(
			std::wstring& line,
			const resource::ResourceLoader& resourceLoader
		) {
			return resourceLoader.loadManifestEntry({ splitManifestLine(line) });
		}

		void populateChildList(
			const std::wstring& fileName,
			resource::ChildList& childList,
			const resource::ResourceLoader& resourceLoader
		) {
			std::wifstream inStream{ fileName };
			std::wstring line{};

			while (std::getline(inStream, line)) {
				childList.push_back(loadManifestEntry(line, resourceLoader));
			}

			inStream.close();
		}
	}

	void ManifestStorage::reload(const std::wstring& id) {
		if (resourceLoaderPointer) {
			auto found{ resourceMap.find(id) };
			if (found != resourceMap.end()) {
				auto& [id, resourcePointer] = *found;
				const resource::ResourceOriginVariant origin{
					resourcePointer->getOrigin()
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
							remove(id);
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

	resource::ResourceBase* ManifestStorage::loadFromFile(
		const resource::FileOrigin& fileOrigin,
		const resource::ResourceLoader& resourceLoader
	) {

		std::shared_ptr childListPointer{ std::make_shared<resource::ChildList>() };

		populateChildList(
			fileOrigin.fileName,
			*childListPointer,
			resourceLoader
		);

		const std::wstring& id{ file::getFileName(fileOrigin.fileName) };
		if (resourceMap.find(id) != resourceMap.end()) {
			throw std::runtime_error{ "Error loaded pre-existing id" };
		}

		std::shared_ptr<resource::ChildListResource> resourceSharedPointer{
			std::make_shared<resource::ChildListResource>(
				id,
				fileOrigin,
				childListPointer
			)
		};

		for (resource::ResourceBase* childPointer : *childListPointer) {
			childPointer->setParentPointer(&*resourceSharedPointer);
		}

		resourceSharedPointer->setStoragePointer(this);

		resourceMap.insert({ id, resourceSharedPointer });
		return resourceSharedPointer.get();
	}

	resource::ResourceBase* ManifestStorage::loadFromManifest(
		const resource::ManifestOrigin& manifestOrigin,
		const resource::ResourceLoader& resourceLoader
	) {

		std::shared_ptr childListPointer{ std::make_shared<resource::ChildList>() };

		const std::wstring& fileName{ manifestOrigin.manifestArguments[1] };

		populateChildList(
			fileName,
			*childListPointer,
			resourceLoader
		);

		const std::wstring& id{ file::getFileName(fileName) };
		if (resourceMap.find(id) != resourceMap.end()) {
			throw std::runtime_error{ "Error loaded pre-existing id" };
		}

		std::shared_ptr<resource::ChildListResource> resourceSharedPointer{
			std::make_shared<resource::ChildListResource>(
				id,
				manifestOrigin,
				childListPointer
			)
		};

		for (resource::ResourceBase* childPointer : *childListPointer) {
			childPointer->setParentPointer(&*resourceSharedPointer);
		}

		resourceSharedPointer->setStoragePointer(this);

		resourceMap.insert({ id, resourceSharedPointer });
		return resourceSharedPointer.get();
	}
}