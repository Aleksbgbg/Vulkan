#ifndef SRC_ENGINE_UI_ELEMENTS_SLIDER_SLIDER_H_
#define SRC_ENGINE_UI_ELEMENTS_SLIDER_SLIDER_H_

#include "SliderLayout.h"
#include "engine/ui/Bindable.h"

struct Slider {
  Bindable<float> value;
  SliderLayout layout;
};

#endif  // SRC_ENGINE_UI_ELEMENTS_SLIDER_SLIDER_H_
