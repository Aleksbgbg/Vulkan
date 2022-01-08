#ifndef VULKAN_SRC_GAME_ACTOR_RESOURCE_RESOURCELIST_H_
#define VULKAN_SRC_GAME_ACTOR_RESOURCE_RESOURCELIST_H_

#include <list>
#include <memory>
#include <unordered_map>

#include "ReleaseListResource.h"
#include "Resource.h"
#include "ResourceKey.h"
#include "general/adapters/MapValueIterator.h"

template <typename TResource>
class ResourceList {
 public:
  std::unique_ptr<Resource> Insert(TResource&& resource) {
    const ResourceKey key = resourceKeyGenerator_.CreateKey();
    resources_.insert(std::make_pair(key, std::move(resource)));
    return std::make_unique<ReleaseListResource<ResourceKey>>(
        key, resourcesToRelease_);
  }

  void ReleasePendingResources() {
    for (const ResourceKey key : resourcesToRelease_) {
      resources_.erase(key);
    }
    resourcesToRelease_.clear();
  }

  std::list<TResource> AcquireResourcesPendingRelease() {
    std::list<TResource> resources;

    for (const ResourceKey key : resourcesToRelease_) {
      const auto iterator = resources_.find(key);
      resources.push_back(std::move(iterator->second));
      resources_.erase(iterator);
    }
    resourcesToRelease_.clear();

    return std::move(resources);
  }

  std::size_t size() const {
    return resources_.size();
  }

  auto begin() {
    return IterateValues(resources_).begin();
  }

  auto end() {
    return IterateValues(resources_).end();
  }

  auto begin() const {
    return IterateValues(resources_).begin();
  }

  auto end() const {
    return IterateValues(resources_).end();
  }

 private:
  ResourceKeyGenerator resourceKeyGenerator_;
  std::unordered_map<ResourceKey, TResource> resources_;
  std::list<ResourceKey> resourcesToRelease_;
};

#endif  // VULKAN_SRC_GAME_ACTOR_RESOURCE_RESOURCELIST_H_
