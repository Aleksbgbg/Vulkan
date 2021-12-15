#include "CompositionBuilder.h"

#include "game/actor/property/ParticleController.h"
#include "game/actor/property/SoundEmitter.h"
#include "game/actor/property/Transform.h"

CompositionBuilder CompositionBuilder::ForActor(ActorConsumer& actorConsumer,
                                                Renderer& renderer,
                                                sys::Sound& sound) {
  Composition composition = std::make_unique<Composition_T>();
  composition->type = Composition_T::Type::Actor;

  return CompositionBuilder(actorConsumer, renderer, sound,
                            std::move(composition));
}

CompositionBuilder CompositionBuilder::ForParticleSystem(
    ActorConsumer& actorConsumer, Renderer& renderer, sys::Sound& sound,
    const ParticleBehaviour particleBehaviour) {
  Composition composition = std::make_unique<Composition_T>();
  composition->type = Composition_T::Type::ParticleSystem;
  composition->infoForType.particleSystem.behaviour = particleBehaviour;

  return CompositionBuilder(actorConsumer, renderer, sound,
                            std::move(composition));
}

CompositionBuilder& CompositionBuilder::Attach(
    const std::function<std::unique_ptr<BehaviourFactory>()> scriptFactory) {
  composition_->scripts.push_back(scriptFactory());
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
  SpawnComposition(composition_, nullptr);
}

CompositionBuilder::CompositionBuilder(ActorConsumer& actorConsumer,
                                       Renderer& renderer, sys::Sound& sound,
                                       Composition composition)
    : actorConsumer_(&actorConsumer),
      renderer_(&renderer),
      sound_(&sound),
      composition_(std::move(composition)) {}

void CompositionBuilder::SpawnComposition(const Composition& composition,
                                          const game::Actor* parent) const {
  const Transformable* parentTransform;

  if (parent == nullptr) {
    parentTransform = nullptr;
  } else {
    parentTransform = &parent->RetrieveProperty<Transform>();
  }

  std::unordered_map<PropertyKey, std::unique_ptr<Property>> properties;
  properties.insert(
      {Transform::Key(), std::make_unique<Transform>(parentTransform)});
  properties.insert(
      {SoundEmitter::Key(), std::make_unique<SoundEmitter>(*sound_)});

  if (composition->type == Composition_T::Type::ParticleSystem) {
    properties.insert(
        {ParticleController::Key(), std::make_unique<ParticleController>()});
  }

  std::unique_ptr<game::Actor> actor =
      std::make_unique<game::Actor>(std::move(properties));
  for (const auto& scriptFactory : composition->scripts) {
    actor->AttachScript(scriptFactory->operator()(parent, *actor));
  }

  switch (composition->type) {
    case Composition_T::Type::Actor: {
      const bool isRenderable = composition->meshHandle.has_value();
      if (isRenderable) {
        renderer_->SpawnRenderable(
            {.meshHandle = composition->meshHandle.value(),
             .transformable = &actor->RetrieveProperty<Transform>()});
      }

      parent = actor.get();
      actorConsumer_->Consume(std::move(actor));

      for (const Composition& subComposition : composition->children) {
        SpawnComposition(subComposition, parent);
      }
    } break;

    case Composition_T::Type::ParticleSystem: {
      renderer_->SpawnParticleSystem({
          .meshHandle = composition->meshHandle.value(),
          .particleBehaviour =
              composition->infoForType.particleSystem.behaviour,
          .transformable = &actor->RetrieveProperty<Transform>(),
          .spawnControllable = &actor->RetrieveProperty<ParticleController>(),
          .spawnRegionLow =
              composition->infoForType.particleSystem.spawnRegionLow,
          .spawnRegionHigh =
              composition->infoForType.particleSystem.spawnRegionHigh,
      });

      actorConsumer_->Consume(std::move(actor));
    } break;
  }
}
