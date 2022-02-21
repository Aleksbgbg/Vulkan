#include "Bitmap.h"

#include <stdexcept>
#include <string>

BitsPerPixel::BitsPerPixel(u32 value) : value(value) {}

BytesPerPixel::BytesPerPixel(u32 value) : value(value) {}

Offset::Offset(u32 x, u32 y) : x(x), y(y) {}

Bitmap::Bitmap(u32 width, u32 height, struct BitsPerPixel format)
    : Bitmap(width, height, format.value / 8) {
  if ((format.value % 8) != 0) {
    throw std::runtime_error(
        std::string("Uneven bits per pixel for bitmap (expected 8, got ") +
        std::to_string(format.value) + ")");
  }
}

Bitmap::Bitmap(u32 width, u32 height, struct BytesPerPixel format)
    : Bitmap(width, height, format.value) {}

Bitmap::Bitmap(u32 width, u32 height, u32 stride, const u8* memory,
               struct BytesPerPixel format)
    : Bitmap(width, height, format.value) {
  u8* write = Data();
  for (u32 y = 0; y < height_; ++y) {
    for (u32 x = 0; x < width_; ++x) {
      for (u32 byte = 0; byte < format.value; ++byte) {
        const u8 value = memory[(y * stride) + (x * format.value) + byte];
        write[(y * stride_) + (x * format.value) + byte] = value;
      }
    }
  }
}

Bitmap::Bitmap(u32 width, u32 height, u32 bytesPerPixel)
    : width_(width),
      height_(height),
      bytesPerPixel_(bytesPerPixel),
      stride_(width * bytesPerPixel),
      data_(height * stride_) {}

void Bitmap::Copy(const Bitmap& source, Bitmap& target, Offset targetOffset) {
  u8* const write = target.Data() + (target.BytesPerPixel() * targetOffset.x);
  for (u32 y = 0; y < source.Height(); ++y) {
    for (u32 x = 0; x < source.Width(); ++x) {
      const u8 color =
          source.Data()[((y + targetOffset.y) * source.Stride()) + x];
      write[(y * target.Stride()) + x] = color;
    }
  }
}

u32 Bitmap::Width() const {
  return width_;
}

u32 Bitmap::Height() const {
  return height_;
}

std::size_t Bitmap::Size() const {
  return data_.size();
}

u32 Bitmap::Stride() const {
  return stride_;
}

u32 Bitmap::BytesPerPixel() const {
  return bytesPerPixel_;
}

const u8* Bitmap::Data() const {
  return data_.data();
}

u8* Bitmap::Data() {
  return data_.data();
}
