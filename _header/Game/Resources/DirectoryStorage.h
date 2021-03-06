#pragma once

#include "Resource\ParentResourceStorage.h"
#include "File\FileUtil.h"

#pragma warning(disable : 4250) //inherit via dominance

namespace wasp::game::resources {

	class DirectoryStorage
		: public resource::ParentResourceStorage
		, public resource::FileLoadable
		, public resource::ManifestLoadable
	{

	public:
		DirectoryStorage()
			: FileLoadable{ {file::directoryExtension} } 
			, ManifestLoadable{ {L"directory"} } {
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