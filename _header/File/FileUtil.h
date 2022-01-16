#pragma once

#include <string>
#include <functional>

namespace wasp::file{
    const std::wstring directoryExtension{ L"directory" };

    std::wstring getFileName(const std::wstring& fileName);

    std::wstring getFileExtension(const std::wstring& fileName);

    void forEachDirectoryEntry(
        const std::wstring& directoryName,
        std::function<void(const std::wstring& fileName)> callBackFunction
    );

    void throwIfFileDoesNotExist(const std::wstring& fileName);


    /*
    * //todo: ini file?

    public static Properties parseProperties(File propertiesFile);

    public static void writeProperties(Properties properties, File propertiesFile);
    */
}