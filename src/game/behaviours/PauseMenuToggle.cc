#include "PauseMenuToggle.h"

PauseMenuToggle::PauseMenuToggle(GraphicalInterface& interface)
    : interface_(interface) {
  interface_.Hide();
}

void PauseMenuToggle::UpdateModel(const UpdateContext& context) {
  if (context.controls.IsControlActive(Control::TogglePauseMenu)) {
    if (interface_.HideRequested()) {
      interface_.Hide();
    } else {
      interface_.Show();
    }
  }
}
