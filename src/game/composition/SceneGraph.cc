#include "SceneGraph.h"

void SceneGraph::Add(const ActorKey parent, const ActorKey key,
                     std::unique_ptr<game::Actor> actor) {
  actors_.insert(std::make_pair(key, std::move(actor)));

  if (parent != 0) {
    childrenOfActor_[parent].push_back(key);
  }
}

void SceneGraph::Remove(const ActorKey key) {
  const auto actorChildrenIterator = childrenOfActor_.find(key);
  if (actorChildrenIterator != childrenOfActor_.end()) {
    for (const ActorKey child : actorChildrenIterator->second) {
      Remove(child);
    }
    childrenOfActor_.erase(actorChildrenIterator);
  }

  actors_.erase(actors_.find(key));
}

SceneGraph::IteratorType SceneGraph::begin() {
  return actors_.begin();
}

SceneGraph::IteratorType SceneGraph::end() {
  return actors_.end();
}
