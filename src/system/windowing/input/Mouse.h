#ifndef SRC_SYSTEM_WINDOWING_INPUT_MOUSE_H_
#define SRC_SYSTEM_WINDOWING_INPUT_MOUSE_H_

#include "util/include/glm.h"
#include "util/include/sdl.h"

class Mouse {
 public:
  Mouse();

  void Movement(const SDL_MouseMotionEvent& event);

  glm::vec2 GetPosition() const;

 private:
  glm::vec2 position_;
};

#endif  // SRC_SYSTEM_WINDOWING_INPUT_MOUSE_H_
