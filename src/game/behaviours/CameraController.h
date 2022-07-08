#ifndef SRC_GAME_BEHAVIOURS_CAMERACONTROLLER_H_
#define SRC_GAME_BEHAVIOURS_CAMERACONTROLLER_H_

#include "engine/behaviour/Behaviour.h"
#include "engine/property/type/Transform.h"
#include "game/Camera.h"

class CameraController : public Behaviour {
 public:
  CameraController(game::Camera& camera, Transform& parentTransform);

  void UpdateModel(const UpdateContext& context) override;

 private:
  game::Camera& camera_;
  Transform& parentTransform_;
};

#endif  // SRC_GAME_BEHAVIOURS_CAMERACONTROLLER_H_
