#ifndef VULKAN_SRC_RENDERER_VULKAN_API_SUBPASSREFERENCE_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_SUBPASSREFERENCE_H_

#include "renderer/vulkan/api/RenderPass.h"
#include "util/types.h"

namespace vk {

struct SubpassReference {
  SubpassReference() = default;
  SubpassReference(const RenderPass& renderPass, const u32 subpass)
      : renderPass(&renderPass), subpass(subpass) {}

  const RenderPass* renderPass;
  u32 subpass;
};

}  // namespace vk

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_SUBPASSREFERENCE_H_
