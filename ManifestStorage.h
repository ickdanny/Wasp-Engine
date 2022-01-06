#pragma once

#include "ParentResourceStorage.h"
#include "FileUtil.h"

#pragma warning(disable : 4250) //inherit via dominance

namespace wasp::game::gameresource {

	class ManifestStorage
		: public resource::ParentResourceStorage
		, public resource::FileLoadable
		, public resource::ManifestLoadable
	{

	public:
		ManifestStorage()
			: FileLoadable{ { L"mnfst" } }
			, ManifestLoadable{ { L"manifest" } } {
		}

		void reload(const std::wstring& id) override;

		resource::ResourceBase* loadFromFile(
			const resource::FileOrigin& fileOrigin,
			const resource::ResourceLoader& resourceLoader
		) override;

		resource::ResourceBase* loadFromManifest(
			const resource::ManifestOrigin& manifestOrigin,
			const resource::ResourceLoader& resourceLoader
		) override;
	};
}