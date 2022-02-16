#ifndef VULKAN_SRC_GAME_UI_ELEMENTS_GRIDLIST_GRIDLISTUIELEMENT_H_
#define VULKAN_SRC_GAME_UI_ELEMENTS_GRIDLIST_GRIDLISTUIELEMENT_H_

#include <game/ui/FocusTracker.h>

#include "GridList.h"
#include "GridListParse.h"
#include "game/ui/UiElement.h"

class GridListUiElement : public UiElement {
 public:
  GridListUiElement(GridListParse gridListParse);

  void OnAcquiredFocus() override;
  void OnLostFocus() override;

  void ProcessAction(FocusTracker& focusTracker, const Action& action) override;
  void CalculateLayout(const LayoutCalculator& layoutCalculator) override;
  void Render(const UiRenderer& renderer) const override;

 private:
  GridList gridList_;
};

#endif  // VULKAN_SRC_GAME_UI_ELEMENTS_GRIDLIST_GRIDLISTUIELEMENT_H_
