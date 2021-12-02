#ifndef VULKAN_SRC_VULKAN_SUBPASSREFERENCE_H_
#define VULKAN_SRC_VULKAN_SUBPASSREFERENCE_H_

#include "util/types.h"
#include "vulkan/RenderPass.h"

struct SubpassReference {
  SubpassReference() = default;
  SubpassReference(const RenderPass& renderPass, const u32 subpass)
      : renderPass(&renderPass), subpass(subpass) {}

  const RenderPass* renderPass;
  u32 subpass;
};

#endif  // VULKAN_SRC_VULKAN_SUBPASSREFERENCE_H_
