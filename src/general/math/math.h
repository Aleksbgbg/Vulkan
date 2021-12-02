#ifndef VULKAN_SRC_GENERAL_MATH_MATH_H_
#define VULKAN_SRC_GENERAL_MATH_MATH_H_

#include <cmath>

template <typename T>
T CoerceToRange(const T value, const T min, const T max) {
  return std::min(max, std::max(min, value));
}

#endif  // VULKAN_SRC_GENERAL_MATH_MATH_H_
