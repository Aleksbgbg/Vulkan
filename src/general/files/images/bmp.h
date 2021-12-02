#ifndef VULKAN_SRC_GENERAL_FILES_IMAGES_BMP_H_
#define VULKAN_SRC_GENERAL_FILES_IMAGES_BMP_H_

#include <string_view>

#include "Image.h"

namespace file {

Image ReadBitmap(const std::string_view path);

}  // namespace file

#endif  // VULKAN_SRC_GENERAL_FILES_IMAGES_BMP_H_
