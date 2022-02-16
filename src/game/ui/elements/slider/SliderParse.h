#ifndef VULKAN_SRC_GAME_UI_ELEMENTS_SLIDER_SLIDERPARSE_H_
#define VULKAN_SRC_GAME_UI_ELEMENTS_SLIDER_SLIDERPARSE_H_

#include "game/ui/Bound.h"

struct SliderParse {
  float defaultValue;
  Bound<float>& valueBinding;
};

#endif  // VULKAN_SRC_GAME_UI_ELEMENTS_SLIDER_SLIDERPARSE_H_
