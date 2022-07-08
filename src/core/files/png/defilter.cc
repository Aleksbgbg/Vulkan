#include "defilter.h"

#include <cstring>
#include <memory>
#include <utility>

#include "core/strings/format.h"
#include "core/types.h"

namespace {

u8 PaethPredictor(const u8 a, const u8 b, const u8 c) {
  const i32 p = a + b - c;

  const i32 pa = std::abs(p - a);
  const i32 pb = std::abs(p - b);
  const i32 pc = std::abs(p - c);

  if ((pa <= pb) && (pa <= pc)) {
    return a;
  }

  if (pb <= pc) {
    return b;
  }

  return c;
}

class PngReconstructor {
 public:
  void Run() {
    for (u32 scanline = 0; scanline < height_; ++scanline) {
      const u8 filterType = *source_++;

      switch (filterType) {
        case 0:
          DecodeNoneFilter();
          break;

        case 1:
          DecodeSubFilter();
          break;

        case 2:
          DecodeUpFilter();
          break;

        case 3:
          DecodeAverageFilter();
          break;

        case 4:
          DecodePaethFilter();
          break;

        default:
          throw std::runtime_error(
              Format("Invalid PNG filter type (", filterType, ")"));
      }

      MoveToNextScanline();
    }
  }

 private:
  void MoveToNextScanline() {
    std::memcpy(sink_, currentScanline_ + bytesPerPixel_, stride_);

    std::swap(previousScanline_, currentScanline_);

    source_ += stride_;
    sink_ += stride_;
  }

  void DecodeNoneFilter() {
    std::memcpy(currentScanline_ + bytesPerPixel_, source_, stride_);
  }

  void DecodeSubFilter() {
    for (u32 byte = 0; byte < stride_; ++byte) {
      const u8 outputByte = source_[byte] + ReadPixelA(byte);
      WriteByte(byte, outputByte);
    }
  }

  void DecodeUpFilter() {
    for (u32 byte = 0; byte < stride_; ++byte) {
      const u8 outputByte = source_[byte] + ReadPixelB(byte);
      WriteByte(byte, outputByte);
    }
  }

  void DecodeAverageFilter() {
    for (u32 byte = 0; byte < stride_; ++byte) {
      const u8 a = ReadPixelA(byte);
      const u8 b = ReadPixelB(byte);

      const u16 sum = a + b;
      const u8 outputByte = source_[byte] + static_cast<u8>(sum / 2.0f);

      WriteByte(byte, outputByte);
    }
  }

  void DecodePaethFilter() {
    for (u32 byte = 0; byte < stride_; ++byte) {
      const u8 a = ReadPixelA(byte);
      const u8 b = ReadPixelB(byte);
      const u8 c = ReadPixelC(byte);

      const u8 outputByte = source_[byte] + PaethPredictor(a, b, c);

      WriteByte(byte, outputByte);
    }
  }

  u8 ReadPixelA(const u32 byte) const {
    return currentScanline_[byte];
  }

  u8 ReadPixelB(const u32 byte) const {
    return previousScanline_[bytesPerPixel_ + byte];
  }

  u8 ReadPixelC(const u32 byte) const {
    return previousScanline_[byte];
  }

  void WriteByte(const u32 byte, const u8 value) {
    currentScanline_[bytesPerPixel_ + byte] = value;
  }

 public:
  PngReconstructor(const u32 bytesPerPixel, const u32 stride, const u32 height,
                   const u32 virtualScanlineSize, const u8* source, u8* sink)
      : bytesPerPixel_(bytesPerPixel),
        stride_(stride),
        height_(height),
        source_(source),
        sink_(sink),
        virtualScanlineBuffer_(std::make_unique<u8[]>(virtualScanlineSize * 2)),
        previousScanline_(virtualScanlineBuffer_.get()),
        currentScanline_(previousScanline_ + virtualScanlineSize) {}

 private:
  u32 bytesPerPixel_;
  u32 stride_;
  u32 height_;
  const u8* source_;
  u8* sink_;
  std::unique_ptr<u8[]> virtualScanlineBuffer_;
  u8* previousScanline_;
  u8* currentScanline_;
};

}  // namespace

namespace file::png {

void Defilter(const u8* filteredPng, Bitmap& bitmap) {
  PngReconstructor(bitmap.BytesPerPixel(), bitmap.Stride(), bitmap.Height(),
                   bitmap.Stride() + bitmap.BytesPerPixel(), filteredPng,
                   bitmap.Data())
      .Run();
}

}  // namespace file::png
