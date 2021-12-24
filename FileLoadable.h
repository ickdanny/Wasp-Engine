#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Loadable.h"
#include "IResource.h"

namespace resource {

	using FileTypes = std::vector<std::wstring>;
	class ResourceLoader;

	struct FileOrigin {
		std::wstring fileName{};
	};


	class FileLoadable : virtual public Loadable{
	private:
		FileTypes fileTypes{};
	public:
		FileLoadable(const FileTypes& fileTypes)
			: fileTypes{fileTypes} {
		}
		virtual ~FileLoadable() = default;

		bool isFileLoadable() const override {
			return true;
		}

		const FileTypes& getAcceptableFileTypes() const {
			return fileTypes;
		}
		virtual IResource* loadFromFile(
			const FileOrigin& fileOrigin, 
			const ResourceLoader& resourceLoader
		) = 0;
	};
}