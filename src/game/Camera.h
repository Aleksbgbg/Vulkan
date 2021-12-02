#ifndef VULKAN_SRC_GAME_CAMERA_H_
#define VULKAN_SRC_GAME_CAMERA_H_

#include "util/include/glm.h"

class Camera {
 public:
  struct View {
    glm::vec3 position;
    glm::vec3 lookAt;
    glm::vec3 rotation;
    bool reverse;
  };

  glm::mat4 GetViewMatrix() const;
  glm::vec3 GetPosition() const;

  void SetView(const View& value);

 private:
  glm::vec3 Up() const;

 private:
  View view;
};

#endif  // VULKAN_SRC_GAME_CAMERA_H_
