#include "FileUtil.h"

#include "framework.h"
#include <filesystem>
#include <functional>

#include "FileError.h"

namespace wasp::file {

	std::wstring getFileName(const std::wstring& fileName){
		throwIfFileDoesNotExist(fileName);
		auto cStringFileName{ fileName.c_str() };
		std::wstring toRet{ PathFindFileNameW(cStringFileName) };

		//remove extension
		std::wstring extension{ PathFindExtensionW(cStringFileName) };
		if (!extension.empty()) {
			toRet.erase(toRet.size() - extension.size(), extension.length());
		}

		return toRet;
	}

	std::wstring getFileExtension(const std::wstring& fileName){
		throwIfFileDoesNotExist(fileName);
		auto cStringFileName{ fileName.c_str() };
		std::wstring extension{ PathFindExtensionW(cStringFileName) };
		if (extension.empty()) {
			//check to see if file is directory
			if (PathIsDirectoryW(cStringFileName)) {
				return directoryExtension;
			}
			//if file has no extension but is also not directory, let caller handle
			return extension;
		}
		if (extension[0] == '.') {
			extension.erase(0, 1);
		}
		return extension;
	}

	void forEachDirectoryEntry(
		const std::wstring& directoryName,
		std::function<void(const std::wstring& fileName)> callBackFunction
	) {
		const std::filesystem::path path{ directoryName };
		for (auto const& dir_entry : std::filesystem::directory_iterator{ path }) {
			callBackFunction(dir_entry.path().wstring());
		}
	}

	void throwIfFileDoesNotExist(const std::wstring& fileName){
		if (!PathFileExistsW(fileName.c_str())) {
			throw FileError{"File not found"};
		}
	}
}