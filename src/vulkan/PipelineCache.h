#ifndef VULKAN_SRC_VULKAN_PIPELINECACHE_H
#define VULKAN_SRC_VULKAN_PIPELINECACHE_H

#include <vulkan/vulkan.h>

#include <vector>

#include "lifetime_semantics.h"
#include "structures/PipelineCacheCreateInfo.h"

class PipelineCache {
  friend class Pipeline;

 public:
  VULKAN_OBJECT_MOVABLE_ROOT(PipelineCache, device, pipelineCache)

  PipelineCache() = default;
  PipelineCache(VkDevice device, PipelineCacheCreateInfoBuilder& infoBuilder);

  PipelineCache(const PipelineCache&) = delete;

  ~PipelineCache();

  PipelineCache& operator=(const PipelineCache&) = delete;

  std::vector<u8> GetPipelineCacheData() const;

 private:
  VkDevice device;
  VkPipelineCache pipelineCache = nullptr;
};

#endif  // VULKAN_SRC_VULKAN_PIPELINECACHE_H
