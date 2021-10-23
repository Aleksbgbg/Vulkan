#ifndef VULKAN_SRC_READ_BITMAP_H
#define VULKAN_SRC_READ_BITMAP_H

#include <string_view>

#include "Image.h"

namespace file {

Image ReadBitmap(const std::string_view path);

}  // namespace file

#endif  // VULKAN_SRC_READ_BITMAP_H
