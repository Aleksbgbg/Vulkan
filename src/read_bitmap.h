#ifndef VULKAN_SRC_READ_BITMAP_H
#define VULKAN_SRC_READ_BITMAP_H

#include <vector>

#include "types.h"

struct Bitmap {
  u32 width;
  u32 height;
  u32 bytesPerPixel;

  u32 size;

  std::vector<u8> data;
};

Bitmap ReadBitmap(const char* const path);

#endif  // VULKAN_SRC_READ_BITMAP_H
