#include"FileUtility.h"
#include<filesystem>
#include<stdexcept>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

FileUtility::FileUtility()
{
}

FileUtility::~FileUtility()
{
}

std::vector<std::string>* FileUtility::GetAllFileNamesWithinFolder(std::string folder)
{
    std::vector<std::string>* names = new std::vector<std::string>();
    for (const auto& file : std::filesystem::directory_iterator(folder))
        names->push_back(file.path().generic_string());
    return names;
}

bool FileUtility::LoadImage(std::vector<unsigned char>& image, const std::string& filename, int& x, int& y)
{
    int n;
    unsigned char* data = stbi_load(filename.c_str(), &x, &y, &n, 4);
    if (data != nullptr)
    {
        image = std::vector<unsigned char>(data, data + x * y * 4);
    }
    stbi_image_free(data);
    return (data != nullptr);
}

std::vector<std::vector<double>*>* FileUtility::RGBAVectorFromImage(std::string filename)
{
    std::vector<std::vector<double>*>* result = new std::vector<std::vector<double>*>();
    int width, height;
    std::vector<unsigned char> image;
    bool success = LoadImage(image, filename, width, height);
    if (!success)
    {
        throw new std::runtime_error("Error loading image.");
    }

    const size_t RGBA = 4;
    result->reserve(static_cast<size_t>(width) * static_cast<size_t>(height));

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            std::vector<double>* rgbaRecord = new std::vector<double>();
            rgbaRecord->reserve(RGBA);
            size_t index = RGBA * (y * width + x);
            rgbaRecord->push_back(static_cast<double>(static_cast<int>(image[index + 0])));
            rgbaRecord->push_back(static_cast<double>(static_cast<int>(image[index + 1])));
            rgbaRecord->push_back(static_cast<double>(static_cast<int>(image[index + 2])));
            rgbaRecord->push_back(static_cast<double>(static_cast<int>(image[index + 3])));
            result->push_back(rgbaRecord);
        }
    }
    return result;
}

std::vector<double>* FileUtility::FlattenedRGBAVectorFromImage(std::string filename)
{
    int width, height;
    std::vector<unsigned char> image;
    bool success = LoadImage(image, filename, width, height);
    if (!success)
    {
        throw std::runtime_error("Error loading image.");
    }

    const size_t RGBA = 4;
    std::vector<double>* result = new std::vector<double>();
    result->reserve(static_cast<size_t>(width) * static_cast<size_t>(height) * RGBA);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            size_t index = RGBA * (y * width + x);
            result->push_back(static_cast<double>(static_cast<int>(image[index + 0])));
            result->push_back(static_cast<double>(static_cast<int>(image[index + 1])));
            result->push_back(static_cast<double>(static_cast<int>(image[index + 2])));
            result->push_back(static_cast<double>(static_cast<int>(image[index + 3])));
        }
    }
    return result;
}
