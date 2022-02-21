#ifndef VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_INSERTCOMPUTEINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_INSERTCOMPUTEINFO_H_

#include <memory>

#include "HostDescriptorWriter.h"
#include "core/types.h"

struct InsertComputeInfo {
  u32 instances;
  std::unique_ptr<HostDescriptorWriter> descriptorWriter;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_INSERTCOMPUTEINFO_H_
