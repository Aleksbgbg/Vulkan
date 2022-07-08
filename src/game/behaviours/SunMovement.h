#ifndef SRC_GAME_BEHAVIOURS_SUNMOVEMENT_H_
#define SRC_GAME_BEHAVIOURS_SUNMOVEMENT_H_

#include "engine/behaviour/Behaviour.h"
#include "engine/property/type/Transform.h"

class SunMovement : public Behaviour {
 public:
  SunMovement(Transform& transform);

  void OnSpawn() override;
  void UpdateModel(const UpdateContext& context) override;

 private:
  Transform& transform_;
  float spin_;
};

#endif  // SRC_GAME_BEHAVIOURS_SUNMOVEMENT_H_
