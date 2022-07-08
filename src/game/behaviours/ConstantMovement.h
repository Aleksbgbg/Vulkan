#ifndef SRC_GAME_BEHAVIOURS_CONSTANTMOVEMENT_H_
#define SRC_GAME_BEHAVIOURS_CONSTANTMOVEMENT_H_

#include "engine/behaviour/Behaviour.h"
#include "engine/composition/parameter_utils.h"
#include "engine/property/type/Transform.h"

class ConstantMovement : public Behaviour {
 public:
  PARAMETER_PACK_BEGIN
  PARAMETER_TYPE(Transform, Transform)
  PARAMETER_VALUE(float, ForwardVelocity)
  PARAMETER_PACK_END

  ConstantMovement(const ParameterPack& parameters);

  void UpdateModel(const UpdateContext& context) override;

 private:
  Transform& transform_;
  glm::vec3 velocity_;
};

#endif  // SRC_GAME_BEHAVIOURS_CONSTANTMOVEMENT_H_
