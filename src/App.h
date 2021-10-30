#ifndef VULKAN_SRC_APP_H
#define VULKAN_SRC_APP_H

#include <game/RenderPipeline.h>
#include <vulkan/vulkan.h>

#include <chrono>
#include <vector>

#include "DynamicUniformBuffer.h"
#include "UiRenderer.h"
#include "game/Npc.h"
#include "game/Player.h"
#include "game/SpaceshipModel.h"
#include "game/rendering/BufferWithMemory.h"
#include "game/rendering/ImageWithMemory.h"
#include "general/animations/NormalizedOneTimeFunctionAnimation.h"
#include "general/animations/UpAndDownGlidingAnimation.h"
#include "general/geometry/Rect.h"
#include "general/threading/MultithreadedMessageQueue.h"
#include "general/windowing/Window.h"
#include "general/windowing/input/Keyboard.h"
#include "general/windowing/input/Mouse.h"
#include "memory/DeviceMemoryAllocator.h"
#include "util/include/glm.h"
#include "vulkan/ImGuiInstance.h"
#include "vulkan/Swapchain.h"
#include "vulkan/VulkanInstance.h"

class App {
 public:
  App();
  ~App();

  int Run();

 private:
  void MainThread();
  void RenderThread();

  void MainLoop();

  void UpdateModel(const float deltaTime);
  void Render();

  void InitializeSwapchain();

 private:
  struct PerSceneData {
    alignas(16) glm::mat4 projection;
  };

  struct Material {
    alignas(16) glm::vec3 ambient;
    alignas(16) glm::vec3 diffuse;
    alignas(16) glm::vec3 specular;
    alignas(4) float shininess;
  };

  struct Light {
    alignas(16) glm::vec3 position;

    alignas(16) glm::vec3 ambient;
    alignas(16) glm::vec3 diffuse;
    alignas(16) glm::vec3 specular;
  };

  struct PerFrameData {
    alignas(16) glm::mat4 view;
    alignas(16) glm::vec3 cameraPosition;
    alignas(16) glm::vec3 lightingPosition;
    Material material;
    Light light;
  };

  struct PerObjectData {
    alignas(16) glm::mat4 model;
  };

  struct LightingData {
    alignas(16) glm::vec3 position;
  };

  BufferWithMemory TransferDataToGpuLocalMemory(CommandBuffer& commandBuffer,
                                                const void* data,
                                                const u32 size,
                                                const VkBufferUsageFlags usage);

  VkFormat SelectDepthStencilFormat(
      const std::vector<VkFormat>& potentialFormats) const;

  VkSurfaceFormatKHR SelectSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR>& availableSurfaceFormats) const;
  static VkExtent2D SelectSwapExtent(
      const VkSurfaceCapabilitiesKHR surfaceCapabilities);
  static VkPresentModeKHR SelectSwapPresentMode(
      const std::vector<VkPresentModeKHR>& availablePresentModes);
  VkSampleCountFlagBits SelectMsaaSamples(
      const VkSampleCountFlagBits preferred);

  static VKAPI_ATTR VkBool32 VKAPI_CALL
  DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData);

  Window window;
  const Keyboard& keyboard;
  const Mouse& mouse;

  VulkanInstance instance;
  DebugUtilsMessenger debugMessenger;
  Surface windowSurface;
  u32 minSwapchainImages;
  u32 framesInFlight;

  PhysicalDevice targetPhysicalDevice;
  VkPhysicalDeviceProperties physicalDeviceProperties;

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

  RenderPipeline spaceshipRenderPipeline;

  Player player;
  Npc npc;

  Sampler textureSampler;

  RenderPass renderPass;

  CommandPool renderCommandPool;
  std::vector<ShaderModule> shaders;

  bool hasSwapchain;
  Swapchain swapchain;
  bool hasOldSwapchain;
  Swapchain oldSwapchain;

  ImageWithMemory depthStencil;
  ImageView depthStencilView;

  ImageWithMemory multisamplingImage;
  ImageView multisamplingImageView;

  std::vector<Framebuffer> swapchainFramebuffers;

  DescriptorSetLayout perSceneDescriptorSetLayout;
  DescriptorSetLayout perFrameDescriptorSetLayout;

  DescriptorSetLayout lightingDescriptorSetLayout;

  DescriptorSetLayout playerTextureSamplerDescriptorSetLayout;
  DescriptorSetLayout npcTextureSamplerDescriptorSetLayout;

  Pipeline pipeline;

  DescriptorPool descriptorPool;
  DescriptorSet sceneDescriptorSet;

  DescriptorSet lightingDescriptorSet;

  DescriptorSet playerTextureSamplerDescriptorSet;
  DescriptorSet npcTextureSamplerDescriptorSet;

  DynamicUniformBuffer<PerFrameData> viewTransformBuffer;

  std::unique_ptr<UiRenderer> uiRenderer;

  BufferWithMemory projectionTransformBuffer;
  PerSceneData projectionTransform;

  BufferWithMemory lightingBuffer;
  LightingData lighting;

  struct SwapchainRenderPass {
    CommandBuffer commandBuffer;

    MeshRenderer meshRenderer;

    Semaphore renderCompleteSemaphore;
    Fence submitCompleteFence;
  };
  std::vector<SwapchainRenderPass> swapchainRenderData;
  struct InFlightImage {
    Semaphore acquireImageSemaphore;
  };
  std::vector<InFlightImage> imagesInFlightSynchronisation;
  u32 currentInFlightImage = 0;

  enum class EventNotification { Exited, Paused, Unpaused, Resized };
  MultithreadedMessageQueue<EventNotification> threadMessenger;

  std::chrono::time_point<std::chrono::steady_clock> previousTime;

  float totalTime;
};

#endif  // VULKAN_SRC_APP_H
