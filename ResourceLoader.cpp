#include "ResourceLoader.h"

#include <stdexcept>

#include "FileUtil.h"

namespace resource {

	template <std::size_t NUM_LOADABLES>
	ResourceLoader::ResourceLoader(
		const std::array<Loadable*, NUM_LOADABLES>& loadables
	) {
		for (Loadable* loadable : loadables) {
			if (loadable->isFileLoadable) {
				FileLoadable* fileLoadable{ asFileLoadable(loadable) };
				for (auto& fileExtension : fileLoadable->getAcceptableFileTypes()) {
					insertFileLoadableIntoMap(fileLoadable);
				}
			}
			if (loadable->isManifestLoadable) {

			}
		}
	}

	void ResourceLoader::insertFileLoadableIntoMap(
		const std::wstring& key, 
		FileLoadable* value
	) {
		if (fileExtensionMap.find(key) != fileExtensionMap.end()) {
			throw std::runtime_error{ "Error 2 types claiming same file extension" };
		}
		fileExtensionMap.insert({ key, value });
	}

	void ResourceLoader::insertManifestLoadableIntoMap(
		const std::wstring& key,
		ManifestLoadable* value
	) {
		if (manifestPrefixMap.find(key) != manifestPrefixMap.end()) {
			throw std::runtime_error{ "Error 2 types claiming same manifest prefix" };
		}
		manifestPrefixMap.insert({ key, value });
	}


	std::weak_ptr<IResource> ResourceLoader::loadFile(
		const FileOrigin& fileOrigin
	) const{
		const std::wstring& extension{ 
			file::getFileExtension(fileOrigin.fileName)
		};
		return fileExtensionMap.at(extension)->loadFromFile(fileOrigin, *this);
	}

	std::weak_ptr<IResource> ResourceLoader::loadManifestEntry(
		const ManifestOrigin& manifestOrigin
	) const {
		const std::wstring& prefix{ manifestOrigin.manifestArguments[0] };
		return manifestPrefixMap.at(prefix)->loadFromManifest(manifestOrigin, *this);
	}
}

//ignoring possibility loading resources creates new resource types