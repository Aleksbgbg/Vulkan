#include "ConstantMovement.h"

ConstantMovement::ConstantMovement(Transform& transform,
                                   const glm::vec3 velocity)
    : transform_(transform), velocity_(velocity) {}

void ConstantMovement::UpdateModel(const UpdateContext& context) {
  transform_.MoveBy(velocity_ * context.deltaTime);
}
