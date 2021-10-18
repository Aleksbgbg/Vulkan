#ifndef VULKAN_SRC_READ_BITMAP_H
#define VULKAN_SRC_READ_BITMAP_H

#include <string_view>

#include "ImageBits.h"

ImageBits ReadBitmap(const std::string_view path);

#endif  // VULKAN_SRC_READ_BITMAP_H
