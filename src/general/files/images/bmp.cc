#include "bmp.h"

#include <stdexcept>

#include "general/files/file.h"

namespace file {

#pragma pack(push, r1, 1)
struct BitmapHeader {
  struct {
    struct {
      u8 char0;
      u8 char1;
    } fileType;

    u32 fileSize;

    i16 reserved0;
    i16 reserved1;

    u32 pixelDataOffset;
  } fileHeader;

  struct {
    u32 headerSize;

    i32 imageWidth;
    i32 imageHeight;

    u16 planes;

    u16 bitsPerPixel;

    u32 compression;
    u32 imageSize;

    i32 xPixelsPerMeter;
    i32 yPixelsPerMeter;

    u32 totalColors;
    u32 importantColors;
  } infoHeader;
};
#pragma pack(pop, r1)

Image ReadBitmap(const std::string_view path) {
  const std::vector<u8> fileData = ReadFile(path);

  const BitmapHeader bitmapHeader =
      *reinterpret_cast<const BitmapHeader*>(fileData.data());

  Image bitmap;
  bitmap.width = bitmapHeader.infoHeader.imageWidth;
  bitmap.height = bitmapHeader.infoHeader.imageHeight;
  bitmap.bytesPerPixel = bitmapHeader.infoHeader.bitsPerPixel / 8;
  bitmap.scanlineSize = bitmap.width * bitmap.bytesPerPixel;
  bitmap.size = bitmap.scanlineSize * bitmap.height;
  bitmap.data = std::vector<u8>(bitmap.size);

  if ((bitmapHeader.infoHeader.bitsPerPixel != 32) || (bitmap.height < 0)) {
    throw std::runtime_error("Bitmap type not implemented.");
  }

  const u32* const readDataPixels = reinterpret_cast<const u32* const>(
      fileData.data() + bitmapHeader.fileHeader.pixelDataOffset);
  u8* const writeDataBytes = bitmap.data.data();

  for (u32 y = 0; y < bitmap.height; ++y) {
    for (u32 x = 0; x < bitmap.width; ++x) {
      const u32 pixel = (y * bitmap.width) + x;

      // Read bitmap from bottom-left corner
      const u32 value =
          readDataPixels[((bitmap.height - y - 1) * bitmap.width) + x];

      // B and R in bitmap are reversed
      const u8 blue = value & 255;
      const u8 green = (value >> 8) & 255;
      const u8 red = (value >> 16) & 255;
      const u8 alpha = (value >> 24) & 255;

      u8* const writePixel = writeDataBytes + (pixel * bitmap.bytesPerPixel);

      writePixel[0] = red;
      writePixel[1] = green;
      writePixel[2] = blue;
      writePixel[3] = alpha;
    }
  }

  return bitmap;
}

}  // namespace file