#ifndef VULKAN_SRC_RECT_H
#define VULKAN_SRC_RECT_H

#include "util/include/glm.h"
#include "util/types.h"

template <typename T, typename TVec>
class Rect;

typedef Rect<float, glm::vec2> Rectf;
typedef Rect<i32, glm::ivec2> Recti;

template <typename T, typename TVec>
class Rect {
 public:
  Rect() = default;

  static Rect<T, TVec> FromPoints(T x1, T y1, T x2, T y2);
  static Rect<T, TVec> FromPoints(const TVec point1, const TVec point2);

  static Rect<T, TVec> FromRegion(const T x, const T y, const T width,
                                  const T height);
  static Rect<T, TVec> FromRegion(const TVec point, const TVec size);

  T X() const;
  T Y() const;
  T Width() const;
  T Height() const;

  T X1() const;
  T Y1() const;
  T X2() const;
  T Y2() const;

  TVec Point() const;
  TVec Size() const;

  TVec Point1() const;
  TVec Point2() const;

  operator Rectf() const;
  operator Recti() const;

 private:
  Rect(const T x1, const T y1, const T x2, const T y2);

 private:
  T x1;
  T y1;
  T x2;
  T y2;
};

#endif  // VULKAN_SRC_RECT_H
