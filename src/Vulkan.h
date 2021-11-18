#ifndef VULKAN_SRC_VULKAN_H
#define VULKAN_SRC_VULKAN_H

#include <vulkan/vulkan.h>

#include "DynamicUniformBuffer.h"
#include "SwapchainWithResources.h"
#include "Vulkan.h"
#include "game/Scene.h"
#include "game/rendering/resources/ResourceLoader.h"
#include "general/animations/NormalizedOneTimeFunctionAnimation.h"
#include "general/geometry/Rect.h"
#include "general/threading/MultithreadedMessageQueue.h"
#include "general/windowing/Window.h"
#include "general/windowing/input/Keyboard.h"
#include "general/windowing/input/Mouse.h"
#include "memory/DeviceMemoryAllocator.h"
#include "vulkan/Swapchain.h"
#include "vulkan/VulkanInstance.h"

// Top-level class which contains all Vulkan resources and configuration
// Implements interfaces used to call Vulkan by higher-level logic
class Vulkan : public SwapchainWithResources::Initializer,
               public Scene::Initializer,
               public DynamicUniformBufferInitializer,
               public RenderPipeline::Initializer,
               public DescriptorSetLayoutFactory,
               public ShaderModuleFactory,
               public ResourceLoader,
               public ResourceBinder::ImageSamplerWriter {
 public:
  Vulkan(const VulkanWindow& window);
  ~Vulkan();

  CommandBuffer AllocatePrimaryRenderCommandBuffer() const;
  Fence CreateFence(const VkFenceCreateFlags flags) const;
  void CmdBeginRenderPass(const CommandBuffer& commandBuffer,
                          const RenderPassBeginInfoBuilder& infoBuilder,
                          const Framebuffer& framebuffer) const;
  void Present(const SwapchainWithResources& swapchain,
               const SynchronisationPack& synchronisation) const;
  void WaitIdle() const;

  Texture LoadTexture(const std::string_view filename) override;
  BufferWithMemory AllocateLocalBuffer(const std::size_t size,
                                       const VkBufferUsageFlags usage) override;
  BufferWithMemory AllocateDeviceBuffer(
      const void* const data, const std::size_t size,
      const VkBufferUsageFlags usage) override;

  Swapchain CreateSwapchain() const override;
  Swapchain CreateSwapchain(const Swapchain& oldSwapchain) const override;
  std::vector<Framebuffer> GetFramebuffers(
      const Swapchain& swapchain,
      const std::vector<const ImageView*>& attachments) const override;
  ImageWithMemory CreateDepthStencilAttachment(
      const Swapchain& swapchain) override;
  ImageWithMemory CreateMultisamplingAttachment(
      const Swapchain& swapchain) override;
  Semaphore CreateSemaphore() const override;

  u32 PaddedSize(const u32 elementSize) const override;
  BufferWithMemory CreateBuffer(const u32 elementPaddedSize) override;

  Pipeline CreateGraphicsPipeline(
      const std::vector<ShaderModule>& shaders,
      const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts,
      const PipelineLayoutCreateInfoBuilder& pipelineLayout,
      const GraphicsPipelineCreateInfoBuilder& infoBuilder) const override;
  PipelineMultisampleStateCreateInfoBuilder CreateMultisampleState()
      const override;

  DescriptorSet::WriteDescriptorSet CreateImageSamplerWrite(
      const DescriptorSet& descriptorSet, const ImageView& imageView,
      const u32 binding) const override;

  ShaderModule LoadShader(const VkShaderStageFlagBits stage,
                          const std::string_view shaderFilename) const override;

  DescriptorSetLayout CreateDescriptorSetLayout(
      const DescriptorSetLayoutCreateInfoBuilder& infoBuilder) const override;

  DescriptorPool CreateDescriptorPool(
      const DescriptorPoolCreateInfoBuilder& infoBuilder) const override;
  void UpdateDescriptorSets(
      const u32 descriptorSetCount,
      const VkWriteDescriptorSet* const descriptorSetWrites) const override;

 private:
  static VKAPI_ATTR VkBool32 VKAPI_CALL
  DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData);

  VkFormat SelectDepthStencilFormat(
      const std::vector<VkFormat>& potentialFormats) const;
  VkSurfaceFormatKHR SelectSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR>& availableSurfaceFormats) const;
  static VkExtent2D SelectSwapExtent(
      const VkSurfaceCapabilitiesKHR surfaceCapabilities);
  static VkPresentModeKHR SelectSwapPresentMode(
      const std::vector<VkPresentModeKHR>& availablePresentModes);
  VkSampleCountFlagBits SelectMsaaSamples(
      const VkSampleCountFlagBits preferred) const;
  RenderPass CreateRenderPass() const;

  SwapchainCreateInfoBuilder SwapchainCreateInfo() const;

 private:
  VulkanInstance instance;
  DebugUtilsMessenger debugMessenger;
  Surface windowSurface;

  PhysicalDevice targetPhysicalDevice;
  VkPhysicalDeviceProperties physicalDeviceProperties;

  u32 swapchainImages;

  u32 queueFamilyIndex;
  VirtualDevice virtualDevice;
  Queue queue;

  PipelineCache pipelineCache;

  DeviceMemoryAllocator deviceAllocator;

  Fence fence;

  CommandPool shortExecutionCommandPool;
  CommandBuffer shortExecutionCommandBuffer;

  VkSurfaceFormatKHR surfaceFormat;
  VkFormat depthStencilFormat;
  VkSampleCountFlagBits samples;

  Sampler textureSampler;

  RenderPass renderPass;
  SubpassReference subpass0;

  CommandPool renderCommandPool;
};

#endif  // VULKAN_SRC_VULKAN_H
