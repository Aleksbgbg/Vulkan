#include "Actor.h"

namespace game {

Actor::Actor(
    ActorKey key, ActorOwner& owner,
    std::list<std::unique_ptr<Resource>> resources,
    std::unordered_map<PropertyKey, std::unique_ptr<Property>> properties)
    : key_(key),
      owner_(owner),
      resources_(std::move(resources)),
      properties_(std::move(properties)),
      behaviours_() {}

void Actor::AttachBehaviour(std::unique_ptr<Behaviour> behaviour) {
  behaviour->OnSpawn();
  behaviours_.push_back(std::move(behaviour));
}

void Actor::UpdateModel(const UpdateContext& context) {
  for (const auto& behaviour : behaviours_) {
    behaviour->UpdateModel(context);
  }
}

void Actor::Despawn() {
  owner_.DespawnActor(key_);
}

}  // namespace game
