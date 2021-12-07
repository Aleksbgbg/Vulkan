#ifndef VULKAN_SRC_GAME_BEHAVIOURS_CAMERACONTROLLER_H_
#define VULKAN_SRC_GAME_BEHAVIOURS_CAMERACONTROLLER_H_

#include "game/Camera.h"
#include "game/actor/behaviour/Behaviour.h"
#include "game/actor/property/Transform.h"

class CameraController : public Behaviour {
 public:
  CameraController(game::Camera& camera, Transform& parentTransform);

  void UpdateModel(const UpdateContext& context) override;

 private:
  game::Camera& camera_;
  Transform& parentTransform_;
};

#endif  // VULKAN_SRC_GAME_BEHAVIOURS_CAMERACONTROLLER_H_
