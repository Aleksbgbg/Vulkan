#ifndef VULKAN_SRC_ENGINE_COMPOSITION_COMPOSITIONBUILDER_H_
#define VULKAN_SRC_ENGINE_COMPOSITION_COMPOSITIONBUILDER_H_

#include <memory>
#include <optional>
#include <vector>

#include "LightSource.h"
#include "LightSourceComposition.h"
#include "MeshHandle.h"
#include "ParticleBehaviour.h"
#include "SpawnDependencies.h"
#include "engine/actor/Actor.h"
#include "engine/behaviour/Behaviour.h"
#include "engine/ui/DrawList.h"
#include "engine/ui/UiDrawList.h"
#include "engine/ui/ViewModel.h"

class CompositionBuilder {
 private:
  struct Composition_T;

  using Composition = std::shared_ptr<Composition_T>;
  using BehaviourFactory = std::function<std::unique_ptr<Behaviour>(
      game::Actor* const, game::Actor&)>;

  struct Composition_T {
    std::optional<MeshHandle> meshHandle;
    std::optional<LightSource> lightSource;
    std::vector<BehaviourFactory> behaviourFactories;
    std::vector<Composition> children;

    enum class Type {
      Actor,
      ParticleSystem,
      Ui,
    };
    Type type;

    struct ActorInfo {};
    ActorInfo actor;

    struct ParticleSystemInfo {
      ParticleBehaviour behaviour;
      glm::vec3 spawnRegionLow;
      glm::vec3 spawnRegionHigh;
    };
    ParticleSystemInfo particleSystem;

    struct UiInfo {
      std::optional<UiTree> uiTree;
      std::unique_ptr<ViewModel> viewModel;
    };
    std::shared_ptr<UiInfo> ui;
  };

 public:
  static CompositionBuilder Actor(SpawnDependencies spawnDependencies);
  static CompositionBuilder ParticleSystem(SpawnDependencies spawnDependencies,
                                           ParticleBehaviour particleBehaviour);
  static CompositionBuilder Ui(SpawnDependencies spawnDependencies,
                               std::string_view view,
                               std::unique_ptr<ViewModel> viewModel);

  CompositionBuilder Copy() const;

  CompositionBuilder& Attach(BehaviourFactory scriptFactory);
  CompositionBuilder& LightSource(
      const LightSourceComposition& lightSourceComposition);
  CompositionBuilder& Mesh(const MeshHandle meshHandle);
  CompositionBuilder& SpawnRegion(const glm::vec3 spawnRegionLow,
                                  const glm::vec3 spawnRegionHigh);
  CompositionBuilder& Child(const CompositionBuilder child);

  void Spawn() const;

 private:
  CompositionBuilder(SpawnDependencies spawnDependencies,
                     Composition composition);

  static void SpawnComposition(const SpawnDependencies& dependencies,
                               const Composition& composition,
                               game::Actor* parent, const ActorKey parentKey);

 private:
  ActorKeyGenerator actorKeyGenerator_;
  SpawnDependencies spawnDependencies_;
  Composition composition_;
};

#endif  // VULKAN_SRC_ENGINE_COMPOSITION_COMPOSITIONBUILDER_H_
