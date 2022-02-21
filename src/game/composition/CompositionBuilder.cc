#include "CompositionBuilder.h"

#include "game/actor/property/GraphicalInterface.h"
#include "game/actor/property/SoundEmitter.h"
#include "game/actor/property/Transform.h"
#include "game/actor/property/Visibility.h"
#include "game/ui/parse_xml_tree.h"
#include "game/ui/produce_ui_tree.h"

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
  composition->particleSystem.behaviour = particleBehaviour;

  return CompositionBuilder(spawnDependencies, std::move(composition));
}

CompositionBuilder CompositionBuilder::Ui(
    const SpawnDependencies spawnDependencies, const std::string_view view,
    std::unique_ptr<ViewModel> viewModel) {
  Composition composition = std::make_shared<Composition_T>();
  composition->type = Composition_T::Type::Ui;
  composition->ui = std::make_shared<Composition_T::UiInfo>();
  composition->ui->uiTree = ProduceUiTree(ui::ParseXmlTree(view), *viewModel);
  composition->ui->viewModel = std::move(viewModel);

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
  composition_->particleSystem.spawnRegionLow = spawnRegionLow;
  composition_->particleSystem.spawnRegionHigh = spawnRegionHigh;
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
    : actorKeyGenerator_(),
      spawnDependencies_(spawnDependencies),
      composition_(std::move(composition)) {}

void CompositionBuilder::SpawnComposition(const SpawnDependencies& dependencies,
                                          const Composition& composition,
                                          game::Actor* parent,
                                          const ActorKey parentKey) {
  const Transform* parentTransform;

  if (parent == nullptr) {
    parentTransform = nullptr;
  } else {
    parentTransform = &parent->RetrieveProperty<Transform>();
  }

  std::list<std::unique_ptr<Resource>> resources;

  game::PropertyCollection properties;
  properties.EmplaceProperty<Transform>(parentTransform);
  properties.EmplaceProperty<SoundEmitter>(*dependencies.sound);
  properties.EmplaceProperty<Visibility>();

  const Transform& selfTransform = properties.RetrieveProperty<Transform>();
  Visibility& visibility = properties.RetrieveProperty<Visibility>();

  if (composition->lightSource.has_value()) {
    resources.push_back(dependencies.renderer->SpawnLightSource(
        {.lightSource = composition->lightSource.value(),
         .transformable = selfTransform}));
  }

  std::unique_ptr<UiDrawList> drawList = nullptr;

  switch (composition->type) {
    case Composition_T::Type::Actor: {
      const bool isRenderable = composition->meshHandle.has_value();
      if (isRenderable) {
        resources.push_back(dependencies.renderer->SpawnRenderable(
            {.meshHandle = composition->meshHandle.value(),
             .transformable = selfTransform,
             .visible = visibility}));
      }
    } break;

    case Composition_T::Type::ParticleSystem:
      resources.push_back(dependencies.renderer->SpawnParticleSystem({
          .meshHandle = composition->meshHandle.value(),
          .particleBehaviour = composition->particleSystem.behaviour,
          .transformable = selfTransform,
          .visible = visibility,
          .spawnRegionLow = composition->particleSystem.spawnRegionLow,
          .spawnRegionHigh = composition->particleSystem.spawnRegionHigh,
      }));
      break;

    case Composition_T::Type::Ui: {
      properties.EmplaceProperty<GraphicalInterface>(visibility);

      if (composition->ui->uiTree.has_value()) {
        drawList = std::make_unique<UiDrawList>(
            *dependencies.fontAtlas,
            properties.RetrieveProperty<GraphicalInterface>(),
            std::move(composition->ui->uiTree.value()),
            dependencies.window->GetSize());
      }
      dependencies.window->Consume(*drawList);
      resources.push_back(dependencies.renderer->SpawnUi(
          {.drawList = drawList.get(), .visible = visibility}));
      resources.push_back(std::move(composition->ui->viewModel));
    } break;
  }

  const ActorKey key = dependencies.actorKeyGenerator->CreateKey();

  std::unique_ptr<game::Actor> actor = std::make_unique<game::Actor>(
      key, *dependencies.actorOwner, std::move(resources),
      std::move(properties));
  if (drawList != nullptr) {
    actor->AttachUpdateable(std::move(drawList));
  }
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
