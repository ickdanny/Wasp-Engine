#pragma once

#include "Resource\ParentResourceStorage.h"
#include "File\FileUtil.h"

#pragma warning(disable : 4250) //inherit via dominance

namespace wasp::game::resources {

	class ManifestStorage
		: public resource::ParentResourceStorage
		, public resource::FileLoadable
		, public resource::ManifestLoadable
	{

	public:
		ManifestStorage()
			: FileLoadable{ { L"mnft" } }
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