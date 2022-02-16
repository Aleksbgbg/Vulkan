#ifndef VULKAN_SRC_GAME_UI_MARGIN_H_
#define VULKAN_SRC_GAME_UI_MARGIN_H_

#include "DistanceUnit.h"
#include "general/geometry/Rect.h"

struct Margin {
  DistanceUnit unit;

  union {
    Rectf fractional;
    Rectu pixel;
  };
};

#endif  // VULKAN_SRC_GAME_UI_MARGIN_H_
