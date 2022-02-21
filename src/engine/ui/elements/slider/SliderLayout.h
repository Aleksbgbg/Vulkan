#ifndef VULKAN_SRC_ENGINE_UI_ELEMENTS_SLIDER_SLIDERLAYOUT_H_
#define VULKAN_SRC_ENGINE_UI_ELEMENTS_SLIDER_SLIDERLAYOUT_H_

#include "core/math/Rect.h"

struct SliderLayout {
  Rectu renderArea;
  Rectu background;
  Rectu highlight;
};

#endif  // VULKAN_SRC_ENGINE_UI_ELEMENTS_SLIDER_SLIDERLAYOUT_H_
