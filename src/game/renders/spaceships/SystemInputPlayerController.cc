#include "SystemInputPlayerController.h"

#include "general/math/math.h"

SystemInputPlayerController::SystemInputPlayerController()
    : velocity_(), rotation_() {}

glm::vec3 SystemInputPlayerController::GetVelocity() const {
  return glm::toMat4(glm::quat(rotation_)) * glm::vec4(velocity_, 0.0f);
}

glm::vec3 SystemInputPlayerController::GetRotation() const {
  return rotation_;
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
}
