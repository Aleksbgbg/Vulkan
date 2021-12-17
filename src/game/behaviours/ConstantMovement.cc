#include "ConstantMovement.h"

ConstantMovement::ConstantMovement(const ParameterPack& parameters)
    : transform_(parameters.GetTransform()),
      velocity_(glm::toMat3(parameters.GetTransform().GetRotation()) *
                glm::vec3(0.0f, 0.0f, parameters.GetForwardVelocity())) {}

void ConstantMovement::UpdateModel(const UpdateContext& context) {
  transform_.MoveBy(velocity_ * context.deltaTime);
}
