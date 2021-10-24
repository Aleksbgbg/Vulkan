#include "SpaceshipModel.h"

#include "util/filenames.h"

SpaceshipModel::SpaceshipModel(Mesh mesh)
    : mesh(std::move(mesh)), position(), transform(1.0f) {}

glm::vec3* SpaceshipModel::Position() {
  return &position;
}

glm::vec3 SpaceshipModel::Size() const {
  return mesh.Size();
}

void SpaceshipModel::WriteTexture(TextureRegistry& resourceStore) const {
  mesh.WriteTexture(resourceStore);
}

void SpaceshipModel::Move(const glm::vec3 movement, const float deltaTime) {
  constexpr float movementSpeed = 10.0f;

  if ((std::abs(movement.x) + std::abs(movement.y) + std::abs(movement.z)) >
      0.0f) {
    const glm::vec3 normalizedMovement =
        glm::normalize(movement) * movementSpeed * deltaTime;
    position += normalizedMovement;
    transform = glm::translate(glm::mat4(1.0f), position);

    mesh.LoadFrame(1);
  } else {
    mesh.LoadFrame(0);
  }
}

void SpaceshipModel::Render(const MeshRenderer& renderer) const {
  renderer.Render(mesh, transform);
}
