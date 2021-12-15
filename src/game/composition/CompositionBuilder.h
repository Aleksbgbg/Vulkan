#ifndef VULKAN_SRC_GAME_COMPOSITION_COMPOSITIONBUILDER_H_
#define VULKAN_SRC_GAME_COMPOSITION_COMPOSITIONBUILDER_H_

#include <memory>
#include <optional>
#include <vector>

#include "ActorConsumer.h"
#include "MeshHandle.h"
#include "ParticleBehaviour.h"
#include "game/Renderer.h"
#include "game/actor/Actor.h"
#include "game/actor/behaviour/Behaviour.h"
#include "system/sound/Sound.h"

class BehaviourFactory {
 public:
  virtual ~BehaviourFactory() = default;

  virtual std::unique_ptr<Behaviour> operator()(const game::Actor* parent,
                                                game::Actor& actor) = 0;
};

#define BEHAVIOUR(type, ...)                                               \
  []() {                                                                   \
    class Factory : public BehaviourFactory {                              \
     public:                                                               \
      std::unique_ptr<Behaviour> operator()(const game::Actor* parent,     \
                                            game::Actor& actor) override { \
        return std::make_unique<type>(__VA_ARGS__);                        \
      }                                                                    \
    };                                                                     \
    return std::make_unique<Factory>();                                    \
  }

class CompositionBuilder {
 private:
  struct Composition_T;
  typedef std::shared_ptr<Composition_T> Composition;

  struct Composition_T {
    std::optional<MeshHandle> meshHandle;
    std::vector<Composition> children;
    std::vector<std::unique_ptr<BehaviourFactory>> scripts;

    enum class Type {
      Actor,
      ParticleSystem,
    };
    Type type;

    union TypedInfo {
      struct ActorInfo {};
      ActorInfo actor;

      struct ParticleSystemInfo {
        ParticleBehaviour behaviour;
        glm::vec3 spawnRegionLow;
        glm::vec3 spawnRegionHigh;
      };
      ParticleSystemInfo particleSystem;
    };
    TypedInfo infoForType;
  };

 public:
  static CompositionBuilder ForActor(ActorConsumer& actorConsumer,
                                     Renderer& renderer, sys::Sound& sound);
  static CompositionBuilder ForParticleSystem(
      ActorConsumer& actorConsumer, Renderer& renderer, sys::Sound& sound,
      const ParticleBehaviour particleBehaviour);

  CompositionBuilder& Attach(
      const std::function<std::unique_ptr<BehaviourFactory>()> scriptFactory);

  CompositionBuilder& Mesh(const MeshHandle meshHandle);

  CompositionBuilder& SpawnRegion(const glm::vec3 spawnRegionLow,
                                  const glm::vec3 spawnRegionHigh);

  CompositionBuilder& Child(const CompositionBuilder child);

  void Spawn() const;

 private:
  CompositionBuilder(ActorConsumer& actorConsumer, Renderer& renderer,
                     sys::Sound& sound, Composition composition);

  void SpawnComposition(const Composition& composition,
                        const game::Actor* parent) const;

 private:
  ActorConsumer* actorConsumer_;
  Renderer* renderer_;
  sys::Sound* sound_;

  Composition composition_;
};

#endif  // VULKAN_SRC_GAME_COMPOSITION_COMPOSITIONBUILDER_H_
