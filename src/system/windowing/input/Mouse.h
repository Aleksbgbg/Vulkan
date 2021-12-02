#ifndef VULKAN_SRC_SYSTEM_WINDOWING_INPUT_MOUSE_H_
#define VULKAN_SRC_SYSTEM_WINDOWING_INPUT_MOUSE_H_

#include <optional>

#include "util/include/glm.h"
#include "util/include/sdl.h"

class Mouse {
 public:
  Mouse();

  void Movement(const SDL_MouseMotionEvent& event);
  void Button(const SDL_MouseButtonEvent& event);

  glm::vec2 GetPositionDeltaFromRightClick() const;

  glm::vec2 GetPosition() const;

 private:
  glm::vec2 position;
  std::optional<glm::vec2> lastRightClickPosition;
};

#endif  // VULKAN_SRC_SYSTEM_WINDOWING_INPUT_MOUSE_H_
