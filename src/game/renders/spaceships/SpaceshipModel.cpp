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

void SpaceshipModel::SetRotation(const glm::vec3 rotation) {
  rotation_ = rotation;
}

void SpaceshipModel::SetIsMoving(const bool value) {
  if (value) {
    int x = 5;
  }
  exhaustParticles_.SetEnabled(value);
}

void SpaceshipModel::SetPosition(const glm::vec3 position) {
  position_ = position;
}

void SpaceshipModel::Update() {
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
}

void SpaceshipModel::Render(const MeshRenderer& renderer) const {
  renderer.RenderTransformed(mesh_, glm::translate(glm::mat4(1.0f), position_) *
                                        glm::toMat4(glm::quat(rotation_)));
}
