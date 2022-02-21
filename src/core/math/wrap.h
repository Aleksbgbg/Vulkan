#ifndef VULKAN_SRC_CORE_MATH_WRAP_H_
#define VULKAN_SRC_CORE_MATH_WRAP_H_

// Wraps value in the range [min, max]
template <typename T>
constexpr T Wrap(const T value, const T min, T max) {
  max += static_cast<T>(1);
  const T valueToWrap = value - min;
  const T wrapBound = max - min;
  const T wrappedValue = ((valueToWrap % wrapBound) + wrapBound) % wrapBound;
  return min + wrappedValue;
}

template <typename T>
constexpr void Wrap(T* const value, const T min, const T max) {
  *value = Wrap(*value, min, max);
}

#endif  // VULKAN_SRC_CORE_MATH_WRAP_H_
