#include "PipelineCache.h"

#include "error.h"

PipelineCache::PipelineCache(VkDevice device,
                             PipelineCacheCreateInfoBuilder& infoBuilder)
    : device(device) {
  PROCEED_ON_VALID_RESULT(vkCreatePipelineCache(device, infoBuilder.Build(),
                                                nullptr, &pipelineCache));
}

PipelineCache::~PipelineCache() {
  if (pipelineCache != nullptr) {
    vkDestroyPipelineCache(device, pipelineCache, nullptr);
  }
}

std::vector<u8> PipelineCache::GetPipelineCacheData() const {
  size_t dataSize;
  PROCEED_ON_VALID_RESULT(
      vkGetPipelineCacheData(device, pipelineCache, &dataSize, nullptr));
  std::vector<u8> data(dataSize);
  PROCEED_ON_VALID_RESULT(
      vkGetPipelineCacheData(device, pipelineCache, &dataSize, data.data()));
  return data;
}
