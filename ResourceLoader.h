#pragma once

#include <unordered_map>
#include <array>
#include <memory>

#include "FileLoadable.h"
#include "ManifestLoadable.h"

namespace resource {

	class ResourceLoader {
	private:
		std::unordered_map<std::wstring, FileLoadable*> fileExtensionMap{};
		std::unordered_map<std::wstring, ManifestLoadable*> manifestPrefixMap{};

	public:
		template <std::size_t NUM_LOADABLES>
		ResourceLoader(
			const std::array<Loadable*, NUM_LOADABLES>& loadables
		) {
			for (Loadable* loadable : loadables) {
				if (loadable->isFileLoadable()) {
					FileLoadable* fileLoadable{ asFileLoadable(loadable) };
					for (auto& fileExtension : 
						fileLoadable->getAcceptableFileTypes()) {
						insertFileLoadableIntoMap(fileExtension, fileLoadable);
					}
				}
				if (loadable->isManifestLoadable()) {
					//todo: handle manifest loadables
				}
			}
		}

		IResource* loadFile(const FileOrigin& fileOrigin) const;
		IResource* loadManifestEntry(
			const ManifestOrigin& manifestOrigin
		) const;

	private:
		FileLoadable* asFileLoadable(Loadable* loadable) const {
			return dynamic_cast<FileLoadable*>(loadable);
		}
		void insertFileLoadableIntoMap(const std::wstring& key, FileLoadable* value);

		ManifestLoadable* asManifestLoadable(Loadable* loadable) const {
			return dynamic_cast<ManifestLoadable*>(loadable);
		}
		void insertManifestLoadableIntoMap(
			const std::wstring& key,
			ManifestLoadable* value
		);
	};
}