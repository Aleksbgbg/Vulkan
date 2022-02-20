#ifndef VULKAN_SRC_GAME_UI_ELEMENTS_SLIDER_SLIDERLAYOUT_H_
#define VULKAN_SRC_GAME_UI_ELEMENTS_SLIDER_SLIDERLAYOUT_H_

#include "general/math/Rect.h"

struct SliderLayout {
  Rectu renderArea;
  Rectu background;
  Rectu highlight;
};

#endif  // VULKAN_SRC_GAME_UI_ELEMENTS_SLIDER_SLIDERLAYOUT_H_
