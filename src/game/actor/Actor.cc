#include "Actor.h"

namespace game {

Actor::Actor(ActorKey key, ActorOwner& owner,
             std::list<std::unique_ptr<Resource>> resources,
             PropertyCollection properties)
    : key_(key),
      owner_(owner),
      resources_(std::move(resources)),
      properties_(std::move(properties)),
      updateables_() {}

void Actor::AttachBehaviour(std::unique_ptr<Behaviour> behaviour) {
  behaviour->OnSpawn();
  updateables_.push_back(std::move(behaviour));
}

void Actor::AttachUpdateable(std::unique_ptr<Updateable> updateable) {
  updateables_.push_back(std::move(updateable));
}

void Actor::UpdateModel(const UpdateContext& context) {
  for (const auto& updateable : updateables_) {
    updateable->UpdateModel(context);
  }
}

void Actor::Despawn() {
  owner_.DespawnActor(key_);
}

}  // namespace game
