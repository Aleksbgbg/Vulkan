#ifndef VULKAN_SRC_VULKAN_SUBPASSREFERENCE_H
#define VULKAN_SRC_VULKAN_SUBPASSREFERENCE_H

struct SubpassReference{
  SubpassReference(RenderPass& renderPass, const u32 subpass) : renderPass(renderPass), subpass(subpass) {}

  RenderPass& renderPass;
  u32 subpass;
};

#endif // VULKAN_SRC_VULKAN_SUBPASSREFERENCE_H
