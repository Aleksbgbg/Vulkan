#include "SliderUiElement.h"

#include "core/math/clamp.h"

Slider ToSlider(const SliderParse& parse) {
  return {.value = Bindable(parse.valueBinding)};
}

SliderUiElement::SliderUiElement(const SliderParse sliderParse)
    : slider_(ToSlider(sliderParse)) {}

void SliderUiElement::ProcessAction(FocusTracker& focusTracker,
                                    const Action& action) {
  static constexpr float Increment = 0.1f;

  if (action.type == Action::Type::Move) {
    float sliderValue = slider_.value;

    if (action.move.direction == Action::Move::Direction::Left) {
      sliderValue -= Increment;
    } else if (action.move.direction == Action::Move::Direction::Right) {
      sliderValue += Increment;
    }

    slider_.value = Clamp(sliderValue, 0.0f, 1.0f);
  }
}

void SliderUiElement::CalculateLayout(
    const LayoutCalculator& layoutCalculator) {
  slider_.layout = layoutCalculator.CalculateLayout(slider_);
}

void SliderUiElement::Render(const UiRenderer& renderer) const {
  renderer.Render(slider_);
}
