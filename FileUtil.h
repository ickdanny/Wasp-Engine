#pragma once

#include <string>
#include <fstream>

namespace fileutil{
    constexpr std::string_view DIRECTORY_EXTENSION{ "directory" };

    //todo: translate the following

    std::wstring getFilePath(File file);

    std::wstring getFileName(File file);

    std::wstring getFileExtension(File file);

    public static BufferedImage parseImage(File imageFile); //todo: move from BitmapManager

    public static Properties parseProperties(File propertiesFile);

    public static void writeProperties(Properties properties, File propertiesFile);
}