#ifndef VULKAN_SRC_CORE_IMAGES_BITMAP_H_
#define VULKAN_SRC_CORE_IMAGES_BITMAP_H_

#include <vector>

#include "core/math/Rect.h"
#include "core/types.h"

struct BitsPerPixel {
  friend class Bitmap;

 public:
  explicit BitsPerPixel(u32 value);

 private:
  u32 value;
};

struct BytesPerPixel {
  friend class Bitmap;

 public:
  explicit BytesPerPixel(u32 value);

 private:
  u32 value;
};

struct Offset {
  friend class Bitmap;

 public:
  Offset(u32 x, u32 y);

 private:
  u32 x;
  u32 y;
};

class Bitmap {
 public:
  Bitmap() = default;
  Bitmap(u32 width, u32 height, BitsPerPixel format);
  Bitmap(u32 width, u32 height, BytesPerPixel format);
  Bitmap(u32 width, u32 height, u32 stride, const u8* memory,
         BytesPerPixel format);

  static void Copy(const Bitmap& source, Bitmap& target, Offset targetOffset);

  u32 Width() const;
  u32 Height() const;
  std::size_t Size() const;

  u32 Stride() const;
  u32 BytesPerPixel() const;

  const u8* Data() const;
  u8* Data();

 private:
  Bitmap(u32 width, u32 height, u32 bytesPerPixel);

 private:
  u32 width_;
  u32 height_;
  u32 bytesPerPixel_;
  u32 stride_;
  std::vector<u8> data_;
};

#endif  // VULKAN_SRC_CORE_IMAGES_BITMAP_H_
