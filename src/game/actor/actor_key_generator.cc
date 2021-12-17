#include "actor_key_generator.h"

ActorKey GenerateActorKey() {
  static ActorKey currentKey = 0;
  return ++currentKey;
}
