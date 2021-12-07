#ifndef VULKAN_SRC_GENERAL_MATH_MATH_H_
#define VULKAN_SRC_GENERAL_MATH_MATH_H_

#include <cmath>

template <typename T>
constexpr T CoerceToRange(const T value, const T min, const T max) {
  return std::min(max, std::max(min, value));
}

// Maps number x in the range [a, b] to number y in the range [c, d]
template <typename T>
constexpr T AffineTransform(const T x, const T a, const T b, const T c,
                            const T d) {
  return ((x - a) * ((d - c) / (b - a))) + c;
}

#endif  // VULKAN_SRC_GENERAL_MATH_MATH_H_
