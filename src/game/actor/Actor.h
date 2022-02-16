#ifndef VULKAN_SRC_GAME_ACTOR_ACTOR_H_
#define VULKAN_SRC_GAME_ACTOR_ACTOR_H_

#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

#include "ActorOwner.h"
#include "game/UpdateContext.h"
#include "game/actor/behaviour/Behaviour.h"
#include "game/actor/property/Property.h"
#include "game/actor/property/PropertyKey.h"
#include "game/actor/resource/Resource.h"

namespace game {

class Actor {
 public:
  Actor(ActorKey key, ActorOwner& owner,
        std::list<std::unique_ptr<Resource>> resources,
        std::unordered_map<PropertyKey, std::unique_ptr<Property>> properties);

  void AttachBehaviour(std::unique_ptr<Behaviour> behaviour);
  void AttachUpdateable(std::unique_ptr<Updateable> updateable);

  void UpdateModel(const UpdateContext& context);

  template <typename T>
  T& RetrieveProperty() const {
    return *reinterpret_cast<T*>(properties_.at(T::Key()).get());
  }

  void Despawn();

 private:
  ActorKey key_;
  ActorOwner& owner_;
  std::list<std::unique_ptr<Resource>> resources_;
  std::unordered_map<PropertyKey, std::unique_ptr<Property>> properties_;
  std::vector<std::unique_ptr<Updateable>> updateables_;
};

}  // namespace game

#endif  // VULKAN_SRC_GAME_ACTOR_ACTOR_H_
