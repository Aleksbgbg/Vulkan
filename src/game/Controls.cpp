#include "Controls.h"

bool Controls::IsControlActive(const Control control) const {
  return activeControls_.contains(control);
}

glm::vec2 Controls::AxisValue(const Axis axis) const {
  return axisValues_.at(axis);
}

void Controls::Update(const Keyboard& keyboard, const Mouse& mouse,
                      const glm::vec2 windowSize) {
  activeControls_.clear();

  if (keyboard.IsKeyDown(SDLK_w)) {
    activeControls_.insert(Control::Accelerate);
  }

  if (keyboard.IsKeyDown(SDLK_s)) {
    activeControls_.insert(Control::Decelerate);
  }

  if (keyboard.IsKeyDown(SDLK_c)) {
    activeControls_.insert(Control::ReverseView);
  }

  if (keyboard.IsKeyDown(SDLK_f)) {
    activeControls_.insert(Control::MoveNpc);
  }

  if (keyboard.PressedKey(SDLK_g)) {
    activeControls_.insert(Control::SpawnPlayer);
  }

  const glm::vec2 halfWindowSize = windowSize / 2.0f;
  const glm::vec2 mousePosition = mouse.GetPosition();
  const glm::vec2 mousePositionCentered = mousePosition - halfWindowSize;
  const glm::vec2 mouseInputNormalized = mousePositionCentered / halfWindowSize;

  if (keyboard.IsKeyDown(SDLK_LALT)) {  // Look around
    axisValues_[Axis::Movement] = glm::vec2(0.0f);
    axisValues_[Axis::View] = mouseInputNormalized;
  } else {
    axisValues_[Axis::Movement] = mouseInputNormalized;
    axisValues_[Axis::View] = glm::vec2(0.0f);
  }
}
