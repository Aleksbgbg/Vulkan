#ifndef SRC_RENDERER_VULKAN_API_PIPELINECACHE_H_
#define SRC_RENDERER_VULKAN_API_PIPELINECACHE_H_

#include <vulkan/vulkan.h>

#include <vector>

#include "renderer/vulkan/api/structures/PipelineCacheCreateInfo.h"

namespace vk {

class PipelineCache {
  friend class GraphicsPipeline;
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

}  // namespace vk

#endif  // SRC_RENDERER_VULKAN_API_PIPELINECACHE_H_
