#ifndef VULKAN_SRC_GAME_RENDERS_SPACESHIPS_NETWORKEDPLAYERCONTROLLER_H
#define VULKAN_SRC_GAME_RENDERS_SPACESHIPS_NETWORKEDPLAYERCONTROLLER_H

#include "game/Network.h"
#include "game/renders/spaceships/PlayerController.h"

class NetworkedPlayerController : public PlayerController {
 public:
  NetworkedPlayerController(const u32 actorId);

  bool IsMoving() const override;
  glm::vec3 GetRotation() const override;
  glm::vec3 GetPosition() const override;

  void Update(const Network& network);

 private:
  u32 actorId_;

  glm::vec3 velocity_;
  glm::vec3 rotation_;
  glm::vec3 position_;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SPACESHIPS_NETWORKEDPLAYERCONTROLLER_H
