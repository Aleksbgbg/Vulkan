#ifndef SRC_GAME_SCENE_H_
#define SRC_GAME_SCENE_H_

#include <memory>
#include <unordered_map>

#include "Camera.h"
#include "GraphicsSettingsConfigurator.h"
#include "Settings.h"
#include "core/text/FontAtlas.h"
#include "engine/RenderPerformanceTracker.h"
#include "engine/actor/Actor.h"
#include "engine/actor/ActorOwner.h"
#include "engine/behaviour/UpdateContext.h"
#include "engine/composition/ActorConsumer.h"
#include "engine/composition/Renderer.h"
#include "engine/composition/SceneComposer.h"
#include "engine/composition/SceneGraph.h"
#include "system/sound/Sound.h"

class Scene : public ActorConsumer, public ActorOwner {
 public:
  Scene(Renderer& renderer, sys::Window& window, sys::Sound& sound,
        game::Camera& camera, const FontAtlas& fontAtlas, Settings& settings,
        GraphicsSettingsConfigurator& graphicsSettingsConfigurator,
        const RenderPerformanceTracker& renderPerformanceTracker);

  void Consume(ActorToSpawn actor) override;
  void DespawnActor(const ActorKey key) override;

  void Update(const UpdateContext& context);

 private:
  SceneComposer scene_;
  SceneGraph sceneGraph_;
  std::list<ActorToSpawn> actorsToSpawn_;
  std::list<ActorKey> actorsToDespawn_;
};

#endif  // SRC_GAME_SCENE_H_
