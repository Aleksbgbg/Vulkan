#include "bits.h"

#include "core/literals.h"
#include "integer_math.h"

std::vector<u8> PackBitsLsbFirst(const std::vector<bool>& bits) {
  const u32 byteCount = DivideCeil(bits.size(), 8_sz);
  std::vector<u8> bytes(byteCount);

  auto bitIterator = std::begin(bits);

  for (u32 byte = 0; byte < byteCount; ++byte) {
    u8 value = 0;

    for (u32 bit = 0; bit < std::min(8_sz, bits.size() - (byte * 8)); ++bit) {
      value |= static_cast<u8>(*bitIterator) << bit;
      bitIterator = std::next(bitIterator);
    }

    bytes[byte] = value;
  }

  return bytes;
}
