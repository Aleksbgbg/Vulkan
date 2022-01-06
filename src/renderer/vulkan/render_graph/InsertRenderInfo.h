#ifndef VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_INSERTRENDERINFO_H_
#define VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_INSERTRENDERINFO_H_

#include <memory>
#include <vector>

#include "HostDescriptorWriter.h"
#include "renderer/vulkan/IndexedVertexBuffer.h"
#include "renderer/vulkan/Texture.h"
#include "util/types.h"

struct InsertRenderInfo {
  u32 instances;
  const IndexedVertexBuffer* drawBuffer;
  const std::vector<Texture>* textures;
  std::unique_ptr<HostDescriptorWriter> descriptorWriter;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_RENDER_GRAPH_INSERTRENDERINFO_H_
