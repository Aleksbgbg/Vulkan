#ifndef VULKAN_SRC_RENDERER_VULKAN_VULKAN_H_
#define VULKAN_SRC_RENDERER_VULKAN_VULKAN_H_

#include <vulkan/vulkan.h>

#include <unordered_map>

#include "BoundBuffer.h"
#include "IndexedVertexBuffer.h"
#include "SwapchainWithResources.h"
#include "Texture.h"
#include "VulkanSystem.h"
#include "game/Renderer.h"
#include "game/Scene.h"
#include "game/Transformable.h"
#include "general/algorithms/RandomNumberGenerator.h"
#include "general/geometry/Rect.h"
#include "renderer/vertices/StructuredVertexData.h"
#include "renderer/vulkan/api/Swapchain.h"
#include "renderer/vulkan/api/VulkanInstance.h"
#include "renderer/vulkan/api/memory/DeviceMemoryAllocator.h"
#include "renderer/vulkan/buffer_structures/GlobalRenderUniform.h"
#include "renderer/vulkan/render_graph/RenderGraph.h"
#include "system/windowing/Window.h"

typedef u32 ResourceKey;

class Vulkan : public Renderer,
               private SwapchainWithResources::Initializer,
               private RenderGraph::ResourceAllocator {
 public:
  Vulkan(const VulkanSystem& vulkanSystem, const sys::Window& window);
  ~Vulkan();

  void RecreateSwapchain();

  struct ComputeContext {
    float deltaTime;
  };
  void ScheduleCompute(const ComputeContext& context);
  void ScheduleRender(const game::Camera& camera, const sys::Window& window);

 private:
  void CalculateProjection();

  RenderPass CreateRenderPass() const;
  RenderGraph CreateRenderGraph();

  Texture LoadTexture(const std::string_view filename);
  IndexedVertexBuffer AllocateDrawBuffer(
      const StructuredVertexData::RawVertexData& vertexData);

  SwapchainCreateInfoBuilder SwapchainCreateInfo() const;

  void ReleaseResources(ResourceKey key);

  Swapchain CreateSwapchain() const override;
  Swapchain CreateSwapchain(const Swapchain& oldSwapchain) const override;
  std::vector<Framebuffer> GetFramebuffers(
      const Swapchain& swapchain,
      const std::vector<const ImageView*>& attachments) const override;
  BoundImage CreateDepthStencilAttachment(const Swapchain& swapchain) override;
  BoundImage CreateMultisamplingAttachment(const Swapchain& swapchain) override;
  Semaphore CreateSemaphore() const override;

  MeshHandle LoadMesh(const RenderType renderType,
                      const MeshLoadParams& meshLoadParams) override;
  std::unique_ptr<Resource> SpawnLightSource(
      const LightSourceInfo& lightSourceInfo) override;
  std::unique_ptr<Resource> SpawnParticleSystem(
      const ParticleSystemInfo& particleSystemInfo) override;
  std::unique_ptr<Resource> SpawnRenderable(RenderInfo renderInfo) override;

  DescriptorSetLayout CreateDescriptorSetLayout(
      const DescriptorSetLayoutCreateInfoBuilder& infoBuilder) const override;
  DescriptorSet CreateDescriptorSet(
      const DescriptorSetLayout& layout) const override;
  void UpdateDescriptorSets(
      const std::vector<DescriptorSet::WriteDescriptorSet>& descriptorSetWrites)
      const override;
  BoundBuffer AllocateHostBuffer(std::size_t size,
                                 VkBufferUsageFlags usage) override;
  BoundBuffer AllocateDeviceBuffer(std::size_t size,
                                   VkBufferUsageFlags usage) override;
  ShaderModule LoadComputeShader(std::string_view name) const override;
  ShaderModule LoadGraphicsShader(std::string_view name,
                                  VkShaderStageFlagBits stage) const override;
  ComputePipeline CreateComputePipeline(
      const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts,
      ShaderModule computeShader) const override;
  GraphicsPipeline CreateGraphicsPipeline(
      const std::vector<const DescriptorSetLayout*>& descriptorSetLayouts,
      std::vector<ShaderModule> shaders,
      const VkVertexInputBindingDescription& vertexInputBindingDescription,
      const std::vector<VkVertexInputAttributeDescription>&
          vertexInputAttributeDescriptions) const override;

  static VKAPI_ATTR VkBool32 VKAPI_CALL
  DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData);

  VkFormat SelectDepthStencilFormat(
      const std::vector<VkFormat>& potentialFormats) const;
  VkSurfaceFormatKHR SelectSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR>& availableSurfaceFormats) const;
  static VkPresentModeKHR SelectSwapPresentMode(
      const std::vector<VkPresentModeKHR>& availablePresentModes);
  VkSampleCountFlagBits SelectMsaaSamples(
      const VkSampleCountFlagBits preferred) const;

 private:
  VulkanInstance instance_;
  DebugUtilsMessenger debugMessenger_;
  Surface windowSurface_;

  PhysicalDevice targetPhysicalDevice_;
  VkPhysicalDeviceProperties physicalDeviceProperties_;

  u32 swapchainImages_;

  u32 graphicsQueueFamilyIndex_;
  u32 computeQueueFamilyIndex_;
  VirtualDevice virtualDevice_;
  Queue graphicsQueue_;
  Queue computeQueue_;

  PipelineCache pipelineCache_;

  DeviceMemoryAllocator deviceAllocator_;

  Fence fence_;

  CommandPool shortExecutionCommandPool_;
  CommandBuffer shortExecutionCommandBuffer_;

  CommandPool computeCommandPool_;
  CommandBuffer computeMainCommandBuffer_;
  CommandBuffer computeTransferCommandBuffer_;
  CommandBuffer computeCommandBuffer_;
  Fence computeCompleteFence_;

  VkSurfaceFormatKHR surfaceFormat_;
  VkFormat depthStencilFormat_;
  VkSampleCountFlagBits samples_;

  Sampler textureSampler_;

  RenderPass renderPass_;
  SubpassReference subpass0_;

  CommandPool renderCommandPool_;

  SwapchainWithResources swapchain_;
  SwapchainWithResources oldSwapchain_;

  struct SwapchainRenderPass {
    CommandBuffer main;
    CommandBuffer transfer;
    CommandBuffer graphics;

    Semaphore renderCompleteSemaphore;
    Fence submitCompleteFence;
  };
  std::vector<SwapchainRenderPass> swapchainRenderData_;

  struct VulkanMesh {
    RenderType renderType;
    IndexedVertexBuffer drawBuffer;
    std::vector<Texture> textures;
  };
  std::vector<VulkanMesh> meshes_;

  DescriptorPool descriptorPool_;

  RenderGraph renderGraph_;

  GlobalRenderUniform sceneData_;

  RandomNumberGenerator randomNumberGenerator_;

  struct PointLightSource {
    const Transformable* transform;
    LightSource::PointLightInfo info;
  };
  std::unordered_map<ResourceKey, PointLightSource> pointLights_;

  std::list<ResourceKey> lightsToDispose_;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_VULKAN_H_
