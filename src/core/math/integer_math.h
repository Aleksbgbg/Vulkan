#ifndef VULKAN_SRC_CORE_MATH_INTEGER_MATH_H_
#define VULKAN_SRC_CORE_MATH_INTEGER_MATH_H_

#include <type_traits>

template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, T> DivideCeil(const T a,
                                                                const T b) {
  return (a + (b - 1)) / b;
}

#endif  // VULKAN_SRC_CORE_MATH_INTEGER_MATH_H_
