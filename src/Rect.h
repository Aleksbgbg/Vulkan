#ifndef VULKAN_SRC_RECT_H
#define VULKAN_SRC_RECT_H

#include "types.h"

template <typename T>
struct Rect;

typedef Rect<float> Rectf;
typedef Rect<i32> Recti;

template <typename T>
struct Rect {
  Rect() = default;
  Rect(T x, T y, T width, T height)
      : x(x), y(y), width(width), height(height) {}

  operator Rectf() const {
    return Rectf(static_cast<float>(x), static_cast<float>(y),
                 static_cast<float>(width), static_cast<float>(height));
  }

  operator Recti() const {
    return Recti(static_cast<i32>(x), static_cast<i32>(y),
                 static_cast<i32>(width), static_cast<i32>(height));
  }

  T x;
  T y;
  T width;
  T height;
};

#endif  // VULKAN_SRC_RECT_H
