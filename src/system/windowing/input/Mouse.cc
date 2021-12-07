#include "Mouse.h"

Mouse::Mouse() : position_(0.0f) {}

void Mouse::Movement(const SDL_MouseMotionEvent& event) {
  position_ = glm::vec2(event.x, event.y);
}

glm::vec2 Mouse::GetPosition() const {
  return position_;
}
