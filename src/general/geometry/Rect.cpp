#include "Rect.h"

template <typename T, typename TVec>
Rect<T, TVec> Rect<T, TVec>::FromPoints(T x1, T y1, T x2, T y2) {
  if (x1 > x2) {
    std::swap(x1, x2);
  }

  if (y1 > y2) {
    std::swap(y1, y2);
  }

  return Rect<T, TVec>(x1, y1, x2, y2);
}

template <typename T, typename TVec>
Rect<T, TVec> Rect<T, TVec>::FromPoints(const TVec point1, const TVec point2) {
  return FromPoints(point1.x, point2.y, point2.x, point2.y);
}

template <typename T, typename TVec>
Rect<T, TVec> Rect<T, TVec>::FromRegion(const T x, const T y, const T width,
                                        const T height) {
  return FromPoints(x, y, x + width, y + height);
}

template <typename T, typename TVec>
Rect<T, TVec> Rect<T, TVec>::FromRegion(const TVec point, const TVec size) {
  return FromPoints(point.x, point.y, point.x + size.x, point.y + size.y);
}

template <typename T, typename TVec>
T Rect<T, TVec>::X() const {
  return x1;
}

template <typename T, typename TVec>
T Rect<T, TVec>::Y() const {
  return y1;
}

template <typename T, typename TVec>
T Rect<T, TVec>::Width() const {
  return x2 - x1;
}

template <typename T, typename TVec>
T Rect<T, TVec>::Height() const {
  return y2 - y1;
}

template <typename T, typename TVec>
T Rect<T, TVec>::X1() const {
  return x1;
}

template <typename T, typename TVec>
T Rect<T, TVec>::Y1() const {
  return y1;
}

template <typename T, typename TVec>
T Rect<T, TVec>::X2() const {
  return x2;
}

template <typename T, typename TVec>
T Rect<T, TVec>::Y2() const {
  return y2;
}

template <typename T, typename TVec>
TVec Rect<T, TVec>::Point() const {
  return TVec(x1, y1);
}

template <typename T, typename TVec>
TVec Rect<T, TVec>::Size() const {
  return TVec(x2 - x1, y2 - y1);
}

template <typename T, typename TVec>
TVec Rect<T, TVec>::Point1() const {
  return TVec(x1, y1);
}

template <typename T, typename TVec>
TVec Rect<T, TVec>::Point2() const {
  return TVec(x2, y2);
}

template <typename T, typename TVec>
Rect<T, TVec>::operator Rectf() const {
  return Rectf::FromPoints(static_cast<float>(x1), static_cast<float>(y1),
                           static_cast<float>(x2), static_cast<float>(y2));
}

template <typename T, typename TVec>
Rect<T, TVec>::operator Recti() const {
  return Recti::FromPoints(static_cast<i32>(x1), static_cast<i32>(y1),
                           static_cast<i32>(x2), static_cast<i32>(y2));
}

template <typename T, typename TVec>
Rect<T, TVec>::Rect(const T x1, const T y1, const T x2, const T y2)
    : x1(x1), y1(y1), x2(x2), y2(y2) {}

template class Rect<float, glm::vec2>;
template class Rect<i32, glm::ivec2>;
