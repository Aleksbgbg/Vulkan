#include "SunMovement.h"

SunMovement::SunMovement(Transform& transform)
    : transform_(transform), spin_(0.0f) {}

void SunMovement::OnSpawn() {
  transform_.Move(glm::vec3(0.0f, 0.0f, 1000.0f));
  transform_.Scale(glm::vec3(100.0f));
}

void SunMovement::UpdateModel(const UpdateContext& context) {
  constexpr float SpinVelocity = 0.1f;
  spin_ = std::fmod(spin_ - (SpinVelocity * context.deltaTime), FULL_TURN);
  transform_.Rotate(Rotation(spin_, X_AXIS + Y_AXIS));
}
