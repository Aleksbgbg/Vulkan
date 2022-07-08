#ifndef SRC_ENGINE_UI_UIELEMENT_H_
#define SRC_ENGINE_UI_UIELEMENT_H_

#include "Action.h"
#include "FocusTracker.h"
#include "Focusable.h"
#include "LayoutCalculator.h"
#include "UiRenderer.h"

class UiElement : public Focusable {
 public:
  virtual ~UiElement() = default;

  virtual void OnAcquiredFocus() override {}
  virtual void OnChildAcquiredFocus() override {}
  virtual void OnLostFocus() override {}

  virtual void ProcessAction(FocusTracker& focusTracker,
                             const Action& action) = 0;
  virtual void CalculateLayout(const LayoutCalculator& layoutCalculator) = 0;
  virtual void Render(const UiRenderer& renderer) const = 0;
};

#endif  // SRC_ENGINE_UI_UIELEMENT_H_
