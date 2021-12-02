#include <gtest/gtest.h>

#include "general/files/images/bmp.h"

TEST(ReadBmpTest, ReadsBasicBitmap) {
  // 2x2 pixels: red-green-blue-white (alpha = 255)
  const file::Image expectedImage{.width = 2,
                                  .height = 2,
                                  .bytesPerPixel = 4,
                                  .scanlineSize = 2 * 4,
                                  .size = 2 * 2 * 4,
                                  .data = {255, 0, 0, 255, 0, 255, 0, 255, 0, 0,
                                           255, 255, 255, 255, 255, 255}};

  const file::Image actualImage =
      file::ReadBitmap("data/images/BasicBitmap.bmp");

  ASSERT_EQ(expectedImage.width, actualImage.width);
  ASSERT_EQ(expectedImage.height, actualImage.height);
  ASSERT_EQ(expectedImage.bytesPerPixel, actualImage.bytesPerPixel);
  ASSERT_EQ(expectedImage.scanlineSize, actualImage.scanlineSize);
  ASSERT_EQ(expectedImage.size, actualImage.size);
  ASSERT_EQ(expectedImage.data, actualImage.data);
}
