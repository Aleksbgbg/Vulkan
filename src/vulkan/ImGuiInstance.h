#ifndef VULKAN_SRC_VULKAN_IMGUIINSTANCE_H
#define VULKAN_SRC_VULKAN_IMGUIINSTANCE_H

#include "VirtualDevice.h"
#include "VulkanInstance.h"
#include "util/include/sdl.h"

class ImGuiInstance {
 public:
  ImGuiInstance();
  ImGuiInstance(SDL_Window* const sdlWindow, const VulkanInstance& instance,
                const PhysicalDevice& physicalDevice,
                const VirtualDevice& virtualDevice, const Queue& queue,
                const RenderPass& renderPass,
                CommandBuffer& temporaryCommandBuffer, const Fence& fence,
                const VkSampleCountFlagBits samples);
  ~ImGuiInstance();

  ImGuiInstance(ImGuiInstance&& other) noexcept;
  ImGuiInstance& operator=(ImGuiInstance&& other) noexcept;

  void ProcessEvent(const SDL_Event& event) const;

  void BeginFrame() const;
  void RenderFrame() const;
  void SubmitFrame(const CommandBuffer& commandBuffer) const;

 private:
  SDL_Window* sdlWindow;
  DescriptorPool descriptorPool;
};

#endif  // VULKAN_SRC_VULKAN_IMGUIINSTANCE_H