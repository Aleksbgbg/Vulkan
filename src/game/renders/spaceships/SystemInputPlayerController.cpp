#include "SystemInputPlayerController.h"

#include "general/math/math.h"

SystemInputPlayerController::SystemInputPlayerController()
    : moving_(), velocity_(), rotation_(), position_() {}

//glm::vec3 SystemInputPlayerController::GetVelocity() const {
//  return glm::toMat4(glm::quat(rotation_)) * glm::vec4(velocity_, 0.0f);
//}

bool SystemInputPlayerController::IsMoving() const {
  return moving_;
}

glm::vec3 SystemInputPlayerController::GetRotation() const {
  return rotation_;
}

glm::vec3 SystemInputPlayerController::GetPosition() const {
  return position_;
}

float OffsetValueByDeadzone(const float value, const float deadzone) {
  if (std::abs(value) < deadzone) {
    return 0.0f;
  }

  if (value < 0.0f) {
    return value + deadzone;
  }

  return value - deadzone;
}

void SystemInputPlayerController::UpdateModel(const UpdateContext& context) {
  glm::vec2 movement = context.controls.AxisValue(Axis::Movement);
  movement.x = -movement.x;

  constexpr float Deadzone = 0.1f;
  movement.x = OffsetValueByDeadzone(movement.x, Deadzone);
  movement.y = OffsetValueByDeadzone(movement.y, Deadzone);

  rotation_.x += movement.y * context.deltaTime;
  rotation_.y += movement.x * context.deltaTime;

  float acceleration = 0.0f;
  if (context.controls.IsControlActive(Control::Accelerate)) {
    acceleration += 1.0f;
  }
  if (context.controls.IsControlActive(Control::Decelerate)) {
    acceleration -= 1.0f;
  }
  velocity_.z += acceleration * context.deltaTime;
  velocity_.z = CoerceToRange(velocity_.z, 0.0f, 1.0f);

  constexpr float movementSpeed = 10.0f;

  const glm::vec3 shipMovement =
      glm::toMat4(glm::quat(rotation_)) * glm::vec4(velocity_, 0.0f);

  if ((std::abs(shipMovement.x) + std::abs(shipMovement.y) +
       std::abs(shipMovement.z)) > 0.0f) {
    const glm::vec3 normalizedMovement =
        glm::normalize(shipMovement) * movementSpeed * context.deltaTime;
    position_ += normalizedMovement;
    moving_ = true;
  } else {
    moving_ = false;
  }
}
