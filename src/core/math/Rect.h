#ifndef VULKAN_SRC_CORE_MATH_RECT_H_
#define VULKAN_SRC_CORE_MATH_RECT_H_

#include "core/types.h"
#include "util/include/glm.h"

template <typename T, typename TVec>
class Rect {
 private:
  constexpr Rect(const T x1, const T y1, const T x2, const T y2)
      : x1_(x1), y1_(y1), x2_(x2), y2_(y2) {}

 public:
  using Vec = TVec;

  constexpr Rect() = default;

  template <typename TOther, typename TVecOther>
  constexpr Rect(const Rect<TOther, TVecOther> other)
      : Rect(static_cast<T>(other.X1()), static_cast<T>(other.Y1()),
             static_cast<T>(other.X2()), static_cast<T>(other.Y2())) {}

  constexpr static Rect FromPoints(T x1, T y1, T x2, T y2) {
    if (x1 > x2) {
      std::swap(x1, x2);
    }

    if (y1 > y2) {
      std::swap(y1, y2);
    }

    return Rect(x1, y1, x2, y2);
  }
  constexpr static Rect FromPoints(const TVec point1, const TVec point2) {
    return FromPoints(point1.x, point1.y, point2.x, point2.y);
  }
  constexpr static Rect FromPoints(const T x1, const T y1, const TVec point2) {
    return FromPoints(x1, y1, point2.x, point2.y);
  }
  constexpr static Rect FromPoints(const TVec point1, const T x2, const T y2) {
    return FromPoints(point1.x, point1.y, x2, y2);
  }

  constexpr static Rect FromRegion(const T x, const T y, const T width,
                                   const T height) {
    return FromPoints(x, y, x + width, y + height);
  }
  constexpr static Rect FromRegion(const TVec point, const TVec size) {
    return FromPoints(point.x, point.y, point.x + size.x, point.y + size.y);
  }
  constexpr static Rect FromRegion(const T x, const T y, const TVec size) {
    return FromPoints(x, y, x + size.x, y + size.y);
  }
  constexpr static Rect FromRegion(const TVec point, const T width,
                                   const T height) {
    return FromPoints(point.x, point.y, point.x + width, point.y + height);
  }

  constexpr static Rect Zero() {
    return Rect(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
                static_cast<T>(0));
  }

  constexpr T X() const {
    return x1_;
  }
  constexpr T Y() const {
    return y1_;
  }
  constexpr T Width() const {
    return x2_ - x1_;
  }
  constexpr T Height() const {
    return y2_ - y1_;
  }

  constexpr T X1() const {
    return x1_;
  }
  constexpr T Y1() const {
    return y1_;
  }
  constexpr T X2() const {
    return x2_;
  }
  constexpr T Y2() const {
    return y2_;
  }

  constexpr T Left() const {
    return x1_;
  }
  constexpr T Right() const {
    return x2_;
  }
  constexpr T Top() const {
    return y1_;
  }
  constexpr T Bottom() const {
    return y2_;
  }

  constexpr TVec Point() const {
    return TVec(x1_, y1_);
  }
  constexpr TVec Size() const {
    return TVec(Width(), Height());
  }

  constexpr TVec Point1() const {
    return TVec(x1_, y1_);
  }
  constexpr TVec Point2() const {
    return TVec(x2_, y2_);
  }

  constexpr bool ContainsPoint(const T x, const T y) const {
    return (x1_ <= x) && (x <= x2_) && (y1_ <= y) && (y <= y2_);
  }
  constexpr bool ContainsPoint(const TVec vec) const {
    return ContainsPoint(vec.x, vec.y);
  }

  constexpr Rect operator+(const T value) const {
    return FromPoints(x1_ + value, y1_ + value, x2_ + value, y2_ + value);
  }
  constexpr Rect operator-(const T value) const {
    return FromPoints(x1_ - value, y1_ - value, x2_ - value, y2_ - value);
  }
  constexpr Rect operator+(const Rect rect) const {
    return FromPoints(x1_ + rect.x1_, y1_ + rect.y1_, x2_ + rect.x2_,
                      y2_ + rect.y2_);
  }
  constexpr Rect operator-(const Rect rect) const {
    return FromPoints(x1_ - rect.x1_, y1_ - rect.y1_, x2_ - rect.x2_,
                      y2_ - rect.y2_);
  }

  constexpr Rect operator*(const T value) const {
    return FromPoints(x1_ * value, y1_ * value, x2_ * value, y2_ * value);
  }
  constexpr Rect operator/(const T value) const {
    return FromPoints(x1_ / value, y1_ / value, x2_ / value, y2_ / value);
  }
  constexpr Rect operator*(const TVec vec) const {
    return FromPoints(x1_ * vec.x, y1_ * vec.y, x2_ * vec.x, y2_ * vec.y);
  }
  constexpr Rect operator/(const TVec vec) const {
    return FromPoints(x1_ / vec.x, y1_ / vec.y, x2_ / vec.x, y2_ / vec.y);
  }

 private:
  T x1_;
  T y1_;
  T x2_;
  T y2_;
};

using Rectf = Rect<float, glm::vec2>;
using Recti = Rect<i32, glm::ivec2>;
using Rectu = Rect<u32, glm::uvec2>;

#endif  // VULKAN_SRC_CORE_MATH_RECT_H_
