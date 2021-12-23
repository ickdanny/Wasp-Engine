#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Loadable.h"
#include "IResource.h"

using ManifestPrefixes = std::vector<std::wstring>;
using ManifestArguments = std::vector<std::wstring>;

namespace resource {

	class ResourceLoader;

	struct ManifestOrigin {
		ManifestArguments manifestArguments{};
	};

	class ManifestLoadable : virtual public Loadable{
	private:
		ManifestPrefixes manifestPrefixes{};
	public:
		ManifestLoadable(const ManifestPrefixes& manifestPrefixes)
			: manifestPrefixes{ manifestPrefixes } {
		}
		virtual ~ManifestLoadable() = default;

		bool isManifestLoadable() const override {
			return true;
		}

		const ManifestPrefixes& getAcceptableManifestPrefixes() const {
			return manifestPrefixes;
		}
		virtual std::weak_ptr<IResource> loadFromManifest(
			const ManifestOrigin& manifestOrigin,
			const ResourceLoader& resourceMasterStorage
		) = 0;
	};
}