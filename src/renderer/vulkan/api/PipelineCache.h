#ifndef VULKAN_SRC_RENDERER_VULKAN_API_PIPELINECACHE_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_PIPELINECACHE_H_

#include <vulkan/vulkan.h>

#include <vector>

#include "renderer/vulkan/api/structures/PipelineCacheCreateInfo.h"

class PipelineCache {
  friend class Pipeline;
  friend class ComputePipeline;

 public:
  PipelineCache();
  PipelineCache(VkDevice device,
                const PipelineCacheCreateInfoBuilder& infoBuilder);

  PipelineCache(const PipelineCache&) = delete;
  PipelineCache(PipelineCache&& other) noexcept;

  ~PipelineCache();

  PipelineCache& operator=(const PipelineCache&) = delete;
  PipelineCache& operator=(PipelineCache&& other) noexcept;

  std::vector<u8> GetPipelineCacheData() const;

 private:
  VkDevice device;
  VkPipelineCache pipelineCache;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_PIPELINECACHE_H_
