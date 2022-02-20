#include "PlayerMovement.h"

#include "general/math/clamp.h"

PlayerMovement::PlayerMovement(Transform& transform)
    : transform_(transform), rotation_(IDENTITY_ROTATION), velocity_(0.0f) {}

void PlayerMovement::UpdateModel(const UpdateContext& context) {
  if (context.controls.IsControlActive(Control::TiltLeft)) {
    rotation_ *= Rotation(context.deltaTime, Z_AXIS);
  }
  if (context.controls.IsControlActive(Control::TiltRight)) {
    rotation_ *= Rotation(-context.deltaTime, Z_AXIS);
  }

  const glm::vec2 movementInput =
      context.controls.NormalizedAxisValue(Axis::Movement);

  rotation_ *= Rotation(movementInput.y * context.deltaTime, X_AXIS) *
               Rotation(-movementInput.x * context.deltaTime, Y_AXIS);

  constexpr float acceleration = 10.0f;
  if (context.controls.IsControlActive(Control::Accelerate)) {
    velocity_ += acceleration * context.deltaTime;
  }
  if (context.controls.IsControlActive(Control::Decelerate)) {
    velocity_ -= acceleration * context.deltaTime;
  }

  Clamp(&velocity_, 0.0f, 50.0f);

  transform_.Rotate(rotation_);
  transform_.MoveBy(glm::toMat4(rotation_) *
                    glm::vec4(0.0f, 0.0f, velocity_ * context.deltaTime, 1.0f));
}
