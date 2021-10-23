#include "SpaceshipModel.h"

#include "util/filenames.h"

SpaceshipModel::SpaceshipModel(const MeshLoader& meshLoader)
    : mesh(meshLoader.LoadMesh(MeshLoadParams{
          .frames = {MeshFrameLoadParams{
                         .model = SPACESHIP_STATIONARY_MODEL_FILENAME},
                     MeshFrameLoadParams{.model =
                                             SPACESHIP_MOVING_MODEL_FILENAME}},
          .texture = SPACESHIP_TEXTURE_FILENAME})),
      position(),
      transform(1.0f) {}

glm::vec3* SpaceshipModel::Position() {
  return &position;
}

glm::vec3 SpaceshipModel::Size() const {
  return mesh.Size();
}

void SpaceshipModel::WriteTexture(TextureRegistry& resourceStore) const {
  mesh.WriteTexture(resourceStore);
}

void SpaceshipModel::UpdateModel(const UpdateContext& context) {
  constexpr float movementSpeed = 10.0f;

  glm::vec3 movement(0.0f);
  if (context.keyboard.IsKeyDown(SDLK_a)) {
    movement.x += 1.0f;
  }
  if (context.keyboard.IsKeyDown(SDLK_d)) {
    movement.x -= 1.0f;
  }
  if (context.keyboard.IsKeyDown(SDLK_w)) {
    movement.y += 1.0f;
  }
  if (context.keyboard.IsKeyDown(SDLK_s)) {
    movement.y -= 1.0f;
  }
  if (context.keyboard.IsKeyDown(SDLK_q)) {
    movement.z -= 1.0f;
  }
  if (context.keyboard.IsKeyDown(SDLK_e)) {
    movement.z += 1.0f;
  }

  if ((std::abs(movement.x) + std::abs(movement.y) + std::abs(movement.z)) >
      0.0f) {
    const glm::vec3 normalizedMovement =
        glm::normalize(movement) * movementSpeed * context.deltaTime;
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
