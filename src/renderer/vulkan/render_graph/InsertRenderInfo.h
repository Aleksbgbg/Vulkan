#ifndef SRC_RENDERER_VULKAN_RENDER_GRAPH_INSERTRENDERINFO_H_
#define SRC_RENDERER_VULKAN_RENDER_GRAPH_INSERTRENDERINFO_H_

#include <memory>
#include <vector>

#include "HostDescriptorWriter.h"
#include "core/types.h"
#include "engine/property/traits/Visible.h"
#include "renderer/vulkan/IndexedVertexBuffer.h"
#include "renderer/vulkan/Texture.h"

struct InsertRenderInfo {
  const Visible& visible;
  u32 instances;
  const IndexedVertexBuffer* drawBuffer;
  const std::vector<Texture>* textures;
  std::unique_ptr<HostDescriptorWriter> descriptorWriter;
};

#endif  // SRC_RENDERER_VULKAN_RENDER_GRAPH_INSERTRENDERINFO_H_
