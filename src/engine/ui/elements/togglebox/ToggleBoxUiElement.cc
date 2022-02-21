#include "ToggleBoxUiElement.h"

ToggleBox ToToggleBox(ToggleBoxParse& parse) {
  return {.elements = ElementContainer<ToggleBoxElement>(
              parse.elements, Bindable(parse.selectedIndexBinder))};
}

ToggleBoxUiElement::ToggleBoxUiElement(ToggleBoxParse toggleBoxParse)
    : toggleBox_(ToToggleBox(toggleBoxParse)) {}

void ToggleBoxUiElement::ProcessAction(FocusTracker& focusTracker,
                                       const Action& action) {
  if (action.type == Action::Type::Move) {
    if (action.move.direction == Action::Move::Direction::Left) {
      toggleBox_.elements.SelectPrevious();
    } else if (action.move.direction == Action::Move::Direction::Right) {
      toggleBox_.elements.SelectNext();
    }
  }
}

void ToggleBoxUiElement::CalculateLayout(
    const LayoutCalculator& layoutCalculator) {
  toggleBox_.layout = layoutCalculator.CalculateLayout(toggleBox_);
}

void ToggleBoxUiElement::Render(const UiRenderer& renderer) const {
  renderer.Render(toggleBox_);
}
