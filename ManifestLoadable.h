#pragma once

#include <string>
#include <vector>

#include "IResource.h"

using ManifestPrefixes = std::vector<std::string>;
using ManifestArguments = std::vector<std::string>;

namespace resource {

	class ResourceMasterStorage;

	struct ManifestOrigin {
		const ManifestArguments& manifestArguments{};
	};

	class ManifestLoadable {
	private:
		const ManifestPrefixes& manifestPrefixes{};
	public:
		ManifestLoadable(const ManifestPrefixes& manifestPrefixes)
			: manifestPrefixes{ manifestPrefixes } {
		}
		virtual ~ManifestLoadable() = default;

		const ManifestPrefixes& getAcceptableManifestPrefixes() const {
			return manifestPrefixes;
		}
		virtual IResource loadFrom(const ManifestOrigin& manifestOrigin, ResourceMasterStorage& resourceMasterStorage) = 0;
	};
}