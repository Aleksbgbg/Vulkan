#ifndef VULKAN_SRC_GAME_CAMERA_H_
#define VULKAN_SRC_GAME_CAMERA_H_

#include "util/include/glm.h"

namespace game {

class Camera {
 public:
  Camera();

  glm::mat4 GetViewMatrix() const;

  struct View {
    glm::mat4 positionTransform;
    glm::vec2 lookAround;
    bool reverse;
  };
  void SetView(const View& view);

 private:
  glm::mat4 cameraTransform_;
};

}  // namespace game

#endif  // VULKAN_SRC_GAME_CAMERA_H_
