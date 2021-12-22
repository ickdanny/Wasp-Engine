#pragma once

#include <unordered_map>

#include "FileLoadable.h"
#include "ManifestLoadable.h"

namespace resource {

	class ResourceMasterStorage {
	private:
		std::unordered_map<std::string&, FileLoadable&> fileExtensionToResourceTypeMap{};
		std::unordered_map<std::string&, ManifestLoadable&> manifestPrefixToResourceTypeMap{};

	public:
		IResource* loadFile(const FileOrigin& fileOrigin);
		IResource* loadManifestEntry(const ManifestOrigin& manifestOrigin);
	};
}