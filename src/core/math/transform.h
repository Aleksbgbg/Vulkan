#ifndef VULKAN_SRC_CORE_MATH_TRANSFORM_H_
#define VULKAN_SRC_CORE_MATH_TRANSFORM_H_

#include "Range.h"

// Maps number x in the input range to a number in the output range
template <typename T>
constexpr T AffineTransform(const T x, const Range<T> input,
                            const Range<T> output) {
  return ((x - input.min) *
          ((output.max - output.min) / (input.max - input.min))) +
         output.min;
}

template <typename T>
constexpr void AffineTransform(T* const x, const Range<T> input,
                               const Range<T> output) {
  *x = AffineTransform(*x, input, output);
}

// Maps number x in the range [0, 1] to a number in the output range
template <typename T>
constexpr T AffineTransform(const T x, const Range<T> output) {
  return AffineTransform(x, Range(static_cast<T>(0), static_cast<T>(1)),
                         output);
}

template <typename T>
constexpr void AffineTransform(T* const x, const Range<T> output) {
  *x = AffineTransform(*x, output);
}

#endif  // VULKAN_SRC_CORE_MATH_TRANSFORM_H_
