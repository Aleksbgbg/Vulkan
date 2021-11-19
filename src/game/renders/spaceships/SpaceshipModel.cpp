#include "SpaceshipModel.h"

SpaceshipModel::SpaceshipModel(SpaceshipMesh& mesh,
                               ParticleStream& exhaustParticles)
    : mesh_(mesh),
      exhaustParticles_(exhaustParticles),
      position_(0.0f),
      rotation_(0.0f) {
  mesh_.LoadFrame(0);
}

glm::vec3* SpaceshipModel::Position() {
  return &position_;
}

glm::vec3 SpaceshipModel::Size() const {
  return mesh_.Size();
}

const Mesh& SpaceshipModel::GetMesh() const {
  return mesh_;
}

void SpaceshipModel::Rotate(const glm::vec3 rotation) {
  rotation_ = rotation;
}

void SpaceshipModel::Move(const glm::vec3 movement, const float deltaTime) {
  constexpr float movementSpeed = 10.0f;

  if ((std::abs(movement.x) + std::abs(movement.y) + std::abs(movement.z)) >
      0.0f) {
    constexpr glm::vec3 ExhaustPositionTopLeft = glm::vec3(13.0f, 6.0f, 46.0f);
    constexpr glm::vec3 ExhaustPositionBottomRight =
        glm::vec3(21.0f, 7.0f, 46.0f);
    constexpr glm::vec3 ShipDimensions = glm::vec3(34.0f, 16.0f, 56.0f);
    constexpr glm::vec3 ExhaustPositionTopLeftNormalized =
        ExhaustPositionTopLeft / ShipDimensions;
    constexpr glm::vec3 ExhaustPositionBottomRightNormalized =
        ExhaustPositionBottomRight / ShipDimensions;

    const glm::vec3 halfSpaceshipSize = Size() / 2.0f;
    const glm::vec3 exhaustPositionTopLeft =
        (ExhaustPositionTopLeftNormalized * Size()) - halfSpaceshipSize;
    const glm::vec3 exhaustPositionBottomRight =
        (ExhaustPositionBottomRightNormalized * Size()) - halfSpaceshipSize;

    exhaustParticles_.SetSpawnArea(
        Rectf::FromPoints(exhaustPositionTopLeft, exhaustPositionBottomRight));
    exhaustParticles_.SetTransform(
        glm::translate(glm::mat4(1.0f), position_) *
        glm::toMat4(glm::quat(rotation_)) *
        glm::translate(glm::mat4(1.0f),
                       glm::vec3(0.0f, 0.0f, -exhaustPositionTopLeft.z)));

    exhaustParticles_.SetEnabled(true);

    const glm::vec3 normalizedMovement =
        glm::normalize(movement) * movementSpeed * deltaTime;
    position_ += normalizedMovement;
  } else {
    exhaustParticles_.SetEnabled(false);
  }
}

void SpaceshipModel::Render(const MeshRenderer& renderer) const {
  glm::mat4 transform(1.0f);

  transform = glm::translate(transform, position_);
  transform = transform * glm::toMat4(glm::quat(rotation_));

//  transform = glm::rotate(transform, rotation_.x, glm::vec3(0.0f, 1.0f, 0.0f));
//  transform = glm::rotate(transform, rotation_.y, glm::vec3(1.0f, 0.0f, 0.0f));
//  transform = glm::rotate(transform, rotation_.z, glm::vec3(0.0f, 0.0f, 1.0f));

  renderer.RenderTransformed(mesh_, transform);
}
