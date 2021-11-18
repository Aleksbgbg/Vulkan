#ifndef VULKAN_SRC_GENERAL_MATH_MATH_H
#define VULKAN_SRC_GENERAL_MATH_MATH_H

#include <cmath>

template <typename T>
T CoerceToRange(const T value, const T min, const T max) {
  return std::min(max, std::max(min, value));
}

#endif  // VULKAN_SRC_GENERAL_MATH_MATH_H
