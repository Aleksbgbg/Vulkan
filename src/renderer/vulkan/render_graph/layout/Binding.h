#ifndef SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BINDING_H_
#define SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BINDING_H_

#include <vulkan/vulkan.h>

#include <cstddef>

#include "core/types.h"
#include "renderer/vulkan/api/Sampler.h"

struct Binding {
  u32 index;
  std::size_t size;
  bool instanced;
  bool hostAccessible;
  VkBufferUsageFlags bufferUsage;
  VkDescriptorType type;
  VkShaderStageFlags stageFlags;
  const vk::Sampler* sampler;
};

#endif  // SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BINDING_H_
