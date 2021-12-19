#include "CompositionBuilder.h"

#include "game/actor/actor_key_generator.h"
#include "game/actor/property/ParticleController.h"
#include "game/actor/property/SoundEmitter.h"
#include "game/actor/property/Transform.h"

CompositionBuilder CompositionBuilder::Actor(
    const SpawnDependencies spawnDependencies) {
  Composition composition = std::make_shared<Composition_T>();
  composition->type = Composition_T::Type::Actor;

  return CompositionBuilder(spawnDependencies, std::move(composition));
}

CompositionBuilder CompositionBuilder::ParticleSystem(
    const SpawnDependencies spawnDependencies,
    const ParticleBehaviour particleBehaviour) {
  Composition composition = std::make_shared<Composition_T>();
  composition->type = Composition_T::Type::ParticleSystem;
  composition->infoForType.particleSystem.behaviour = particleBehaviour;

  return CompositionBuilder(spawnDependencies, std::move(composition));
}

CompositionBuilder CompositionBuilder::Copy() const {
  Composition copy = std::make_shared<Composition_T>();
  *copy = *composition_;
  return CompositionBuilder(spawnDependencies_, std::move(copy));
}

CompositionBuilder& CompositionBuilder::Attach(BehaviourFactory scriptFactory) {
  composition_->behaviourFactories.push_back(std::move(scriptFactory));
  return *this;
}

CompositionBuilder& CompositionBuilder::LightSource(
    const LightSourceComposition& lightSourceComposition) {
  composition_->lightSource = lightSourceComposition.GetLightSource();
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
  SpawnComposition(spawnDependencies_, composition_, nullptr, 0);
}

CompositionBuilder::CompositionBuilder(SpawnDependencies spawnDependencies,
                                       Composition composition)
    : spawnDependencies_(spawnDependencies),
      composition_(std::move(composition)) {}

void CompositionBuilder::SpawnComposition(const SpawnDependencies& dependencies,
                                          const Composition& composition,
                                          const game::Actor* parent,
                                          const ActorKey parentKey) {
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
  properties.insert({SoundEmitter::Key(),
                     std::make_unique<SoundEmitter>(*dependencies.sound)});

  const Transformable* selfTransform =
      reinterpret_cast<Transform*>(properties.at(Transform::Key()).get());

  if (composition->lightSource.has_value()) {
    resources.push_back(dependencies.renderer->SpawnLightSource(
        {.lightSource = composition->lightSource.value(),
         .transformable = selfTransform}));
  }

  switch (composition->type) {
    case Composition_T::Type::Actor: {
      const bool isRenderable = composition->meshHandle.has_value();
      if (isRenderable) {
        resources.push_back(dependencies.renderer->SpawnRenderable({
            .meshHandle = composition->meshHandle.value(),
            .transformable = selfTransform,
        }));
      }
    } break;

    case Composition_T::Type::ParticleSystem: {
      properties.insert(
          {ParticleController::Key(), std::make_unique<ParticleController>()});

      resources.push_back(dependencies.renderer->SpawnParticleSystem({
          .meshHandle = composition->meshHandle.value(),
          .particleBehaviour =
              composition->infoForType.particleSystem.behaviour,
          .transformable = selfTransform,
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
      key, *dependencies.actorOwner, std::move(resources),
      std::move(properties));
  for (const auto& scriptFactory : composition->behaviourFactories) {
    actor->AttachBehaviour(scriptFactory(parent, *actor));
  }

  parent = actor.get();
  dependencies.actorConsumer->Consume(
      {.parent = parentKey, .key = key, .actor = std::move(actor)});

  for (const Composition& subComposition : composition->children) {
    SpawnComposition(dependencies, subComposition, parent, key);
  }
}
