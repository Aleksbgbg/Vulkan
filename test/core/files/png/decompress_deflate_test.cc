#include "core/files/png/decompress_deflate.h"

#include <gtest/gtest.h>

#include "core/math/bits.h"

namespace {

static constexpr u32 kOutputSize = 1024;

}  // namespace

TEST(DecompressDeflateTest, FixedCodeEmitSimpleValue) {
  const std::vector<u8> content = PackBitsLsbFirst({
      1,                       // Final block (true)
      1, 0,                    // Fixed block (1)
      0, 0, 1, 1, 0, 0, 0, 1,  // 1
      0, 0, 0, 0, 0, 0, 0,     // 256 (end)
  });
  std::unique_ptr<u8[]> output = std::make_unique<u8[]>(kOutputSize);
  BitStreamReader bitStreamReader(content.data());
  ByteStreamReadWriter byteStreamReadWriter(output.get());

  file::png::DecompressDeflate(bitStreamReader, byteStreamReadWriter);

  ASSERT_EQ(output[0], 1);
}
