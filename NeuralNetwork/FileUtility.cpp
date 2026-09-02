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

void FileUtility::LoadAndDownsampleImage(const std::string& filename, int targetWidth, int targetHeight, std::vector<unsigned char>& image, int& width, int& height)
{
    bool success = LoadImage(image, filename, width, height);
    if (!success)
    {
        throw new std::runtime_error("Error loading image.");
    }
    if (targetWidth != 0 || targetHeight != 0)
    {
        if (targetWidth <= 0 || targetHeight <= 0)
            throw std::invalid_argument("targetWidth and targetHeight must both be provided together, and both greater than 0.");
        if (targetWidth > width || targetHeight > height)
            throw std::invalid_argument("targetWidth/targetHeight must each be <= the source image's dimensions; this downsamples, it does not upsample.");
        image = DownsampleImage(image, width, height, targetWidth, targetHeight);
        width = targetWidth;
        height = targetHeight;
    }
}

std::vector<unsigned char> FileUtility::DownsampleImage(const std::vector<unsigned char>& image, int srcWidth, int srcHeight, int targetWidth, int targetHeight)
{
    const size_t RGBA = 4;
    std::vector<unsigned char> result(static_cast<size_t>(targetWidth) * static_cast<size_t>(targetHeight) * RGBA);

    for (int ty = 0; ty < targetHeight; ty++)
    {
        int srcYStart = ty * srcHeight / targetHeight;
        int srcYEnd = (ty + 1) * srcHeight / targetHeight;
        if (srcYEnd <= srcYStart)
            srcYEnd = srcYStart + 1;
        for (int tx = 0; tx < targetWidth; tx++)
        {
            int srcXStart = tx * srcWidth / targetWidth;
            int srcXEnd = (tx + 1) * srcWidth / targetWidth;
            if (srcXEnd <= srcXStart)
                srcXEnd = srcXStart + 1;

            unsigned long sums[4] = { 0, 0, 0, 0 };
            int count = 0;
            for (int sy = srcYStart; sy < srcYEnd; sy++)
            {
                for (int sx = srcXStart; sx < srcXEnd; sx++)
                {
                    size_t srcIndex = RGBA * (static_cast<size_t>(sy) * srcWidth + sx);
                    sums[0] += image[srcIndex + 0];
                    sums[1] += image[srcIndex + 1];
                    sums[2] += image[srcIndex + 2];
                    sums[3] += image[srcIndex + 3];
                    count++;
                }
            }
            size_t dstIndex = RGBA * (static_cast<size_t>(ty) * targetWidth + tx);
            result[dstIndex + 0] = static_cast<unsigned char>(sums[0] / count);
            result[dstIndex + 1] = static_cast<unsigned char>(sums[1] / count);
            result[dstIndex + 2] = static_cast<unsigned char>(sums[2] / count);
            result[dstIndex + 3] = static_cast<unsigned char>(sums[3] / count);
        }
    }
    return result;
}

std::vector<std::vector<double>*>* FileUtility::RGBAVectorFromImage(std::string filename, int targetWidth, int targetHeight)
{
    std::vector<std::vector<double>*>* result = new std::vector<std::vector<double>*>();
    int width, height;
    std::vector<unsigned char> image;
    LoadAndDownsampleImage(filename, targetWidth, targetHeight, image, width, height);

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

std::vector<double>* FileUtility::FlattenedRGBAVectorFromImage(std::string filename, int targetWidth, int targetHeight)
{
    int width, height;
    std::vector<unsigned char> image;
    LoadAndDownsampleImage(filename, targetWidth, targetHeight, image, width, height);

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
