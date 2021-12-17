#ifndef VULKAN_SRC_GAME_BEHAVIOURS_CONSTANTMOVEMENT_H_
#define VULKAN_SRC_GAME_BEHAVIOURS_CONSTANTMOVEMENT_H_

#include "game/actor/behaviour/Behaviour.h"
#include "game/actor/property/Transform.h"
#include "game/composition/parameter_utils.h"

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

#endif  // VULKAN_SRC_GAME_BEHAVIOURS_CONSTANTMOVEMENT_H_
