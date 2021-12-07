#ifndef VULKAN_SRC_GAME_SCENE_H_
#define VULKAN_SRC_GAME_SCENE_H_

#include <memory>
#include <vector>

#include "Camera.h"
#include "Renderer.h"
#include "UpdateContext.h"
#include "game/actor/Actor.h"
#include "game/composition/ActorConsumer.h"

class Scene : public ActorConsumer {
 public:
  Scene(Renderer& renderer, game::Camera& camera);

  void Consume(std::unique_ptr<game::Actor> value) override;

  void UpdateModel(const UpdateContext& context);

 private:
  std::vector<std::unique_ptr<game::Actor>> actors_;
};

#endif  // VULKAN_SRC_GAME_SCENE_H_
