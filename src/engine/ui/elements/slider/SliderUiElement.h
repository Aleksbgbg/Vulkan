#ifndef SRC_ENGINE_UI_ELEMENTS_SLIDER_SLIDERUIELEMENT_H_
#define SRC_ENGINE_UI_ELEMENTS_SLIDER_SLIDERUIELEMENT_H_

#include "Slider.h"
#include "SliderParse.h"
#include "engine/ui/FocusTracker.h"
#include "engine/ui/UiElement.h"

class SliderUiElement : public UiElement {
 public:
  SliderUiElement(SliderParse sliderParse);

  void ProcessAction(FocusTracker& focusTracker, const Action& action) override;
  void CalculateLayout(const LayoutCalculator& layoutCalculator) override;
  void Render(const UiRenderer& renderer) const override;

 private:
  Slider slider_;
};

#endif  // SRC_ENGINE_UI_ELEMENTS_SLIDER_SLIDERUIELEMENT_H_
