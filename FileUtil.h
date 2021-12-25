#pragma once

#include <string>
#include <functional>

namespace file{
    const std::wstring directoryExtension{ L"directory" };

    std::wstring getFileName(const std::wstring& fileName);

    std::wstring getFileExtension(const std::wstring& fileName);

    void forEachDirectoryEntry(
        const std::wstring& directoryName,
        std::function<void(const std::wstring& fileName)> callBackFunction
    );

    void throwIfFileDoesNotExist(const std::wstring& fileName);


    /*
    * //todo: translate the following; move from BitmapManager
    public static BufferedImage parseImage(File imageFile); 

    public static Properties parseProperties(File propertiesFile);

    public static void writeProperties(Properties properties, File propertiesFile);
    */
}