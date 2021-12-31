#ifndef VULKAN_SRC_RENDERER_VULKAN_VULKAN_H_
#define VULKAN_SRC_RENDERER_VULKAN_VULKAN_H_

#include <renderer/vertices/StructuredVertexData.h>
#include <vulkan/vulkan.h>

#include <unordered_map>

#include "BoundBuffer.h"
#include "IndexedVertexBuffer.h"
#include "SwapchainWithResources.h"
#include "Texture.h"
#include "Vulkan.h"
#include "VulkanSystem.h"
#include "game/Renderer.h"
#include "game/Scene.h"
#include "game/Transformable.h"
#include "general/algorithms/RandomNumberGenerator.h"
#include "general/geometry/Rect.h"
#include "renderer/vulkan/api/Swapchain.h"
#include "renderer/vulkan/api/VulkanInstance.h"
#include "renderer/vulkan/api/memory/DeviceMemoryAllocator.h"
#include "renderer/vulkan/buffer_structures/GlobalRenderUniform.h"
#include "system/windowing/Window.h"

// TODO: Lots of cleanup since this file has been through a lot of evolution

struct ShaderConfiguration {
  VkShaderStageFlagBits stage;
  std::string_view path;
};

class RenderConfiguration {
 public:
  virtual ~RenderConfiguration() = default;

  virtual DescriptorSetLayout ConfigureDescriptors(
      const VirtualDevice& virtualDevice) const = 0;
  virtual std::vector<ShaderConfiguration> ConfigureShaders() const = 0;
  virtual std::vector<VkVertexInputBindingDescription> ConfigureVertexBindings()
      const = 0;
  virtual std::vector<VkVertexInputAttributeDescription>
  ConfigureVertexAttributes() const = 0;
};

typedef u32 ResourceKey;

class ResourceDisposer {
 public:
  virtual ~ResourceDisposer() = default;

  virtual void DisposeResource(ResourceKey key) = 0;
};

class VulkanResource : public Resource {
 public:
  VulkanResource(ResourceDisposer* disposer, ResourceKey key);
  ~VulkanResource() override;

 private:
  ResourceDisposer* resourceDisposer_;
  ResourceKey key_;
};

class Vulkan : private SwapchainWithResources::Initializer,
               public Renderer,
               public ResourceDisposer {
 public:
  Vulkan(const VulkanSystem& vulkanSystem, const sys::Window& window);
  ~Vulkan();

  void RecreateSwapchain();
  void CalculateProjection();

  CommandBuffer AllocatePrimaryRenderCommandBuffer() const;
  Fence CreateFence(const VkFenceCreateFlags flags) const;

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

  struct ComputeContext {
    float deltaTime;
  };
  void ScheduleCompute(const ComputeContext& context);
  void ScheduleRender(const game::Camera& camera, const sys::Window& window);

  void DisposeResource(ResourceKey key) override;

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

  Texture LoadTexture(const std::string_view filename);
  BoundBuffer AllocateLocalBuffer(const std::size_t size,
                                  const VkBufferUsageFlags usage);
  BoundBuffer AllocateDeviceBuffer(const void* const data,
                                   const std::size_t size,
                                   const VkBufferUsageFlags usage);
  IndexedVertexBuffer AllocateDrawBuffer(
      const StructuredVertexData::RawVertexData& vertexData);

  SwapchainCreateInfoBuilder SwapchainCreateInfo() const;

  void ReleaseResources(ResourceKey key);

 private:
  VulkanInstance instance;
  DebugUtilsMessenger debugMessenger;
  Surface windowSurface;

  PhysicalDevice targetPhysicalDevice;
  VkPhysicalDeviceProperties physicalDeviceProperties;

  u32 swapchainImages;

  u32 queueFamilyIndex;
  u32 computeQueueFamilyIndex_;
  VirtualDevice virtualDevice;
  Queue queue;
  Queue computeQueue_;

  PipelineCache pipelineCache;

  DeviceMemoryAllocator deviceAllocator;

  Fence fence;

  CommandPool shortExecutionCommandPool;
  CommandBuffer shortExecutionCommandBuffer;

  CommandPool computeCommandPool_;
  CommandBuffer computeCommandBuffer_;
  std::vector<CommandBuffer> computeCommandsBuffers_;

  VkSurfaceFormatKHR surfaceFormat;
  VkFormat depthStencilFormat;
  VkSampleCountFlagBits samples;

  Sampler textureSampler;

  RenderPass renderPass;
  SubpassReference subpass0;

  CommandPool renderCommandPool;

  SwapchainWithResources swapchain;
  SwapchainWithResources oldSwapchain;

  u32 imageIndex;

  struct SwapchainRenderPass {
    CommandBuffer commandBuffer;

    Semaphore renderCompleteSemaphore;
    Fence submitCompleteFence;
  };
  std::vector<SwapchainRenderPass> swapchainRenderData;

  struct VulkanMesh {
    RenderType renderType;
    IndexedVertexBuffer drawBuffer;
    std::vector<Texture> textures;
  };
  std::vector<VulkanMesh> meshes_;

  DescriptorPool descriptorPool_;
  DescriptorSetLayout sceneDescriptorSetLayout_;
  DescriptorSet sceneDescriptorSet_;
  GlobalRenderUniform sceneData_;
  BoundBuffer sceneUniformBuffer_;

  struct Draw {
    DescriptorSet descriptorSet;
    const Transformable* transformable;
    const IndexedVertexBuffer* drawBuffer;
    u32 instances;
  };

  struct Render {
    DescriptorSetLayout descriptorSetLayout;
    GraphicsPipeline pipeline;

    std::unordered_map<ResourceKey, Draw> draws;
  };

  std::unordered_map<RenderType, Render> renders_;

  DescriptorSetLayout computeDescriptorSetLayout_;
  DescriptorSet computeDescriptorSet_;

  BoundBuffer computeRootBuffer_;

  struct ComputeInstance {
    DescriptorSet descriptorSet;
    const Transformable* base;
    const SpawnControllable* spawn;
    glm::vec3 spawnRegionLow;
    glm::vec3 spawnRegionHigh;
    BoundBuffer particleSpawnParamsBuffer;
    BoundBuffer particleBuffer;
    BoundBuffer particleRenderBuffer;
  };

  struct Compute {
    DescriptorSetLayout descriptorSetLayout;
    ComputePipeline pipeline;

    std::unordered_map<ResourceKey, ComputeInstance> instances;
  };
  std::unordered_map<ParticleBehaviour, Compute> particleComputes_;

  RandomNumberGenerator randomNumberGenerator_;

  struct PointLightSource {
    const Transformable* transform;
    LightSource::PointLightInfo info;
  };
  std::unordered_map<ResourceKey, PointLightSource> pointLights_;

  std::list<ResourceKey> resourcesToDispose_;

 private:
  Render CreateRender(const RenderConfiguration& configuration) const;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_VULKAN_H_
