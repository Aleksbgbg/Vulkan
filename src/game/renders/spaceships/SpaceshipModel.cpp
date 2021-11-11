#include "SpaceshipModel.h"

#include <glm/gtx/quaternion.hpp>

SpaceshipModel::SpaceshipModel(SpaceshipMesh mesh)
    : mesh(std::move(mesh)), position(0.0f), rotation(0.0f) {}

glm::vec3* SpaceshipModel::Position() {
  return &position;
}

glm::vec3 SpaceshipModel::Size() const {
  return mesh.Size();
}

const Mesh& SpaceshipModel::GetMesh() const {
  return mesh;
}

void SpaceshipModel::Rotate(const glm::vec3 rotation) {
  this->rotation = rotation;
}

void SpaceshipModel::Move(const glm::vec3 movement, const float deltaTime) {
  constexpr float movementSpeed = 10.0f;

  if ((std::abs(movement.x) + std::abs(movement.y) + std::abs(movement.z)) >
      0.0f) {
    const glm::vec3 normalizedMovement =
        glm::normalize(movement) * movementSpeed * deltaTime;
    position += normalizedMovement;

    mesh.LoadFrame(1);
  } else {
    mesh.LoadFrame(0);
  }
}

void SpaceshipModel::Render(const MeshRenderer& renderer) const {
  glm::mat4 transform(1.0f);

  transform = glm::translate(transform, position);

  transform = glm::rotate(transform, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
  transform = glm::rotate(transform, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
  transform = glm::rotate(transform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

  renderer.RenderTransformed(mesh, transform);
}
