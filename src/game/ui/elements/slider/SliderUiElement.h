#ifndef VULKAN_SRC_GAME_UI_ELEMENTS_SLIDER_SLIDERUIELEMENT_H_
#define VULKAN_SRC_GAME_UI_ELEMENTS_SLIDER_SLIDERUIELEMENT_H_

#include <game/ui/FocusTracker.h>

#include "Slider.h"
#include "SliderParse.h"
#include "game/ui/UiElement.h"

class SliderUiElement : public UiElement {
 public:
  SliderUiElement(SliderParse sliderParse);

  void ProcessAction(FocusTracker& focusTracker, const Action& action) override;
  void CalculateLayout(const LayoutCalculator& layoutCalculator) override;
  void Render(const UiRenderer& renderer) const override;

 private:
  Slider slider_;
};

#endif  // VULKAN_SRC_GAME_UI_ELEMENTS_SLIDER_SLIDERUIELEMENT_H_
