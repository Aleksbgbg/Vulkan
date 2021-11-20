#include "NetworkedPlayerController.h"

NetworkedPlayerController::NetworkedPlayerController(const u32 actorId)
    : actorId_(actorId), velocity_(), rotation_() {}

//glm::vec3 NetworkedPlayerController::GetVelocity() const {
//  return glm::toMat4(glm::quat(rotation_)) * glm::vec4(velocity_, 0.0f);
//}

bool NetworkedPlayerController::IsMoving() const {
  return velocity_.z > 0.0f;
}

glm::vec3 NetworkedPlayerController::GetRotation() const {
  return rotation_;
}

glm::vec3 NetworkedPlayerController::GetPosition() const {
  return position_;
}

void NetworkedPlayerController::Update(const Network& network) {
  rotation_.x = network.ReadFloat(actorId_, Network::Attribute::RotationX);
  rotation_.y = network.ReadFloat(actorId_, Network::Attribute::RotationY);
  velocity_.z = network.ReadFloat(actorId_, Network::Attribute::VelocityZ);
  position_.x = network.ReadFloat(actorId_, Network::Attribute::PositionX);
  position_.y = network.ReadFloat(actorId_, Network::Attribute::PositionY);
  position_.z = network.ReadFloat(actorId_, Network::Attribute::PositionZ);
}
