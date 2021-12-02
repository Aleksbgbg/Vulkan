#include "Mouse.h"

Mouse::Mouse() : position(0.0f), lastRightClickPosition(std::nullopt) {}

void Mouse::Movement(const SDL_MouseMotionEvent& event) {
  position = glm::vec2(event.x, event.y);
}

void Mouse::Button(const SDL_MouseButtonEvent& event) {
  if (event.button == SDL_BUTTON_RIGHT) {
    if (event.state == SDL_PRESSED) {
      lastRightClickPosition = glm::vec2(event.x, event.y);
      SDL_SetRelativeMouseMode(SDL_TRUE);
    } else {
      lastRightClickPosition = std::nullopt;
      SDL_SetRelativeMouseMode(SDL_FALSE);
    }
  }
}

glm::vec2 Mouse::GetPositionDeltaFromRightClick() const {
  if (!lastRightClickPosition.has_value()) {
    return glm::vec2(0.0f);
  }

  return position - lastRightClickPosition.value();
}

glm::vec2 Mouse::GetPosition() const {
  return position;
}
