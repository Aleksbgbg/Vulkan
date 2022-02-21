#ifndef VULKAN_SRC_ENGINE_RESOURCE_RESOURCECOLLECTION_H_
#define VULKAN_SRC_ENGINE_RESOURCE_RESOURCECOLLECTION_H_

#include <memory>
#include <vector>

#include "Resource.h"

namespace game {

class ResourceCollection {
 public:
  void push_back(std::unique_ptr<Resource> resource);

 private:
  std::vector<std::unique_ptr<Resource>> resources_;
};

}  // namespace game

#endif  // VULKAN_SRC_ENGINE_RESOURCE_RESOURCECOLLECTION_H_
