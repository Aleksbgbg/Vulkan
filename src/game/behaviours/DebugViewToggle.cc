#include "DebugViewToggle.h"

#include "util/build_definition.h"

DebugViewToggle::DebugViewToggle(GraphicalInterface& interface)
    : interface_(interface) {
#if !defined(DEBUG)
  interface_.Hide();
#endif
}

void DebugViewToggle::UpdateModel(const UpdateContext& context) {
  if (context.controls.IsControlActive(Control::ToggleDebugView)) {
    interface_.ToggleVisibility();
  }
}
