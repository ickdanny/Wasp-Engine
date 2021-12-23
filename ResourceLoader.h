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
		ResourceLoader(const std::array<Loadable*, NUM_LOADABLES>& loadables);

		std::weak_ptr<IResource> loadFile(const FileOrigin& fileOrigin) const;
		std::weak_ptr<IResource> loadManifestEntry(const ManifestOrigin& manifestOrigin) const;

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