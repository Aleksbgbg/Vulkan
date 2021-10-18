#include <gtest/gtest.h>

#include "general/files/images/bmp.h"
#include "general/files/images/png.h"

TEST(ReadPngTest, ReadsBasicSrgbPng) {
  const ImageBits expectedData = ReadBitmap("data/images/SpaceshipTexture.bmp");

  const ImageBits pngData = ReadPng("data/images/SpaceshipTexture.png");

  ASSERT_EQ(expectedData.width, pngData.width);
  ASSERT_EQ(expectedData.height, pngData.height);
  ASSERT_EQ(expectedData.bytesPerPixel, pngData.bytesPerPixel);
  ASSERT_EQ(expectedData.size, pngData.size);
  ASSERT_EQ(expectedData.data, pngData.data);
}

TEST(ReadPngTest, ReadsLargePng) {
  const ImageBits expectedData = ReadBitmap("data/images/Alice.bmp");

  ImageBits pngData = ReadPng("data/images/Alice.png");

  ASSERT_EQ(expectedData.width, pngData.width);
  ASSERT_EQ(expectedData.height, pngData.height);
  ASSERT_EQ(expectedData.bytesPerPixel, pngData.bytesPerPixel);
  ASSERT_EQ(expectedData.size, pngData.size);
  ASSERT_EQ(expectedData.data, pngData.data);
}
