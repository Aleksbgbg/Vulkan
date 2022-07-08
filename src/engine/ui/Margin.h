#ifndef SRC_ENGINE_UI_MARGIN_H_
#define SRC_ENGINE_UI_MARGIN_H_

#include "DistanceUnit.h"
#include "core/math/Rect.h"

struct Margin {
  DistanceUnit unit;

  union {
    Rectf fractional;
    Rectu pixel;
  };
};

#endif  // SRC_ENGINE_UI_MARGIN_H_
