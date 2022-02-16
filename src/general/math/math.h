#ifndef VULKAN_SRC_GENERAL_MATH_MATH_H_
#define VULKAN_SRC_GENERAL_MATH_MATH_H_

#include <cmath>

template <typename T>
constexpr T Clamp(const T value, const T min, const T max) {
  return std::min(max, std::max(min, value));
}

template <typename T>
constexpr void Clamp(T* const value, const T min, const T max) {
  *value = std::min(max, std::max(min, *value));
}

// Wraps value in the range [min, max]
template <typename T>
constexpr T Wrap(const T value, const T min, T max) {
  max += 1;
  const T wrapBound = max - min;
  const T valueToWrap = value - min;
  const T wrappedValue = ((valueToWrap % wrapBound) + wrapBound) % wrapBound;
  return min + wrappedValue;
}

// Maps number x in the range [a, b] to number y in the range [c, d]
template <typename T>
constexpr T AffineTransform(const T x, const T a, const T b, const T c,
                            const T d) {
  return ((x - a) * ((d - c) / (b - a))) + c;
}

#endif  // VULKAN_SRC_GENERAL_MATH_MATH_H_
