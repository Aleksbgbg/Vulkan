#ifndef SRC_RENDERER_VULKAN_API_SUBPASSREFERENCE_H_
#define SRC_RENDERER_VULKAN_API_SUBPASSREFERENCE_H_

#include "core/types.h"
#include "renderer/vulkan/api/RenderPass.h"

namespace vk {

struct SubpassReference {
  SubpassReference() = default;
  SubpassReference(const RenderPass& renderPass, const u32 subpass)
      : renderPass(&renderPass), subpass(subpass) {}

  const RenderPass* renderPass;
  u32 subpass;
};

}  // namespace vk

#endif  // SRC_RENDERER_VULKAN_API_SUBPASSREFERENCE_H_
