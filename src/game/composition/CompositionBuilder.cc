#include "CompositionBuilder.h"

#include "game/actor/actor_key_generator.h"
#include "game/actor/property/ParticleController.h"
#include "game/actor/property/SoundEmitter.h"
#include "game/actor/property/Transform.h"

CompositionBuilder CompositionBuilder::Actor(
    const SpawnDependencies spawnDependencies) {
  Composition composition = std::make_unique<Composition_T>();
  composition->type = Composition_T::Type::Actor;

  return CompositionBuilder(spawnDependencies, std::move(composition));
}

CompositionBuilder CompositionBuilder::ParticleSystem(
    const SpawnDependencies spawnDependencies,
    const ParticleBehaviour particleBehaviour) {
  Composition composition = std::make_unique<Composition_T>();
  composition->type = Composition_T::Type::ParticleSystem;
  composition->infoForType.particleSystem.behaviour = particleBehaviour;

  return CompositionBuilder(spawnDependencies, std::move(composition));
}

CompositionBuilder& CompositionBuilder::Attach(BehaviourFactory scriptFactory) {
  composition_->behaviourFactories.push_back(std::move(scriptFactory));
  return *this;
}

CompositionBuilder& CompositionBuilder::Mesh(const MeshHandle meshHandle) {
  composition_->meshHandle = meshHandle;
  return *this;
}

CompositionBuilder& CompositionBuilder::SpawnRegion(
    const glm::vec3 spawnRegionLow, const glm::vec3 spawnRegionHigh) {
  composition_->infoForType.particleSystem.spawnRegionLow = spawnRegionLow;
  composition_->infoForType.particleSystem.spawnRegionHigh = spawnRegionHigh;
  return *this;
}

CompositionBuilder& CompositionBuilder::Child(const CompositionBuilder child) {
  composition_->children.push_back(child.composition_);
  return *this;
}

void CompositionBuilder::Spawn() const {
  SpawnComposition(composition_, nullptr, 0);
}

CompositionBuilder::CompositionBuilder(SpawnDependencies spawnDependencies,
                                       Composition composition)
    : spawnDependencies_(spawnDependencies),
      composition_(std::move(composition)) {}

void CompositionBuilder::SpawnComposition(const Composition& composition,
                                          const game::Actor* parent,
                                          const ActorKey parentKey) const {
  const Transformable* parentTransform;

  if (parent == nullptr) {
    parentTransform = nullptr;
  } else {
    parentTransform = &parent->RetrieveProperty<Transform>();
  }

  std::list<std::unique_ptr<Resource>> resources;

  std::unordered_map<PropertyKey, std::unique_ptr<Property>> properties;
  properties.insert(
      {Transform::Key(), std::make_unique<Transform>(parentTransform)});
  properties.insert({SoundEmitter::Key(), std::make_unique<SoundEmitter>(
                                              *spawnDependencies_.sound)});

  switch (composition->type) {
    case Composition_T::Type::Actor: {
      const bool isRenderable = composition->meshHandle.has_value();
      if (isRenderable) {
        resources.push_back(spawnDependencies_.renderer->SpawnRenderable({
            .meshHandle = composition->meshHandle.value(),
            .transformable = reinterpret_cast<Transform*>(
                properties.at(Transform::Key()).get()),
        }));
      }
    } break;

    case Composition_T::Type::ParticleSystem: {
      properties.insert(
          {ParticleController::Key(), std::make_unique<ParticleController>()});

      resources.push_back(spawnDependencies_.renderer->SpawnParticleSystem({
          .meshHandle = composition->meshHandle.value(),
          .particleBehaviour =
              composition->infoForType.particleSystem.behaviour,
          .transformable = reinterpret_cast<Transform*>(
              properties.at(Transform::Key()).get()),
          .spawnControllable = reinterpret_cast<ParticleController*>(
              properties.at(ParticleController::Key()).get()),
          .spawnRegionLow =
              composition->infoForType.particleSystem.spawnRegionLow,
          .spawnRegionHigh =
              composition->infoForType.particleSystem.spawnRegionHigh,
      }));
    } break;
  }

  const ActorKey key = GenerateActorKey();

  std::unique_ptr<game::Actor> actor = std::make_unique<game::Actor>(
      key, *spawnDependencies_.actorOwner, std::move(resources),
      std::move(properties));
  for (const auto& scriptFactory : composition->behaviourFactories) {
    actor->AttachBehaviour(scriptFactory(parent, *actor));
  }

  parent = actor.get();
  spawnDependencies_.actorConsumer->Consume(
      {.parent = parentKey, .key = key, .actor = std::move(actor)});

  for (const Composition& subComposition : composition->children) {
    SpawnComposition(subComposition, parent, key);
  }
}
