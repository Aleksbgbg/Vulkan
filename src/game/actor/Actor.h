#ifndef VULKAN_SRC_GAME_ACTOR_ACTOR_H_
#define VULKAN_SRC_GAME_ACTOR_ACTOR_H_

#include <list>
#include <memory>
#include <vector>

#include "ActorOwner.h"
#include "engine/property/PropertyCollection.h"
#include "engine/resource/ResourceCollection.h"
#include "game/UpdateContext.h"
#include "game/actor/behaviour/Behaviour.h"

namespace game {

class Actor {
 public:
  Actor(ActorKey key, ActorOwner& owner, ResourceCollection resources,
        PropertyCollection properties);

  void AttachBehaviour(std::unique_ptr<Behaviour> behaviour);
  void AttachUpdateable(std::unique_ptr<Updateable> updateable);

  void UpdateModel(const UpdateContext& context);

  void Despawn();

  template <typename T>
  const T& RetrieveProperty() const {
    return properties_.RetrieveProperty<T>();
  }
  template <typename T>
  T& RetrieveProperty() {
    return properties_.RetrieveProperty<T>();
  }

 private:
  ActorKey key_;
  ActorOwner& owner_;
  game::ResourceCollection resources_;
  game::PropertyCollection properties_;
  std::vector<std::unique_ptr<Updateable>> updateables_;
};

}  // namespace game

#endif  // VULKAN_SRC_GAME_ACTOR_ACTOR_H_
