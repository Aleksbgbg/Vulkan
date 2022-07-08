#ifndef SRC_ENGINE_ACTOR_ACTOROWNER_H_
#define SRC_ENGINE_ACTOR_ACTOROWNER_H_

#include "ActorKey.h"

class ActorOwner {
 public:
  virtual ~ActorOwner() = default;

  virtual void DespawnActor(const ActorKey key) = 0;
};

#endif  // SRC_ENGINE_ACTOR_ACTOROWNER_H_
