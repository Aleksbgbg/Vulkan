#ifndef VULKAN_SRC_CORE_MATH_RANGE_H_
#define VULKAN_SRC_CORE_MATH_RANGE_H_

// Range [min, max]
template <typename T>
struct Range {
  constexpr Range(const T min, const T max) : min(min), max(max) {}

  T min;
  T max;
};

#endif  // VULKAN_SRC_CORE_MATH_RANGE_H_
