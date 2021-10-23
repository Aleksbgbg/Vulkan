#ifndef VULKAN_SRC_VULKAN_IMGUIINSTANCE_H
#define VULKAN_SRC_VULKAN_IMGUIINSTANCE_H

#include "VirtualDevice.h"
#include "VulkanInstance.h"
#include "general/windowing/ImGuiWindow.h"

class ImGuiInstance {
 public:
  ImGuiInstance();
  ImGuiInstance(const ImGuiWindow& window, const VulkanInstance& instance,
                const PhysicalDevice& physicalDevice,
                const VirtualDevice& virtualDevice, const Queue& queue,
                const RenderPass& renderPass,
                CommandBuffer& temporaryCommandBuffer, const Fence& fence,
                const VkSampleCountFlagBits samples);
  ~ImGuiInstance();

  ImGuiInstance(ImGuiInstance&& other) noexcept;
  ImGuiInstance& operator=(ImGuiInstance&& other) noexcept;

  void BeginFrame() const;
  void RenderFrame() const;
  void SubmitFrame(const CommandBuffer& commandBuffer) const;

 private:
  const ImGuiWindow* window;
  DescriptorPool descriptorPool;
};

#endif  // VULKAN_SRC_VULKAN_IMGUIINSTANCE_H
