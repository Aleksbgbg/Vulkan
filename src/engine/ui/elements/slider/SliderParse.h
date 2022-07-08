#ifndef SRC_ENGINE_UI_ELEMENTS_SLIDER_SLIDERPARSE_H_
#define SRC_ENGINE_UI_ELEMENTS_SLIDER_SLIDERPARSE_H_

#include "engine/ui/Bound.h"

struct SliderParse {
  float defaultValue;
  Bound<float>& valueBinding;
};

#endif  // SRC_ENGINE_UI_ELEMENTS_SLIDER_SLIDERPARSE_H_
