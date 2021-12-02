#ifndef VULKAN_SRC_GENERAL_FILES_IMAGES_PNG_H_
#define VULKAN_SRC_GENERAL_FILES_IMAGES_PNG_H_

#include <string_view>

#include "Image.h"

namespace file {

Image ReadPng(const std::string_view path);

}  // namespace file

#endif  // VULKAN_SRC_GENERAL_FILES_IMAGES_PNG_H_
