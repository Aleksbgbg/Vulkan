#ifndef VULKAN_SRC_RENDERER_VULKAN_TRANSFORMDESCRIPTORWRITER_H_
#define VULKAN_SRC_RENDERER_VULKAN_TRANSFORMDESCRIPTORWRITER_H_

#include <unordered_map>
#include <vector>

#include "game/Transformable.h"
#include "renderer/vulkan/render_graph/HostDescriptorWriter.h"

class TransformDescriptorWriter : public HostDescriptorWriter {
 public:
  TransformDescriptorWriter(const Transformable& transformable);

  void Fill(
      const std::unordered_map<u32, Descriptor>& descriptors) const override;

 private:
  const Transformable& transformable_;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_TRANSFORMDESCRIPTORWRITER_H_
