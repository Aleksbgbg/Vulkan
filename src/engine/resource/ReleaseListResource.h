#ifndef SRC_ENGINE_RESOURCE_RELEASELISTRESOURCE_H_
#define SRC_ENGINE_RESOURCE_RELEASELISTRESOURCE_H_

#include <list>

#include "Resource.h"

template <typename TKey>
class ReleaseListResource : public Resource {
 public:
  ReleaseListResource(TKey resourceKey, std::list<TKey>& resourcesToRelease)
      : resourceKey_(resourceKey), releaseList_(&resourcesToRelease) {}

  ReleaseListResource(const ReleaseListResource&) = delete;
  ReleaseListResource(ReleaseListResource&& other) noexcept
      : resourceKey_(other.resourceKey_), releaseList_(other.releaseList_) {
    other.releaseList_ = nullptr;
  }

  ~ReleaseListResource() override {
    if (releaseList_ != nullptr) {
      releaseList_->push_back(resourceKey_);
    }
  }

  ReleaseListResource& operator=(const ReleaseListResource&) = delete;
  ReleaseListResource& operator=(ReleaseListResource&& other) noexcept {
    this->~ReleaseListResource();
    new (this) ReleaseListResource(std::move(other));
    return *this;
  }

 private:
  TKey resourceKey_;
  std::list<TKey>* releaseList_;
};

#endif  // SRC_ENGINE_RESOURCE_RELEASELISTRESOURCE_H_
