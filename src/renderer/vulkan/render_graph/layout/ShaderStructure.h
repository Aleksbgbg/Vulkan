#ifndef VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_SHADERSTRUCTURE_H_
#define VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_SHADERSTRUCTURE_H_

#include <vulkan/vulkan.h>

#include <list>

#include "util/types.h"

struct ShaderStructure {
  VkShaderStageFlagBits shaderStage;
  std::list<u32> globalBindings;
  std::list<u32> localBindings;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_SHADERSTRUCTURE_H_
