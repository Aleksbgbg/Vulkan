#ifndef VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_BINDINGS_H_
#define VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_BINDINGS_H_

#include "renderer/vulkan/api/Sampler.h"
#include "renderer/vulkan/render_graph/layout/Binding.h"

enum StructureFlags {
  STRUCTURE_FLAGS_NONE = 0x0,
  STRUCTURE_FLAGS_HOST_ACCESSIBLE = 0x1,
};

template <typename T>
Binding UniformStructure(const StructureFlags flags = STRUCTURE_FLAGS_NONE) {
  return Binding{.size = sizeof(T),
                 .hostAccessible = ((flags & STRUCTURE_FLAGS_HOST_ACCESSIBLE) ==
                                    STRUCTURE_FLAGS_HOST_ACCESSIBLE),
                 .bufferUsage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                 .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER};
}

template <typename T>
Binding UniformStructurePerInstance() {
  return Binding{.size = sizeof(T),
                 .instanced = true,
                 .bufferUsage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                 .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER};
}

template <typename T>
Binding BufferStructure() {
  return Binding{.size = sizeof(T),
                 .bufferUsage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                 .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER};
}

template <typename T>
Binding BufferStructurePerInstance() {
  return Binding{.size = sizeof(T),
                 .instanced = true,
                 .bufferUsage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                 .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER};
}

Binding TextureSampler(const vk::Sampler& sampler) {
  return Binding{.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                 .sampler = &sampler};
}

#endif  // VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_LAYOUT_BUILDER_BINDINGS_H_
