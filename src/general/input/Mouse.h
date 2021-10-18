#ifndef VULKAN_SRC_MOUSE_H
#define VULKAN_SRC_MOUSE_H

#include <optional>

#include "util/include/glm.h"
#include "util/include/sdl.h"

class Mouse {
 public:
  Mouse();

  void Movement(const SDL_MouseMotionEvent& event);
  void Button(const SDL_MouseButtonEvent& event);

  glm::vec2 GetPositionDeltaFromRightClick() const;

 private:
  glm::vec2 position;
  std::optional<glm::vec2> lastRightClickPosition;
};

#endif  // VULKAN_SRC_MOUSE_H
