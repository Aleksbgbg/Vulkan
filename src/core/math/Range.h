#ifndef SRC_CORE_MATH_RANGE_H_
#define SRC_CORE_MATH_RANGE_H_

// Range [min, max]
template <typename T>
struct Range {
  constexpr Range(const T min, const T max) : min(min), max(max) {}

  T min;
  T max;
};

#endif  // SRC_CORE_MATH_RANGE_H_
