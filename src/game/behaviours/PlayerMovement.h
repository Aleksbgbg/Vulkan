#ifndef SRC_GAME_BEHAVIOURS_PLAYERMOVEMENT_H_
#define SRC_GAME_BEHAVIOURS_PLAYERMOVEMENT_H_

#include "engine/behaviour/Behaviour.h"
#include "engine/property/type/Transform.h"

class PlayerMovement : public Behaviour {
 public:
  PlayerMovement(Transform& transform);

  void UpdateModel(const UpdateContext& context) override;

 private:
  Transform& transform_;
  glm::quat rotation_;
  float velocity_;
};

#endif  // SRC_GAME_BEHAVIOURS_PLAYERMOVEMENT_H_
