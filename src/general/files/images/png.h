#ifndef VULKAN_SRC_GENERAL_FILES_PNG_H
#define VULKAN_SRC_GENERAL_FILES_PNG_H

#include <string_view>

#include "ImageBits.h"

ImageBits ReadPng(const std::string_view path);

#endif  // VULKAN_SRC_GENERAL_FILES_PNG_H
