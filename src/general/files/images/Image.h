#ifndef VULKAN_SRC_GENERAL_FILES_IMAGES_IMAGE_H_
#define VULKAN_SRC_GENERAL_FILES_IMAGES_IMAGE_H_

#include <vector>

#include "util/types.h"

namespace file {

struct Image {
  u32 width;
  u32 height;
  u32 bytesPerPixel;

  u32 scanlineSize;
  u32 size;

  std::vector<u8> data;
};

}  // namespace file

#endif  // VULKAN_SRC_GENERAL_FILES_IMAGES_IMAGE_H_
