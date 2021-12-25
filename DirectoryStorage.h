#pragma once

#include "ParentResourceStorage.h"
#include "FileUtil.h"

namespace gameresource {

	class DirectoryStorage
		: public ParentResourceStorage
		, public resource::FileLoadable
	{

	public:
		DirectoryStorage()
			: FileLoadable{ {file::directoryExtension} } {
		}

		void reload(const std::wstring& id) override;

		resource::IResource* loadFromFile(
			const resource::FileOrigin& fileOrigin,
			const resource::ResourceLoader& resourceLoader
		) override;
	};
}