#ifndef VULKAN_SRC_GAME_ACTOR_RESOURCE_RELEASELISTRESOURCE_H_
#define VULKAN_SRC_GAME_ACTOR_RESOURCE_RELEASELISTRESOURCE_H_

#include <list>

#include "Resource.h"

template <typename TKey>
class ReleaseListResource : public Resource {
 public:
  ReleaseListResource(TKey resourceKey, std::list<TKey>& resourcesToRelease)
      : resourceKey_(resourceKey), releaseList_(resourcesToRelease) {}

  ReleaseListResource(ReleaseListResource&) = delete;
  ReleaseListResource(ReleaseListResource&&) = delete;

  ~ReleaseListResource() override {
    releaseList_.push_back(resourceKey_);
  }

  ReleaseListResource& operator=(ReleaseListResource&) = delete;
  ReleaseListResource& operator=(ReleaseListResource&&) = delete;

 private:
  TKey resourceKey_;
  std::list<TKey>& releaseList_;
};

#endif  // VULKAN_SRC_GAME_ACTOR_RESOURCE_RELEASELISTRESOURCE_H_
