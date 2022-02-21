#include <gtest/gtest.h>

#include "core/files/images/bmp.h"
#include "core/files/images/png.h"
#include "matchers.h"

namespace {

using matchers::EqualsMemoryRegion;

}  // namespace

TEST(ReadPngTest, ReadsBasicSrgbPng) {
  const Bitmap expectedImage =
      file::ReadBitmap("data/images/SpaceshipTexture.bmp");

  const Bitmap actualImage = file::ReadPng("data/images/SpaceshipTexture.png");

  ASSERT_EQ(expectedImage.Width(), actualImage.Width());
  ASSERT_EQ(expectedImage.Height(), actualImage.Height());
  ASSERT_EQ(expectedImage.BytesPerPixel(), actualImage.BytesPerPixel());
  ASSERT_EQ(expectedImage.Stride(), actualImage.Stride());
  ASSERT_EQ(expectedImage.Size(), actualImage.Size());
  ASSERT_THAT(actualImage.Data(),
              EqualsMemoryRegion(expectedImage.Data(), expectedImage.Size()));
}

TEST(ReadPngTest, ReadsLargePng) {
  const Bitmap expectedImage = file::ReadBitmap("data/images/Alice.bmp");

  const Bitmap actualImage = file::ReadPng("data/images/Alice.png");

  ASSERT_EQ(expectedImage.Width(), actualImage.Width());
  ASSERT_EQ(expectedImage.Height(), actualImage.Height());
  ASSERT_EQ(expectedImage.BytesPerPixel(), actualImage.BytesPerPixel());
  ASSERT_EQ(expectedImage.Stride(), actualImage.Stride());
  ASSERT_EQ(expectedImage.Size(), actualImage.Size());
  ASSERT_THAT(actualImage.Data(),
              EqualsMemoryRegion(expectedImage.Data(), expectedImage.Size()));
}

TEST(ReadPngTest, ReadsPngUsingUpAndAverageFilter) {
  const Bitmap expectedImage = file::ReadBitmap("data/images/Space.bmp");

  const Bitmap actualImage = file::ReadPng("data/images/Space.png");

  ASSERT_EQ(expectedImage.Width(), actualImage.Width());
  ASSERT_EQ(expectedImage.Height(), actualImage.Height());
  ASSERT_EQ(expectedImage.BytesPerPixel(), actualImage.BytesPerPixel());
  ASSERT_EQ(expectedImage.Stride(), actualImage.Stride());
  ASSERT_EQ(expectedImage.Size(), actualImage.Size());
  ASSERT_THAT(actualImage.Data(),
              EqualsMemoryRegion(expectedImage.Data(), expectedImage.Size()));
}

TEST(ReadPngTest, ReadsPngUsingPaethAndSubFilters) {
  const Bitmap expectedImage = file::ReadBitmap("data/images/Nebula.bmp");

  const Bitmap actualImage = file::ReadPng("data/images/Nebula.png");

  ASSERT_EQ(expectedImage.Width(), actualImage.Width());
  ASSERT_EQ(expectedImage.Height(), actualImage.Height());
  ASSERT_EQ(expectedImage.BytesPerPixel(), actualImage.BytesPerPixel());
  ASSERT_EQ(expectedImage.Stride(), actualImage.Stride());
  ASSERT_EQ(expectedImage.Size(), actualImage.Size());
  ASSERT_THAT(actualImage.Data(),
              EqualsMemoryRegion(expectedImage.Data(), expectedImage.Size()));
}
