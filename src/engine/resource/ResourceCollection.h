#ifndef VULKAN_SRC_ENGINE_RESOURCE_RESOURCECOLLECTION_H_
#define VULKAN_SRC_ENGINE_RESOURCE_RESOURCECOLLECTION_H_

#include <array>
#include <memory>
#include <vector>

#include "Resource.h"

namespace game {

class ResourceCollection : public Resource {
 public:
  ResourceCollection() = default;

  template <std::size_t N>
  ResourceCollection(std::array<std::unique_ptr<Resource>, N> resources)
      : ResourceCollection() {
    for (auto& resource : resources) {
      push_back(std::move(resource));
    }
  }

  void push_back(std::unique_ptr<Resource> resource);

 private:
  std::vector<std::unique_ptr<Resource>> resources_;
};

}  // namespace game

#endif  // VULKAN_SRC_ENGINE_RESOURCE_RESOURCECOLLECTION_H_
