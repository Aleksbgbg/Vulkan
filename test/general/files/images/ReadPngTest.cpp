#include <gtest/gtest.h>

#include "general/files/images/bmp.h"
#include "general/files/images/png.h"

TEST(ReadPngTest, ReadsBasicSrgbPng) {
  const file::Image expectedImage =
      file::ReadBitmap("data/images/SpaceshipTexture.bmp");

  const file::Image actualImage =
      file::ReadPng("data/images/SpaceshipTexture.png");

  ASSERT_EQ(expectedImage.width, actualImage.width);
  ASSERT_EQ(expectedImage.height, actualImage.height);
  ASSERT_EQ(expectedImage.bytesPerPixel, actualImage.bytesPerPixel);
  ASSERT_EQ(expectedImage.scanlineSize, actualImage.scanlineSize);
  ASSERT_EQ(expectedImage.size, actualImage.size);
  ASSERT_EQ(expectedImage.data, actualImage.data);
}

TEST(ReadPngTest, ReadsLargePng) {
  const file::Image expectedImage = file::ReadBitmap("data/images/Alice.bmp");

  const file::Image actualImage = file::ReadPng("data/images/Alice.png");

  ASSERT_EQ(expectedImage.width, actualImage.width);
  ASSERT_EQ(expectedImage.height, actualImage.height);
  ASSERT_EQ(expectedImage.bytesPerPixel, actualImage.bytesPerPixel);
  ASSERT_EQ(expectedImage.scanlineSize, actualImage.scanlineSize);
  ASSERT_EQ(expectedImage.size, actualImage.size);
  ASSERT_EQ(expectedImage.data, actualImage.data);
}
