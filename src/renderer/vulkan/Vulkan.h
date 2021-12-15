#ifndef VULKAN_SRC_RENDERER_VULKAN_VULKAN_H_
#define VULKAN_SRC_RENDERER_VULKAN_VULKAN_H_

#include <vulkan/vulkan.h>

#include <unordered_map>

#include "DynamicUniformBuffer.h"
#include "SwapchainWithResources.h"
#include "Texture.h"
#include "ViewTransform.h"
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

class Vulkan : public SwapchainWithResources::Initializer,
               public DynamicUniformBufferInitializer,
               public Renderer {
 public:
  Vulkan(const VulkanSystem& vulkanSystem, const sys::Window& window);
  ~Vulkan();

  void WindowResized();

  CommandBuffer AllocatePrimaryRenderCommandBuffer() const;
  Fence CreateFence(const VkFenceCreateFlags flags) const;

  Texture LoadTexture(const std::string_view filename);
  BufferWithMemory AllocateLocalBuffer(const std::size_t size,
                                       const VkBufferUsageFlags usage);
  BufferWithMemory AllocateDeviceBuffer(const void* const data,
                                        const std::size_t size,
                                        const VkBufferUsageFlags usage);

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

  DescriptorSet::WriteDescriptorSet CreateImageSamplerWrite(
      const DescriptorSet& descriptorSet, const ImageView& imageView,
      const u32 binding) const;

  MeshHandle LoadMesh(const RenderType renderType,
                      const MeshLoadParams& meshLoadParams) override;
  void SpawnParticleSystem(
      const ParticleSystemInfo& particleSystemInfo) override;
  void SpawnRenderable(RenderInfo renderInfo) override;

  struct ComputeContext {
    float deltaTime;
  };
  void ScheduleCompute(const ComputeContext& context);
  void ScheduleRender(const game::Camera& camera, const sys::Window& window);

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
    BufferWithMemory vertexBuffer;
    BufferWithMemory indexBuffer;
    u32 indexCount;
    std::vector<Texture> textures;
  };
  std::vector<VulkanMesh> meshes_;

  DescriptorPool descriptorPool_;
  DescriptorSetLayout sceneDescriptorSetLayout_;
  DescriptorSet sceneDescriptorSet_;
  DynamicUniformBuffer<PerFrameData> sceneUniformBuffer_;

  struct Instance {
    DescriptorSet descriptorSet;
    const Transformable* transformable;
    const Buffer* vertexBuffer;
    const Buffer* indexBuffer;
    u32 indexCount;
  };

  struct IndirectDraw {
    DescriptorSet descriptorSet;
    const Transformable* transformable;
    const Buffer* vertexBuffer;
    const Buffer* indexBuffer;
    BufferWithMemory drawBuffer;
  };

  struct Render {
    DescriptorSetLayout descriptorSetLayout;
    Pipeline pipeline;

    std::vector<Instance> instances;
    std::vector<IndirectDraw> indirectDraws;
  };

  std::unordered_map<RenderType, Render> renders_;

  DescriptorSetLayout computeDescriptorSetLayout_;
  DescriptorSet computeDescriptorSet_;

  BufferWithMemory computeRootBuffer_;

  struct SceneUniform {
    alignas(4) float deltaTime;
  };

  struct ParticleSpawnParams {
    alignas(4) u32 randomSeed;
    alignas(4) u32 enableRespawn;
    alignas(16) glm::mat4 baseTransform;
    alignas(16) glm::vec3 spawnRegionLow;
    alignas(16) glm::vec3 spawnRegionHigh;
  };

  struct ParticleRender {
    alignas(16) glm::mat4 transform;
    alignas(4) float fractionOfLife;
  };

  struct Particle {
    alignas(16) glm::mat4 baseTransform;
    alignas(16) glm::vec3 position;
    alignas(4) float velocity;
    alignas(4) float totalLife;
    alignas(4) float timeToLive;
    alignas(4) u32 isRendering;
  };

  struct ComputeInstance {
    DescriptorSet descriptorSet;
    const Transformable* base;
    const SpawnControllable* spawn;
    glm::vec3 spawnRegionLow;
    glm::vec3 spawnRegionHigh;
    BufferWithMemory particleSpawnParamsBuffer;
    BufferWithMemory particleBuffer;
    BufferWithMemory particleRenderBuffer;
  };

  struct Compute {
    DescriptorSetLayout descriptorSetLayout;
    ComputePipeline pipeline;

    std::vector<ComputeInstance> instances;
  };
  std::unordered_map<ParticleBehaviour, Compute> particleComputes_;

  RandomNumberGenerator randomNumberGenerator_;

 private:
  Render CreateRender(const RenderConfiguration& configuration) const;
};

#endif  // VULKAN_SRC_RENDERER_VULKAN_VULKAN_H_
