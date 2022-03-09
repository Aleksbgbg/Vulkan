#include <gtest/gtest.h>

#include "core/files/bmp.h"
#include "matchers.h"

namespace {

using matchers::EqualsMemoryRegion;

}  // namespace

TEST(ReadBmpTest, ReadsBasicBitmap) {
  // 2x2 pixels: red-green-blue-white (alpha = 255)
  const std::vector<u8> image{255, 0, 0,   255, 0,   255, 0,   255,
                              0,   0, 255, 255, 255, 255, 255, 255};
  const Bitmap expectedImage(2, 2, 2 * 4, image.data(), BytesPerPixel(4));

  const Bitmap actualImage = file::ReadBitmap("data/images/BasicBitmap.bmp");

  ASSERT_EQ(expectedImage.Width(), actualImage.Width());
  ASSERT_EQ(expectedImage.Height(), actualImage.Height());
  ASSERT_EQ(expectedImage.BytesPerPixel(), actualImage.BytesPerPixel());
  ASSERT_EQ(expectedImage.Stride(), actualImage.Stride());
  ASSERT_EQ(expectedImage.Size(), actualImage.Size());
  ASSERT_THAT(actualImage.Data(),
              EqualsMemoryRegion(expectedImage.Data(), expectedImage.Size()));
}
