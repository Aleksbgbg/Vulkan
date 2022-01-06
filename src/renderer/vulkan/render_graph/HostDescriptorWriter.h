#ifndef VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_HOSTDESCRIPTORWRITER_H_
#define VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_HOSTDESCRIPTORWRITER_H_

#include <unordered_map>

#include "renderer/vulkan/BoundBuffer.h"

class HostDescriptorWriter {
 public:
  virtual ~HostDescriptorWriter() = default;

  struct Descriptor {
    const BoundBuffer& buffer;
  };
  virtual void Fill(
      const std::unordered_map<u32, Descriptor>& descriptors) const = 0;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_HOSTDESCRIPTORWRITER_H_
