#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Loadable.h"
#include "ResourceBase.h"

namespace wasp::resource {

	using ManifestPrefixes = std::vector<std::wstring>;
	using ManifestArguments = std::vector<std::wstring>;

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
		virtual ResourceBase* loadFromManifest(
			const ManifestOrigin& manifestOrigin,
			const ResourceLoader& resourceLoader
		) = 0;
	};
}