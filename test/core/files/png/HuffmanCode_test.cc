#include "core/files/png/HuffmanCode.h"

#include <gtest/gtest.h>

#include "core/math/bits.h"

namespace {

u32 DecodeBitStream(const HuffmanCode& code, const std::vector<bool>& bits) {
  const std::vector<u8> byteStream = PackBitsLsbFirst(bits);
  BitStreamReader bitStream(byteStream.data());
  return code.DecodeNextValue(bitStream);
}

}  // namespace

TEST(HuffmanCodeTest, EncodesSimpleAlphabetAndBitLengths) {
  const std::vector<u32> alphabet = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
  const std::vector<u32> bitLengths = {3, 3, 3, 3, 3, 2, 4, 4};

  const HuffmanCode code = HuffmanCode::Build(alphabet, bitLengths);

  ASSERT_EQ('A', DecodeBitStream(code, {0, 1, 0}));
  ASSERT_EQ('B', DecodeBitStream(code, {0, 1, 1}));
  ASSERT_EQ('C', DecodeBitStream(code, {1, 0, 0}));
  ASSERT_EQ('D', DecodeBitStream(code, {1, 0, 1}));
  ASSERT_EQ('E', DecodeBitStream(code, {1, 1, 0}));
  ASSERT_EQ('F', DecodeBitStream(code, {0, 0}));
  ASSERT_EQ('G', DecodeBitStream(code, {1, 1, 1, 0}));
  ASSERT_EQ('H', DecodeBitStream(code, {1, 1, 1, 1}));
}

TEST(HuffmanCodeTest, SortsAlphabetLexicographically) {
  const std::vector<u32> alphabet = {16, 17, 18, 0, 8,  7, 9,  6, 10, 5,
                                     11, 4,  12, 3, 13, 2, 14, 1, 15};
  const std::vector<u32> bitLengths = {2, 6, 0, 5, 3, 5, 3, 6, 3,
                                       6, 3, 5, 5, 5, 0, 6, 5};

  const HuffmanCode code = HuffmanCode::Build(alphabet, bitLengths);

  ASSERT_EQ(11, DecodeBitStream(code, {1, 0, 1}));
  ASSERT_EQ(14, DecodeBitStream(code, {1, 1, 1, 0, 1}));
  ASSERT_EQ(0, DecodeBitStream(code, {1, 1, 0, 0, 0}));
  ASSERT_EQ(12, DecodeBitStream(code, {1, 1, 1, 0, 0}));
  ASSERT_EQ(17, DecodeBitStream(code, {1, 1, 1, 1, 1, 1}));
}
