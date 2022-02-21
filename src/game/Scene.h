#ifndef VULKAN_SRC_GAME_SCENE_H_
#define VULKAN_SRC_GAME_SCENE_H_

#include <memory>
#include <unordered_map>

#include "Camera.h"
#include "GraphicsSettingsConfigurator.h"
#include "Renderer.h"
#include "Settings.h"
#include "UpdateContext.h"
#include "core/text/FontAtlas.h"
#include "game/actor/Actor.h"
#include "game/actor/ActorOwner.h"
#include "game/composition/ActorConsumer.h"
#include "game/composition/SceneComposer.h"
#include "game/composition/SceneGraph.h"
#include "system/sound/Sound.h"

class Scene : public ActorConsumer, public ActorOwner {
 public:
  Scene(Renderer& renderer, sys::Window& window, sys::Sound& sound,
        game::Camera& camera, const FontAtlas& fontAtlas, Settings& settings,
        GraphicsSettingsConfigurator& graphicsSettingsConfigurator);

  void Consume(ActorToSpawn value) override;
  void DespawnActor(const ActorKey key) override;

  void Update(const UpdateContext& context);

 private:
  SceneComposer scene_;
  SceneGraph sceneGraph_;
  std::list<ActorToSpawn> actorsToSpawn_;
  std::list<ActorKey> actorsToDespawn_;
};

#endif  // VULKAN_SRC_GAME_SCENE_H_
