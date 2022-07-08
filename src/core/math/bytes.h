#ifndef SRC_CORE_MATH_BYTES_H_
#define SRC_CORE_MATH_BYTES_H_

#include <cassert>
#include <type_traits>

#include "Range.h"
#include "bits.h"
#include "core/types.h"

template <typename T, typename = std::enable_if_t<sizeof(T) == 1>>
constexpr u32 BytesToInt(const T* const bytes) {
  return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

constexpr u32 ExtractBytes(const u32 value, const Range<u32> range) {
  assert(range.min > 0);
  const u32 min = range.min - 1;
  return (value >> min) & ValueOfBits(range.max - min);
}

#endif  // SRC_CORE_MATH_BYTES_H_
