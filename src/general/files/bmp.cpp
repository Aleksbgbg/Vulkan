#include "bmp.h"

#include "file.h"

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

Bitmap ReadBitmap(const std::string_view path) {
  const std::vector<u8> fileData = ReadFile(path);

  const BitmapHeader bitmapHeader =
      *reinterpret_cast<const BitmapHeader*>(fileData.data());

  Bitmap bitmap;
  bitmap.width = bitmapHeader.infoHeader.imageWidth;
  bitmap.height = bitmapHeader.infoHeader.imageHeight;
  bitmap.bytesPerPixel = bitmapHeader.infoHeader.bitsPerPixel / 8;
  bitmap.size = bitmap.width * bitmap.height * bitmap.bytesPerPixel;
  bitmap.data = std::vector<u8>(bitmap.size);

  if (bitmap.height < 0) {
    std::memcpy(bitmap.data.data(),
                fileData.data() + bitmapHeader.fileHeader.pixelDataOffset,
                bitmap.size);
    bitmap.height = -bitmap.height;
  } else {
    const u32 bitmapPixels = bitmap.width * bitmap.height;
    const u32* const pixelData = reinterpret_cast<const u32* const>(
        fileData.data() + bitmapHeader.fileHeader.pixelDataOffset);
    u8* targetData = bitmap.data.data();

    for (u32 pixel = 0; pixel < bitmapPixels; ++pixel) {
      const u32 value = pixelData[pixel];

      const u8 red = value & 255;
      const u8 green = (value & (255 << 8)) >> 8;
      const u8 blue = (value & (255 << 16)) >> 16;
      const u8 alpha = (value & (255 << 24)) >> 24;

      u8* targetColors = targetData + (pixel * bitmap.bytesPerPixel);

      targetColors[0] = blue;
      targetColors[1] = green;
      targetColors[2] = red;
      targetColors[3] = alpha;
    }
  }

  return bitmap;
}
