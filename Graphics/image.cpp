#include "image.h"

#include <iostream>

Image::Image()
{
}

Image::~Image() {
    ilDeleteImage(m_imageId);
}

bool Image::loadFromFile(std::string filename)
{
    ilInit();

     // The image name to return.
    ilGenImages(1, &m_imageId); // Grab a new image name.

    ilBindImage(m_imageId);

    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    if (ilLoadImage(filename.c_str()) != IL_TRUE) {
        std::cerr << "Error loading image from: " << filename << '\n';
        return false;
    }

    m_data = ilGetData();

    m_width = ilGetInteger(IL_IMAGE_WIDTH);
    m_height = ilGetInteger(IL_IMAGE_HEIGHT);

    m_bytesPerPixel = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

    return true;
}