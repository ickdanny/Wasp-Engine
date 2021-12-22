#pragma once

#include <string>
#include <vector>

#include "IResource.h"

namespace resource {

	using FileTypes = std::vector<std::string>;
	class ResourceMasterStorage;

	struct FileOrigin {
		const std::wstring& fileName{};
	};


	class FileLoadable {
	private:
		const FileTypes& fileTypes{};
	public:
		FileLoadable(const FileTypes& fileTypes)
			: fileTypes{fileTypes} {
		}
		virtual ~FileLoadable() = default;

		const FileTypes& getAcceptableFileTypes() const {
			return fileTypes;
		}
		virtual IResource loadFromFile(const FileOrigin& fileOrigin, ResourceMasterStorage& resourceMasterStorage) = 0;
	};
}