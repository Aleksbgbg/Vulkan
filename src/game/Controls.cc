#include "Controls.h"

#include "general/math/clamp.h"
#include "general/math/transform.h"

Controls::Controls()
    : continuousKeyControls_({
          {Control::Accelerate, SDLK_w},
          {Control::Decelerate, SDLK_s},
          {Control::ReverseView, SDLK_c},
          {Control::TiltLeft, SDLK_e},
          {Control::TiltRight, SDLK_q},
      }),
      singleKeyControls_({
          {Control::SpawnPlayer, SDLK_g},
          {Control::Shoot, SDLK_f},
          {Control::TogglePauseMenu, SDLK_ESCAPE},
      }),
      activeControls_(),
      normalizedAxisValues_() {}

bool Controls::IsControlActive(const Control control) const {
  return activeControls_.contains(control);
}

glm::vec2 Controls::NormalizedAxisValue(const Axis axis) const {
  return normalizedAxisValues_.at(axis);
}

void Controls::Update(const sys::Window& window) {
  const Keyboard& keyboard = window.GetKeyboard();
  const Mouse& mouse = window.GetMouse();

  activeControls_.clear();

  for (const ControlWithKey controlWithKey : continuousKeyControls_) {
    if (keyboard.IsKeyDown(controlWithKey.key)) {
      activeControls_.insert(controlWithKey.control);
    }
  }

  for (const ControlWithKey controlWithKey : singleKeyControls_) {
    if (keyboard.PressedKey(controlWithKey.key)) {
      activeControls_.insert(controlWithKey.control);
    }
  }

  if (window.IsFocused()) {
    const glm::vec2 halfWindowSize = window.GetSize() / 2.0f;
    const glm::vec2 mousePositionCentered =
        mouse.GetPosition() - halfWindowSize;

    glm::vec2 mouseInputNormalized = mousePositionCentered / halfWindowSize;

    const bool lookAround = keyboard.IsKeyDown(SDLK_LALT);
    if (lookAround) {
      if (glm::length(mouseInputNormalized) > 1.0f) {
        mouseInputNormalized = glm::normalize(mouseInputNormalized);
      }

      normalizedAxisValues_[Axis::Movement] = glm::vec2(0.0f);
      normalizedAxisValues_[Axis::View] = mouseInputNormalized;
    } else {
      static constexpr float Deadzone = 0.1f;
      if (mouseInputNormalized.x < 0.0f) {
        mouseInputNormalized.x += Deadzone;
        Clamp(&mouseInputNormalized.x, -1.0f, 0.0f);
      } else if (mouseInputNormalized.x > 0.0f) {
        mouseInputNormalized.x -= Deadzone;
        Clamp(&mouseInputNormalized.x, 0.0f, 1.0f);
      }
      if (mouseInputNormalized.y < 0.0f) {
        mouseInputNormalized.y += Deadzone;
        Clamp(&mouseInputNormalized.y, -1.0f, 0.0f);
      } else if (mouseInputNormalized.y > 0.0f) {
        mouseInputNormalized.y -= Deadzone;
        Clamp(&mouseInputNormalized.y, 0.0f, 1.0f);
      }

      static constexpr Range MouseInputRange = Range(-0.9f, 0.9f);
      static constexpr Range MouseOutputRange = Range(-1.0f, 1.0f);
      AffineTransform(&mouseInputNormalized.x, MouseInputRange,
                      MouseOutputRange);
      AffineTransform(&mouseInputNormalized.y, MouseInputRange,
                      MouseOutputRange);

      if (glm::length(mouseInputNormalized) > 1.0f) {
        mouseInputNormalized = glm::normalize(mouseInputNormalized);
      }

      normalizedAxisValues_[Axis::Movement] = mouseInputNormalized;
      normalizedAxisValues_[Axis::View] = glm::vec2(0.0f);
    }
  } else {
    normalizedAxisValues_[Axis::Movement] = glm::vec2(0.0f);
    normalizedAxisValues_[Axis::View] = glm::vec2(0.0f);
  }
}
