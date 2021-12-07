#include "ForwardMovement.h"

ForwardMovement::ForwardMovement(Transform& transform)
    : transform_(transform) {}

void ForwardMovement::UpdateModel(const UpdateContext& context) {
  transform_.MoveBy(glm::vec3(0.0f, 0.0f, 1.0f * context.deltaTime));
}
