#ifndef VULKAN_SRC_GENERAL_FILES_PNG_H
#define VULKAN_SRC_GENERAL_FILES_PNG_H

#include <string_view>

#include "Image.h"

namespace file {

Image ReadPng(const std::string_view path);

}  // namespace file

#endif  // VULKAN_SRC_GENERAL_FILES_PNG_H
