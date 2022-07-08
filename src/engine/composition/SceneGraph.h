#ifndef SRC_ENGINE_COMPOSITION_SCENEGRAPH_H_
#define SRC_ENGINE_COMPOSITION_SCENEGRAPH_H_

#include <list>
#include <memory>
#include <unordered_map>

#include "core/adapters/MapValueIterator.h"
#include "engine/actor/Actor.h"
#include "engine/actor/ActorKey.h"

class SceneGraph {
 private:
  std::unordered_map<ActorKey, std::unique_ptr<game::Actor>> actors_;
  std::unordered_map<ActorKey, std::list<ActorKey>> childrenOfActor_;

  using IteratorType = MapValueIterable<const decltype(actors_)>::Iterator;

 public:
  void Add(const ActorKey parent, const ActorKey key,
           std::unique_ptr<game::Actor> actor);
  void Remove(const ActorKey key);

  IteratorType begin() const;
  IteratorType end() const;
};

#endif  // SRC_ENGINE_COMPOSITION_SCENEGRAPH_H_
