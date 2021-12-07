#ifndef VULKAN_SRC_GAME_ACTOR_ACTOR_H_
#define VULKAN_SRC_GAME_ACTOR_ACTOR_H_

#include <memory>
#include <unordered_map>

#include "game/UpdateContext.h"
#include "game/actor/behaviour/Behaviour.h"
#include "game/actor/property/Property.h"
#include "game/actor/property/PropertyKey.h"

namespace game {

class Actor {
 public:
  Actor(std::unordered_map<PropertyKey, std::unique_ptr<Property>> properties);

  void AttachScript(std::unique_ptr<Behaviour> script);

  void UpdateModel(const UpdateContext& context);

  template <typename T>
  T& RetrieveProperty() const {
    return *reinterpret_cast<T*>(properties_.at(T::Key()).get());
  }

 private:
  std::unordered_map<PropertyKey, std::unique_ptr<Property>> properties_;
  std::vector<std::unique_ptr<Behaviour>> scripts_;
};

}  // namespace game

#endif  // VULKAN_SRC_GAME_ACTOR_ACTOR_H_
