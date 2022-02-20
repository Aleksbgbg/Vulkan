#ifndef VULKAN_SRC_GENERAL_MATH_CLAMP_H_
#define VULKAN_SRC_GENERAL_MATH_CLAMP_H_

#include <cmath>

template <typename T>
constexpr T Clamp(const T value, const T min, const T max) {
  return std::min(max, std::max(min, value));
}

template <typename T>
constexpr void Clamp(T* const value, const T min, const T max) {
  *value = Clamp(*value, min, max);
}

#endif  // VULKAN_SRC_GENERAL_MATH_CLAMP_H_
