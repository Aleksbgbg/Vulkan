#include "Player.h"

Player::Player(Mesh mesh) : spaceshipModel(std::move(mesh)) {}

void Player::UpdateModel(const UpdateContext& context) {
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

  spaceshipModel.Move(movement, context.deltaTime);
}

glm::vec3* Player::Position() {
  return spaceshipModel.Position();
}
glm::vec3 Player::Size() const {
  return spaceshipModel.Size();
}

void Player::WriteTexture(TextureRegistry& textureRegistry) const {
  spaceshipModel.WriteTexture(textureRegistry);
}

void Player::Render(const MeshRenderer& renderer) const {
  spaceshipModel.Render(renderer);
}
