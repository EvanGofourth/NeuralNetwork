// Milk.cpp contains only untemplated functions that utilize stb_image.h.

#include"Milk.hpp"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

bool VectorUtilities<double>::LoadImage(std::vector<unsigned char>& image, const std::string& filename, int& x, int& y)
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

std::vector<std::vector<double>*>* VectorUtilities<double>::RGBAVectorFromImage(std::string filename)
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

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            std::vector<double>* rgbaRecord = new std::vector<double>();
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