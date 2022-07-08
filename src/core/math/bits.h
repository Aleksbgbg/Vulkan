#ifndef SRC_CORE_MATH_BITS_H_
#define SRC_CORE_MATH_BITS_H_

#include <vector>

#include "core/types.h"

constexpr std::size_t ValueOfBits(const std::size_t bits) {
  return (0b1ull << bits) - 1;
}

constexpr bool LowerBitsEqual(const std::size_t bits, const std::size_t a,
                              const std::size_t b) {
  const std::size_t bitSet = ValueOfBits(bits);
  return (a & bitSet) == (b & bitSet);
}

std::vector<u8> PackBitsLsbFirst(const std::vector<bool>& bits);

#endif  // SRC_CORE_MATH_BITS_H_
