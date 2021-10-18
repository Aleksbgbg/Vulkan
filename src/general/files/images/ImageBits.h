#ifndef VULKAN_SRC_GENERAL_FILES_IMAGEBITS_H
#define VULKAN_SRC_GENERAL_FILES_IMAGEBITS_H

#include <vector>

#include "util/types.h"

struct ImageBits {
  u32 width;
  u32 height;
  u32 bytesPerPixel;

  u32 size;

  std::vector<u8> data;
};

#endif  // VULKAN_SRC_GENERAL_FILES_IMAGEBITS_H
