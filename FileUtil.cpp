#include "FileUtil.h"

#include "framework.h"

#include "FileError.h"

namespace file {
	std::wstring getFileName(const std::wstring& fileName){
		throwIfFileDoesNotExist(fileName);
		auto cStringFileName{ fileName.c_str() };
		std::wstring toRet{ PathFindFileNameW(cStringFileName) };
		std::wstring extension{ PathFindExtensionW(cStringFileName) };
		if (!extension.empty()) {
			//remove extension
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
				return DIRECTORY_EXTENSION;
			}
			//if file has no extension but is also not directory, let caller handle
			return extension;
		}
		if (extension[0] == '.') {
			extension.erase(0, 1);
		}
		return extension;
	}

	void throwIfFileDoesNotExist(const std::wstring& fileName){
		if (!PathFileExistsW(fileName.c_str())) {
			throw FileError{"File not found"};
		}
	}

	//todo: std filesystem can help out
}