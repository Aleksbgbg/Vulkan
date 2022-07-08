#ifndef SRC_ENGINE_UI_ELEMENTS_LISTBOX_LISTBOXUIELEMENT_H_
#define SRC_ENGINE_UI_ELEMENTS_LISTBOX_LISTBOXUIELEMENT_H_

#include "ListBox.h"
#include "ListBoxParse.h"
#include "engine/ui/UiElement.h"

class ListBoxUiElement : public UiElement {
 public:
  ListBoxUiElement(ListBoxParse listBoxParse);

  void OnAcquiredFocus() override;
  void OnChildAcquiredFocus() override;
  void OnLostFocus() override;

  void ProcessAction(FocusTracker& focusTracker, const Action& action) override;
  void CalculateLayout(const LayoutCalculator& layoutCalculator) override;
  void Render(const UiRenderer& renderer) const override;

 private:
  ListBox listBox_;
};

#endif  // SRC_ENGINE_UI_ELEMENTS_LISTBOX_LISTBOXUIELEMENT_H_
