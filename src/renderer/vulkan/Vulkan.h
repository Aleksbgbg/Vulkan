#ifndef VULKAN_SRC_RENDERER_VULKAN_VULKAN_H_
#define VULKAN_SRC_RENDERER_VULKAN_VULKAN_H_

#include <vulkan/vulkan.h>

#include <unordered_map>

#include "BoundBuffer.h"
#include "IndexedVertexBuffer.h"
#include "SwapchainWithResources.h"
#include "Texture.h"
#include "VulkanSystem.h"
#include "game/GraphicsSettingsConfigurator.h"
#include "game/Renderer.h"
#include "game/Scene.h"
#include "game/Transformable.h"
#include "game/actor/resource/ResourceList.h"
#include "general/algorithms/RandomNumberGenerator.h"
#include "general/images/Bitmap.h"
#include "general/math/Rect.h"
#include "general/text/FontAtlas.h"
#include "renderer/vertices/StructuredVertexData.h"
#include "renderer/vulkan/api/Swapchain.h"
#include "renderer/vulkan/api/VulkanInstance.h"
#include "renderer/vulkan/api/memory/DeviceMemoryAllocator.h"
#include "renderer/vulkan/buffer_structures/GlobalRenderUniform.h"
#include "renderer/vulkan/render_graph/RenderGraph.h"
#include "system/windowing/Window.h"

