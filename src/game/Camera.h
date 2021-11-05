#ifndef VULKAN_SRC_GAME_CAMERA_H
#define VULKAN_SRC_GAME_CAMERA_H

#include "util/include/glm.h"

class Camera {
 public:
  struct View {
    glm::vec3 position;
    glm::vec3 lookAt;
  };

  View GetView() const;

  void SetView(const View& value);

 private:
  View view;
};

#endif  // VULKAN_SRC_GAME_CAMERA_H
