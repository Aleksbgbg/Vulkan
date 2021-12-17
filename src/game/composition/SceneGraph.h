#ifndef VULKAN_SRC_GAME_COMPOSITION_SCENEGRAPH_H_
#define VULKAN_SRC_GAME_COMPOSITION_SCENEGRAPH_H_

#include <list>
#include <memory>
#include <unordered_map>

#include "game/actor/Actor.h"
#include "game/actor/ActorKey.h"

class SceneGraph {
 private:
  std::unordered_map<ActorKey, std::unique_ptr<game::Actor>> actors_;
  std::unordered_map<ActorKey, std::list<ActorKey>> childrenOfActor_;

  typedef decltype(actors_)::const_iterator IteratorType;

 public:
  void Add(const ActorKey parent, const ActorKey key,
           std::unique_ptr<game::Actor> actor);
  void Remove(const ActorKey key);

  IteratorType begin();
  IteratorType end();
};

#endif  // VULKAN_SRC_GAME_COMPOSITION_SCENEGRAPH_H_