class Vulkan : public Renderer,
               public GraphicsSettingsConfigurator,
               private SwapchainWithResources::Initializer,
               private RenderGraph::ResourceAllocator {
 public:
  Vulkan(const VulkanSystem& vulkanSystem, const sys::Window& window,
         const FontAtlas& fontAtlas, const Settings& settings);
  ~Vulkan();

  void RecreateSwapchain();

  struct ComputeContext {
    float deltaTime;
  };
  void ScheduleCompute(const ComputeContext& context);
  void ScheduleRender(const game::Camera& camera, const sys::Window& window);

 private:
  void CalculateProjection();

  vk::RenderPass CreateRenderPass() const;
  RenderGraph CreateRenderGraph();

  Texture LoadTexture(std::string_view filename);
  Texture LoadTexture(const Bitmap& image,
                      VkFormat format = VK_FORMAT_R8G8B8A8_SRGB);
  IndexedVertexBuffer AllocateDrawBuffer(
      const StructuredVertexData::RawVertexData& vertexData);

  SwapchainCreateInfoBuilder SwapchainCreateInfo() const;

  MeshHandle LoadMesh(const RenderType renderType,
                      const MeshLoadParams& meshLoadParams) override;
  std::unique_ptr<Resource> SpawnLightSource(
      const LightSourceInfo& lightSourceInfo) override;
  std::unique_ptr<Resource> SpawnParticleSystem(
      const ParticleSystemInfo& particleSystemInfo) override;
  std::unique_ptr<Resource> SpawnRenderable(
      const RenderInfo& renderInfo) override;
  std::unique_ptr<Resource> SpawnUi(const UiInfo& uiInfo) override;

  void SetMsaaIndex(u32 index) override;

  vk::Swapchain CreateSwapchain() const override;
  vk::Swapchain CreateSwapchain(
      const vk::Swapchain& oldSwapchain) const override;
  std::vector<vk::Framebuffer> GetFramebuffers(
      const vk::Swapchain& swapchain,
      const std::vector<const vk::ImageView*>& attachments) const override;
  BoundImage CreateDepthStencilAttachment(
      const vk::Swapchain& swapchain) override;
  BoundImage CreateMultisamplingAttachment(
      const vk::Swapchain& swapchain) override;
  vk::Semaphore CreateSemaphore() const override;

  vk::DescriptorSetLayout CreateDescriptorSetLayout(
      const DescriptorSetLayoutCreateInfoBuilder& infoBuilder) const override;
  vk::DescriptorSet CreateDescriptorSet(
      const vk::DescriptorSetLayout& layout) const override;
  void UpdateDescriptorSets(
      const std::vector<vk::DescriptorSet::WriteDescriptorSet>&
          descriptorSetWrites) const override;
  BoundBuffer AllocateHostBuffer(std::size_t size,
                                 VkBufferUsageFlags usage) override;
  BoundBuffer AllocateDeviceBuffer(std::size_t size,
                                   VkBufferUsageFlags usage) override;
  vk::ShaderModule LoadComputeShader(std::string_view name) const override;
  vk::ShaderModule LoadGraphicsShader(
      std::string_view name, VkShaderStageFlagBits stage) const override;
  vk::ComputePipeline CreateComputePipeline(
      const std::vector<const vk::DescriptorSetLayout*>& descriptorSetLayouts,
      vk::ShaderModule computeShader) const override;
  vk::GraphicsPipeline CreateGraphicsPipeline(
      const std::vector<const vk::DescriptorSetLayout*>& descriptorSetLayouts,
      std::vector<vk::ShaderModule> shaders,
      const VkVertexInputBindingDescription& vertexInputBindingDescription,
      const std::vector<VkVertexInputAttributeDescription>&
          vertexInputAttributeDescriptions,
      GraphicsPipelineCreateInfoBuilder infoBuilder) const override;

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
  VkSampleCountFlagBits SelectMsaaSamples(const Settings& settings) const;

 private:
  vk::VulkanInstance instance_;
  vk::DebugUtilsMessenger debugMessenger_;
  vk::Surface windowSurface_;

  vk::PhysicalDevice targetPhysicalDevice_;
  VkPhysicalDeviceProperties physicalDeviceProperties_;

  u32 swapchainImages_;

  u32 graphicsQueueFamilyIndex_;
  u32 computeQueueFamilyIndex_;
  vk::VirtualDevice virtualDevice_;
  vk::Queue graphicsQueue_;
  vk::Queue computeQueue_;

  vk::PipelineCache pipelineCache_;

  vk::DeviceMemoryAllocator deviceAllocator_;

  vk::Fence fence_;

  vk::CommandPool shortExecutionCommandPool_;
  vk::CommandBuffer shortExecutionCommandBuffer_;

  vk::CommandPool computeCommandPool_;
  vk::CommandBuffer computeMainCommandBuffer_;
  vk::CommandBuffer computeTransferCommandBuffer_;
  vk::CommandBuffer computeCommandBuffer_;
  vk::Fence computeCompleteFence_;

  VkSurfaceFormatKHR surfaceFormat_;
  VkFormat depthStencilFormat_;
  VkSampleCountFlagBits samples_;

  vk::Sampler textureSampler_;
  vk::Sampler fontSampler_;

  vk::RenderPass renderPass_;
  vk::SubpassReference subpass0_;

  vk::CommandPool renderCommandPool_;

  SwapchainWithResources swapchain_;
  SwapchainWithResources oldSwapchain_;

  struct SwapchainRenderPass {
    vk::CommandBuffer main;
    vk::CommandBuffer transfer;
    vk::CommandBuffer graphics;

    vk::Semaphore renderCompleteSemaphore;
    vk::Fence submitCompleteFence;
  };
  std::vector<SwapchainRenderPass> swapchainRenderData_;

  struct VulkanMesh {
    RenderType renderType;
    IndexedVertexBuffer drawBuffer;
    std::vector<Texture> textures;
  };
  std::vector<VulkanMesh> meshes_;

  vk::DescriptorPool descriptorPool_;

  RenderGraph renderGraph_;

  GlobalRenderUniform sceneData_;

  RandomNumberGenerator randomNumberGenerator_;

  struct PointLightSource {
    LightSource::PointLightInfo info;
    const Transformable& transform;
  };
  ResourceList<PointLightSource> pointLights_;

  std::vector<Texture> uiTexture_;

  struct UiRender {
    const Visible& visible;
    DrawList* drawList;
    std::unique_ptr<IndexedVertexBuffer> elementsBuffer;
    std::unique_ptr<IndexedVertexBuffer> textBuffer;
  };
  ResourceList<UiRender> uiRenders_;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_VULKAN_H_
