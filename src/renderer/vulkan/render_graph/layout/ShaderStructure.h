#ifndef SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_SHADERSTRUCTURE_H_
#define SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_SHADERSTRUCTURE_H_

#include <vulkan/vulkan.h>

#include <list>

#include "core/types.h"

struct ShaderStructure {
  VkShaderStageFlagBits shaderStage;
  std::list<u32> globalBindings;
  std::list<u32> localBindings;
};

#endif  // SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_SHADERSTRUCTURE_H_
