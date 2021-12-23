#pragma once

#include <string>

namespace file{
    const std::wstring DIRECTORY_EXTENSION{ L"directory" };

    std::wstring getFileName(const std::wstring& fileName);

    std::wstring getFileExtension(const std::wstring& fileName);

    void throwIfFileDoesNotExist(const std::wstring& fileName);


    /*
    * //todo: translate the following; move from BitmapManager
    public static BufferedImage parseImage(File imageFile); 

    public static Properties parseProperties(File propertiesFile);

    public static void writeProperties(Properties properties, File propertiesFile);
    */
}