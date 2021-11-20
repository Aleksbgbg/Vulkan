#include "NoMovementPlayerController.h"

bool NoMovementPlayerController::IsMoving() const {
  return false;
}

glm::vec3 NoMovementPlayerController::GetRotation() const {
  return glm::vec3();
}

glm::vec3 NoMovementPlayerController::GetPosition() const {
  return glm::vec3();
}
