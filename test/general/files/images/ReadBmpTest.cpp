#include <gtest/gtest.h>

#include "general/files/images/bmp.h"

TEST(ReadBmpTest, ReadsBasicBitmap) {
  // 2x2; pixels: red-green-blue-white (alpha = 255)
  const ImageBits expectedData{.width = 2,
                               .height = 2,
                               .bytesPerPixel = 4,
                               .size = 2 * 2 * 4,
                               .data = {255, 0, 0, 255, 0, 255, 0, 255, 0, 0,
                                        255, 255, 255, 255, 255, 255}};

  const ImageBits actualData = ReadBitmap("data/images/BasicBitmap.bmp");

  ASSERT_EQ(expectedData.width, actualData.width);
  ASSERT_EQ(expectedData.height, actualData.height);
  ASSERT_EQ(expectedData.bytesPerPixel, actualData.bytesPerPixel);
  ASSERT_EQ(expectedData.size, actualData.size);
  ASSERT_EQ(expectedData.data, actualData.data);
}
