#include "Actor.h"

namespace game {

Actor::Actor(
    std::unordered_map<PropertyKey, std::unique_ptr<Property>> properties)
    : properties_(std::move(properties)), scripts_() {}

void Actor::AttachScript(std::unique_ptr<Behaviour> script) {
  script->OnSpawn();
  scripts_.push_back(std::move(script));
}

void Actor::UpdateModel(const UpdateContext& context) {
  for (const auto& script : scripts_) {
    script->UpdateModel(context);
  }
}

}  // namespace game
