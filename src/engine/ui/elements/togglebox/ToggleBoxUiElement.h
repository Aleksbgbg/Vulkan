#ifndef VULKAN_SRC_ENGINE_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOXUIELEMENT_H_
#define VULKAN_SRC_ENGINE_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOXUIELEMENT_H_

#include "ToggleBox.h"
#include "ToggleBoxParse.h"
#include "engine/ui/UiElement.h"

class ToggleBoxUiElement : public UiElement {
 public:
  ToggleBoxUiElement(ToggleBoxParse toggleBoxParse);

  void ProcessAction(FocusTracker& focusTracker, const Action& action) override;
  void CalculateLayout(const LayoutCalculator& layoutCalculator) override;
  void Render(const UiRenderer& renderer) const override;

 private:
  ToggleBox toggleBox_;
};

#endif  // VULKAN_SRC_ENGINE_UI_ELEMENTS_TOGGLEBOX_TOGGLEBOXUIELEMENT_H_
