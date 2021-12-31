#include "Vulkan.h"

#include <array>
#include <filesystem>
#include <stdexcept>

#include "general/adapters/MapValueIterator.h"
#include "general/files/file.h"
#include "general/files/images/png.h"
#include "general/logging/log.h"
#include "general/math/math.h"
#include "memory/Alignment.h"
#include "renderer/vertices/PositionNormalTextureVertex.h"
#include "renderer/vertices/PositionTextureVertex.h"
#include "renderer/vertices/PositionVertex.h"
#include "renderer/vulkan/api/Buffer.h"
#include "renderer/vulkan/api/DescriptorPool.h"
#include "renderer/vulkan/api/VulkanInstance.h"
#include "renderer/vulkan/api/structures/AttachmentReference.h"
#include "renderer/vulkan/api/structures/ClearValue.h"
#include "renderer/vulkan/api/structures/DescriptorSetLayoutBinding.h"
#include "renderer/vulkan/api/structures/DeviceQueueCreateInfo.h"
#include "renderer/vulkan/api/structures/PushConstantRange.h"
#include "renderer/vulkan/api/structures/SubpassDescription.h"
#include "renderer/vulkan/api/structures/default.h"
#include "renderer/vulkan/api/util.h"
#include "renderer/vulkan/buffer_structures/GlobalComputeUniform.h"
#include "renderer/vulkan/buffer_structures/ModelTransform.h"
#include "renderer/vulkan/buffer_structures/Particle.h"
#include "renderer/vulkan/buffer_structures/ParticleRender.h"
#include "renderer/vulkan/buffer_structures/ParticleSpawnParams.h"
#include "util/build_definition.h"
#include "util/filenames.h"

static constexpr u32 WANTED_SWAPCHAIN_IMAGES = 3u;

VulkanResource::VulkanResource(ResourceDisposer* const disposer,
                               const ResourceKey key)
    : resourceDisposer_(disposer), key_(key) {}

VulkanResource::~VulkanResource() {
  resourceDisposer_->DisposeResource(key_);
}

ResourceKey GenerateResourceKey() {
  static u32 currentKey = 0;
  return ++currentKey;
}

VulkanInstance CreateVulkanInstance(const VulkanSystem& vulkanSystem) {
  const std::vector<VkExtensionProperties> availableExtensions =
      LoadArray(VulkanInstance::LoadInstanceExtensionProperties);

  const std::vector<const char*> requiredExtensions = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    vulkanSystem.SurfaceExtensionName(),
#if defined(VALIDATION)
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
  };

  if (!RequiredValuesAreAvailable(availableExtensions, requiredExtensions,
                                  [](const VkExtensionProperties& property) {
                                    return property.extensionName;
                                  })) {
    throw std::runtime_error("Required extensions not available.");
  }

  const std::vector<VkLayerProperties> availableLayers =
      LoadArray(VulkanInstance::LoadInstanceLayerProperties);
  const std::vector<const char*> requiredLayers = {
#ifdef VALIDATION
      "VK_LAYER_KHRONOS_validation",
#endif
  };

  if (!RequiredValuesAreAvailable(availableLayers, requiredLayers,
                                  [](const VkLayerProperties& property) {
                                    return property.layerName;
                                  })) {
    throw std::runtime_error("Required layers not available.");
  }

  return VulkanInstance(
      InstanceCreateInfoBuilder()
          .SetApplicationInfo(ApplicationInfoBuilder()
                                  .SetPApplicationName("Application")
                                  .SetApplicationVersion(1))
          .SetEnabledExtensionCount(requiredExtensions.size())
          .SetPpEnabledExtensionNames(requiredExtensions.data())
          .SetEnabledLayerCount(requiredLayers.size())
          .SetPpEnabledLayerNames(requiredLayers.data()));
}

PhysicalDevice ChoosePhysicalDevice(const VulkanInstance& instance) {
  std::vector<PhysicalDevice> physicalDevices = instance.GetPhysicalDevices();

  if (physicalDevices.empty()) {
    throw std::runtime_error("No Vulkan-enabled GPUs found on the machine.");
  }

  BufferedLog(std::string("Found ") + std::to_string(physicalDevices.size()) +
              " GPU(s):");
  for (int index = 0; index < physicalDevices.size(); ++index) {
    BufferedLog(std::string("[") + std::to_string(index) + "] " +
                physicalDevices[index].GetProperties().deviceName);
  }
  ImmediateLog("Selecting GPU 0 as render target.");

  return physicalDevices[0];
}

u32 CalculateSwapchainImages(
    const VkSurfaceCapabilitiesKHR& surfaceCapabilities) {
  const u32 maxImageCount = surfaceCapabilities.maxImageCount == 0
                                ? WANTED_SWAPCHAIN_IMAGES
                                : surfaceCapabilities.maxImageCount;

  return CoerceToRange(WANTED_SWAPCHAIN_IMAGES,
                       surfaceCapabilities.minImageCount, maxImageCount);
}

u32 ChooseQueueFamily(const PhysicalDevice& physicalDevice,
                      const Surface& windowSurface) {
  const std::optional<u32> queueFamilyIndex =
      physicalDevice.FindAppropriateQueueFamily(
          VK_QUEUE_GRAPHICS_BIT, [&](const u32 queueFamilyIndex) {
            return windowSurface.IsSupportedByPhysicalDevice(physicalDevice,
                                                             queueFamilyIndex);
          });

  if (!queueFamilyIndex.has_value()) {
    throw std::runtime_error(
        "Could not find device queue family with required features.");
  }

  return queueFamilyIndex.value();
}

u32 ChooseComputeFamily(const PhysicalDevice& physicalDevice) {
  const std::optional<u32> queueFamilyIndex =
      physicalDevice.FindAppropriateQueueFamily(VK_QUEUE_TRANSFER_BIT |
                                                VK_QUEUE_COMPUTE_BIT);

  if (!queueFamilyIndex.has_value()) {
    throw std::runtime_error("Could not find compute queue.");
  }

  return queueFamilyIndex.value();
}

VirtualDevice CreateVirtualDevice(const PhysicalDevice& physicalDevice,
                                  const u32 graphicsQueueFamily,
                                  const u32 computeQueueFamily) {
  const std::vector<const char*> deviceExtensions{
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  constexpr float queuePriorities[] = {1.0f};

  std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;

  if (graphicsQueueFamily == computeQueueFamily) {
    deviceQueueCreateInfos = {DeviceQueueCreateInfoBuilder()
                                  .SetQueueCount(1)
                                  .SetQueueFamilyIndex(graphicsQueueFamily)
                                  .SetPQueuePriorities(queuePriorities)};
  } else {
    deviceQueueCreateInfos = {
        DeviceQueueCreateInfoBuilder()
            .SetQueueCount(1)
            .SetQueueFamilyIndex(graphicsQueueFamily)
            .SetPQueuePriorities(queuePriorities),
        DeviceQueueCreateInfoBuilder()
            .SetQueueCount(1)
            .SetQueueFamilyIndex(computeQueueFamily)
            .SetPQueuePriorities(queuePriorities),
    };
  }

  return physicalDevice.CreateVirtualDevice(
      DeviceCreateInfoBuilder()
          .SetQueueCreateInfoCount(deviceQueueCreateInfos.size())
          .SetPQueueCreateInfos(deviceQueueCreateInfos.data())
          .SetPEnabledFeatures(
              PhysicalDeviceFeaturesBuilder().SetSamplerAnisotropy(VK_TRUE))
          .SetEnabledExtensionCount(deviceExtensions.size())
          .SetPpEnabledExtensionNames(deviceExtensions.data()));
}

PipelineCache LoadOrCreatePipelineCache(const VirtualDevice& virtualDevice) {
  if (std::filesystem::exists(PIPELINE_CACHE_FILENAME)) {
    return virtualDevice.LoadPipelineCache(
        file::ReadFile(PIPELINE_CACHE_FILENAME));
  }

  return virtualDevice.CreatePipelineCache();
}

VkSurfaceFormatKHR Vulkan::SelectSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats) const {
  for (const auto& format : availableFormats) {
    if ((format.format == VK_FORMAT_B8G8R8A8_SRGB) &&
        (format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)) {
      BufferedLog("Selecting sRGB 32-bit BGRA swapchain format.");
      return format;
    }
  }

  BufferedLog(
      "Selecting first available swapchain format (could not find sRGB).");
  return availableFormats[0];
}

VkExtent2D Vulkan::SelectSwapExtent(
    const VkSurfaceCapabilitiesKHR surfaceCapabilities) {
  // TODO: Check the framebuffer size if the currentExtent is a special value
  // UINT32_MAX meaning the picture can be a different size than the window
  return surfaceCapabilities.currentExtent;
}

VkPresentModeKHR Vulkan::SelectSwapPresentMode(
    const std::vector<VkPresentModeKHR>& availablePresentModes) {
  BufferedLog("Selecting vertically synced swapchain present mode.");
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkFormat Vulkan::SelectDepthStencilFormat(
    const std::vector<VkFormat>& potentialFormats) const {
  for (const VkFormat format : potentialFormats) {
    const VkFormatProperties formatProperties =
        targetPhysicalDevice.GetFormatProperties(format);

    if (formatProperties.optimalTilingFeatures &
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
      return format;
    }
  }

  throw std::runtime_error("Could not find suitable depth stencil format.");
}

VkSampleCountFlagBits Vulkan::SelectMsaaSamples(
    const VkSampleCountFlagBits preferred) const {
  const VkSampleCountFlags supportedSamples =
      physicalDeviceProperties.limits.framebufferColorSampleCounts &
      physicalDeviceProperties.limits.framebufferDepthSampleCounts;

  for (VkSampleCountFlagBits samples = preferred;
       samples > VK_SAMPLE_COUNT_1_BIT;
       samples = static_cast<VkSampleCountFlagBits>(samples >> 1)) {
    if ((supportedSamples & samples) == samples) {
      return samples;
    }
  }

  return VK_SAMPLE_COUNT_1_BIT;
}

RenderPass Vulkan::CreateRenderPass() const {
  const VkFormat swapchainImageFormat = surfaceFormat.format;
  const std::array<VkAttachmentDescription, 3> attachmentDescriptions{
      AttachmentDescriptionBuilder()
          .SetFormat(swapchainImageFormat)
          .SetSamples(VK_SAMPLE_COUNT_1_BIT)
          .SetLoadOp(VK_ATTACHMENT_LOAD_OP_CLEAR)
          .SetStoreOp(VK_ATTACHMENT_STORE_OP_STORE)
          .SetStencilLoadOp(VK_ATTACHMENT_LOAD_OP_DONT_CARE)
          .SetStencilStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetFinalLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR),
      AttachmentDescriptionBuilder()
          .SetFormat(swapchainImageFormat)
          .SetSamples(samples)
          .SetLoadOp(VK_ATTACHMENT_LOAD_OP_CLEAR)
          .SetStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetStencilLoadOp(VK_ATTACHMENT_LOAD_OP_DONT_CARE)
          .SetStencilStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetFinalLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL),
      AttachmentDescriptionBuilder()
          .SetFormat(depthStencilFormat)
          .SetSamples(samples)
          .SetLoadOp(VK_ATTACHMENT_LOAD_OP_CLEAR)
          .SetStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetStencilLoadOp(VK_ATTACHMENT_LOAD_OP_DONT_CARE)
          .SetStencilStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetFinalLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL),
  };
  constexpr const VkAttachmentReference resolveAttachment =
      AttachmentReferenceBuilder().SetAttachment(0).SetLayout(
          VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
  constexpr const VkAttachmentReference colorAttachment =
      AttachmentReferenceBuilder().SetAttachment(1).SetLayout(
          VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
  constexpr const VkAttachmentReference depthStencilAttachment =
      AttachmentReferenceBuilder().SetAttachment(2).SetLayout(
          VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
  const std::array<VkSubpassDescription, 1> subpasses{
      SubpassDescriptionBuilder()
          .SetPipelineBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS)
          .SetPResolveAttachments(&resolveAttachment)
          .SetColorAttachmentCount(1)
          .SetPColorAttachments(&colorAttachment)
          .SetPDepthStencilAttachment(&depthStencilAttachment),
  };

  return virtualDevice.CreateRenderPass(
      RenderPassCreateInfoBuilder()
          .SetAttachmentCount(attachmentDescriptions.size())
          .SetPAttachments(attachmentDescriptions.data())
          .SetSubpassCount(subpasses.size())
          .SetPSubpasses(subpasses.data()));
}

DescriptorPool MakeDescriptorPool(const VirtualDevice& virtualDevice) {
  constexpr std::array<VkDescriptorPoolSize, 3> descriptorPoolSizes{
      DescriptorPoolSizeBuilder()
          .SetType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
          .SetDescriptorCount(1000),
      DescriptorPoolSizeBuilder()
          .SetType(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
          .SetDescriptorCount(1000),
      DescriptorPoolSizeBuilder()
          .SetType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
          .SetDescriptorCount(1000),
  };
  return virtualDevice.CreateDescriptorPool(
      DescriptorPoolCreateInfoBuilder()
          .SetFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
          .SetPoolSizeCount(descriptorPoolSizes.size())
          .SetPPoolSizes(descriptorPoolSizes.data())
          .SetMaxSets(1000));
}

Vulkan::Render Vulkan::CreateRender(
    const RenderConfiguration& configuration) const {
  Render render;
  render.descriptorSetLayout =
      configuration.ConfigureDescriptors(virtualDevice);
  std::vector<ShaderModule> shaders;
  for (const ShaderConfiguration& shader : configuration.ConfigureShaders()) {
    shaders.push_back(
        std::move(virtualDevice.LoadShader(shader.stage, shader.path)));
  }
  const std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions =
      configuration.ConfigureVertexBindings();
  const std::vector<VkVertexInputAttributeDescription>
      vertexAttributeDescriptions = configuration.ConfigureVertexAttributes();
  constexpr std::array<VkDynamicState, 2> dynamicStates = {
      VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
  render.pipeline = virtualDevice.CreateGraphicsPipeline(
      pipelineCache, shaders,
      virtualDevice.CreatePipelineLayout(
          {&sceneDescriptorSetLayout_, &render.descriptorSetLayout},
          PipelineLayoutCreateInfoBuilder()
              .SetPushConstantRangeCount(1)
              .SetPPushConstantRanges(
                  PushConstantRangeBuilder()
                      .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT)
                      .SetOffset(0)
                      .SetSize(sizeof(ModelTransform)))),
      subpass0,
      GraphicsPipelineCreateInfoBuilder()
          .SetPDepthStencilState(PipelineDepthStencilStateCreateInfoBuilder()
                                     .SetDepthTestEnable(VK_TRUE)
                                     .SetDepthWriteEnable(VK_TRUE)
                                     .SetDepthCompareOp(VK_COMPARE_OP_LESS)
                                     .SetDepthBoundsTestEnable(VK_FALSE))
          .SetPVertexInputState(PipelineVertexInputStateCreateInfoBuilder()
                                    .SetVertexBindingDescriptionCount(
                                        vertexBindingDescriptions.size())
                                    .SetPVertexBindingDescriptions(
                                        vertexBindingDescriptions.data())
                                    .SetVertexAttributeDescriptionCount(
                                        vertexAttributeDescriptions.size())
                                    .SetPVertexAttributeDescriptions(
                                        vertexAttributeDescriptions.data()))
          .SetPInputAssemblyState(
              PipelineInputAssemblyStateCreateInfoBuilder().SetTopology(
                  VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST))
          .SetPViewportState(PipelineViewportStateCreateInfoBuilder()
                                 .SetViewportCount(1)
                                 .SetScissorCount(1))
          .SetPDynamicState(PipelineDynamicStateCreateInfoBuilder()
                                .SetDynamicStateCount(dynamicStates.size())
                                .SetPDynamicStates(dynamicStates.data()))
          .SetPRasterizationState(
              PipelineRasterizationStateCreateInfoBuilder()
                  .SetCullMode(VK_CULL_MODE_BACK_BIT)
                  .SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE)
                  .SetPolygonMode(VK_POLYGON_MODE_FILL)
                  .SetLineWidth(1.0f))
          .SetPMultisampleState(PipelineMultisampleStateCreateInfoBuilder()
                                    .SetRasterizationSamples(samples)
                                    .SetMinSampleShading(1.0f))
          .SetPColorBlendState(
              PipelineColorBlendStateCreateInfoBuilder()
                  .SetAttachmentCount(1)
                  .SetPAttachments(
                      PipelineColorBlendAttachmentStateBuilder()
                          .SetColorWriteMask(VK_COLOR_COMPONENT_R_BIT |
                                             VK_COLOR_COMPONENT_G_BIT |
                                             VK_COLOR_COMPONENT_B_BIT |
                                             VK_COLOR_COMPONENT_A_BIT)
                          .SetSrcColorBlendFactor(VK_BLEND_FACTOR_ONE)
                          .SetDstColorBlendFactor(VK_BLEND_FACTOR_ZERO)
                          .SetColorBlendOp(VK_BLEND_OP_ADD)
                          .SetSrcAlphaBlendFactor(VK_BLEND_FACTOR_ONE)
                          .SetDstAlphaBlendFactor(VK_BLEND_FACTOR_ZERO)
                          .SetAlphaBlendOp(VK_BLEND_OP_ADD))));
  return render;
}

class SkyboxRenderConfiguration : public RenderConfiguration {
 public:
  DescriptorSetLayout ConfigureDescriptors(
      const VirtualDevice& virtualDevice) const override {
    return virtualDevice.CreateDescriptorSetLayout(
        DescriptorSetLayoutCreateInfoBuilder().SetBindingCount(1).SetPBindings(
            DescriptorSetLayoutBindingBuilder()
                .SetBinding(0)
                .SetDescriptorCount(1)
                .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
                .SetStageFlags(VK_SHADER_STAGE_FRAGMENT_BIT)));
  }

  std::vector<ShaderConfiguration> ConfigureShaders() const override {
    return {{VK_SHADER_STAGE_VERTEX_BIT, "shaders/sky.vert.spv"},
            {VK_SHADER_STAGE_FRAGMENT_BIT, "shaders/sky.frag.spv"}};
  }

  std::vector<VkVertexInputBindingDescription> ConfigureVertexBindings()
      const override {
    return {
        VertexInputBindingDescriptionBuilder()
            .SetBinding(0)
            .SetStride(sizeof(PositionTextureVertex))
            .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX),
    };
  }

  std::vector<VkVertexInputAttributeDescription> ConfigureVertexAttributes()
      const override {
    return {
        VertexInputAttributeDescriptionBuilder()
            .SetBinding(0)
            .SetLocation(0)
            .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
            .SetOffset(offsetof(PositionTextureVertex, position)),
        VertexInputAttributeDescriptionBuilder()
            .SetBinding(0)
            .SetLocation(1)
            .SetFormat(VK_FORMAT_R32G32_SFLOAT)
            .SetOffset(offsetof(PositionTextureVertex, textureCoordinate)),
    };
  }
};

class SunRenderConfiguration : public RenderConfiguration {
 public:
  DescriptorSetLayout ConfigureDescriptors(
      const VirtualDevice& virtualDevice) const override {
    return virtualDevice.CreateDescriptorSetLayout(
        DescriptorSetLayoutCreateInfoBuilder().SetBindingCount(1).SetPBindings(
            DescriptorSetLayoutBindingBuilder()
                .SetBinding(0)
                .SetDescriptorCount(1)
                .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
                .SetStageFlags(VK_SHADER_STAGE_FRAGMENT_BIT)));
  }

  std::vector<ShaderConfiguration> ConfigureShaders() const override {
    return {{VK_SHADER_STAGE_VERTEX_BIT, "shaders/light.vert.spv"},
            {VK_SHADER_STAGE_FRAGMENT_BIT, "shaders/light.frag.spv"}};
  }

  std::vector<VkVertexInputBindingDescription> ConfigureVertexBindings()
      const override {
    return {
        VertexInputBindingDescriptionBuilder()
            .SetBinding(0)
            .SetStride(sizeof(PositionTextureVertex))
            .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX),
    };
  }

  std::vector<VkVertexInputAttributeDescription> ConfigureVertexAttributes()
      const override {
    return {
        VertexInputAttributeDescriptionBuilder()
            .SetBinding(0)
            .SetLocation(0)
            .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
            .SetOffset(offsetof(PositionTextureVertex, position)),
        VertexInputAttributeDescriptionBuilder()
            .SetBinding(0)
            .SetLocation(1)
            .SetFormat(VK_FORMAT_R32G32_SFLOAT)
            .SetOffset(offsetof(PositionTextureVertex, textureCoordinate)),
    };
  }
};

class SpaceshipRenderConfiguration : public RenderConfiguration {
 public:
  DescriptorSetLayout ConfigureDescriptors(
      const VirtualDevice& virtualDevice) const override {
    constexpr std::array<VkDescriptorSetLayoutBinding, 2>
        textureSamplerLayoutBindings = {
            DescriptorSetLayoutBindingBuilder()
                .SetBinding(0)
                .SetDescriptorCount(1)
                .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
                .SetStageFlags(VK_SHADER_STAGE_FRAGMENT_BIT),
            DescriptorSetLayoutBindingBuilder()
                .SetBinding(1)
                .SetDescriptorCount(1)
                .SetDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
                .SetStageFlags(VK_SHADER_STAGE_FRAGMENT_BIT),
        };
    return virtualDevice.CreateDescriptorSetLayout(
        DescriptorSetLayoutCreateInfoBuilder()
            .SetBindingCount(textureSamplerLayoutBindings.size())
            .SetPBindings(textureSamplerLayoutBindings.data()));
  }

  std::vector<ShaderConfiguration> ConfigureShaders() const override {
    return {{VK_SHADER_STAGE_VERTEX_BIT, "shaders/spaceship.vert.spv"},
            {VK_SHADER_STAGE_FRAGMENT_BIT, "shaders/spaceship.frag.spv"}};
  }

  std::vector<VkVertexInputBindingDescription> ConfigureVertexBindings()
      const override {
    return {
        VertexInputBindingDescriptionBuilder()
            .SetBinding(0)
            .SetStride(sizeof(PositionNormalTextureVertex))
            .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX),
    };
  }

  std::vector<VkVertexInputAttributeDescription> ConfigureVertexAttributes()
      const override {
    return {
        VertexInputAttributeDescriptionBuilder()
            .SetBinding(0)
            .SetLocation(0)
            .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
            .SetOffset(offsetof(PositionNormalTextureVertex, position)),
        VertexInputAttributeDescriptionBuilder()
            .SetBinding(0)
            .SetLocation(1)
            .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
            .SetOffset(offsetof(PositionNormalTextureVertex, normal)),
        VertexInputAttributeDescriptionBuilder()
            .SetBinding(0)
            .SetLocation(2)
            .SetFormat(VK_FORMAT_R32G32_SFLOAT)
            .SetOffset(
                offsetof(PositionNormalTextureVertex, textureCoordinate)),
    };
  }
};

class ParticleRenderConfiguration : public RenderConfiguration {
 public:
  DescriptorSetLayout ConfigureDescriptors(
      const VirtualDevice& virtualDevice) const override {
    constexpr const VkDescriptorSetLayoutBinding particleDescriptorSet =
        DescriptorSetLayoutBindingBuilder()
            .SetBinding(0)
            .SetDescriptorCount(1)
            .SetDescriptorType(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
            .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT);
    return virtualDevice.CreateDescriptorSetLayout(
        DescriptorSetLayoutCreateInfoBuilder().SetBindingCount(1).SetPBindings(
            &particleDescriptorSet));
  }

  std::vector<ShaderConfiguration> ConfigureShaders() const override {
    return {{VK_SHADER_STAGE_VERTEX_BIT, "shaders/particles.vert.spv"},
            {VK_SHADER_STAGE_FRAGMENT_BIT, "shaders/particles.frag.spv"}};
  }

  std::vector<VkVertexInputBindingDescription> ConfigureVertexBindings()
      const override {
    return {
        VertexInputBindingDescriptionBuilder()
            .SetBinding(0)
            .SetStride(sizeof(PositionVertex))
            .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX),
    };
  }

  std::vector<VkVertexInputAttributeDescription> ConfigureVertexAttributes()
      const override {
    return {
        VertexInputAttributeDescriptionBuilder()
            .SetBinding(0)
            .SetLocation(0)
            .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
            .SetOffset(offsetof(PositionVertex, position)),
    };
  }
};

Vulkan::Vulkan(const VulkanSystem& vulkanSystem, const sys::Window& window)
    : instance(CreateVulkanInstance(vulkanSystem)),
#ifdef VALIDATION
      debugMessenger(instance.CreateDebugUtilsMessenger(
          DebugUtilsMessengerCreateInfoExtBuilder()
              .SetMessageSeverity(
                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
              .SetMessageType(VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
              .SetPfnUserCallback(DebugCallback))),
#endif
      windowSurface(instance.CreateSurface(window)),
      targetPhysicalDevice(ChoosePhysicalDevice(instance)),
      physicalDeviceProperties(targetPhysicalDevice.GetProperties()),
      swapchainImages(CalculateSwapchainImages(
          windowSurface.GetCapabilities(targetPhysicalDevice))),
      queueFamilyIndex(ChooseQueueFamily(targetPhysicalDevice, windowSurface)),
      computeQueueFamilyIndex_(ChooseComputeFamily(targetPhysicalDevice)),
      virtualDevice(CreateVirtualDevice(targetPhysicalDevice, queueFamilyIndex,
                                        computeQueueFamilyIndex_)),
      queue(virtualDevice.GetQueue(queueFamilyIndex, 0)),
      computeQueue_(virtualDevice.GetQueue(computeQueueFamilyIndex_, 0)),
      pipelineCache(LoadOrCreatePipelineCache(virtualDevice)),
      deviceAllocator(&virtualDevice,
                      targetPhysicalDevice.GetMemoryProperties()),
      fence(virtualDevice.CreateFence()),
      shortExecutionCommandPool(queue.CreateCommandPool(
          VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)),
      shortExecutionCommandBuffer(
          shortExecutionCommandPool.AllocatePrimaryCommandBuffer()),
      computeCommandPool_(computeQueue_.CreateCommandPool(
          VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)),
      computeCommandBuffer_(computeCommandPool_.AllocatePrimaryCommandBuffer()),
      surfaceFormat(SelectSwapSurfaceFormat(
          windowSurface.GetFormats(targetPhysicalDevice))),
      depthStencilFormat(SelectDepthStencilFormat(
          {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
           VK_FORMAT_D24_UNORM_S8_UINT})),
      samples(SelectMsaaSamples(VK_SAMPLE_COUNT_16_BIT)),
      textureSampler(virtualDevice.CreateSampler(
          SamplerCreateInfoBuilder()
              .SetMagFilter(VK_FILTER_LINEAR)
              .SetMinFilter(VK_FILTER_LINEAR)
              .SetAddressModeU(VK_SAMPLER_ADDRESS_MODE_REPEAT)
              .SetAddressModeV(VK_SAMPLER_ADDRESS_MODE_REPEAT)
              .SetAddressModeW(VK_SAMPLER_ADDRESS_MODE_REPEAT)
              .SetAnisotropyEnable(VK_TRUE)
              .SetMaxAnisotropy(
                  physicalDeviceProperties.limits.maxSamplerAnisotropy))),
      renderPass(CreateRenderPass()),
      subpass0(SubpassReference(renderPass, 0)),
      renderCommandPool(queue.CreateCommandPool(
          VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)),
      swapchain(*this),
      oldSwapchain(),
      imageIndex(0),
      descriptorPool_(MakeDescriptorPool(virtualDevice)),
      sceneDescriptorSetLayout_(virtualDevice.CreateDescriptorSetLayout(
          DescriptorSetLayoutCreateInfoBuilder()
              .SetBindingCount(1)
              .SetPBindings(
                  DescriptorSetLayoutBindingBuilder()
                      .SetBinding(0)
                      .SetDescriptorCount(1)
                      .SetDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
                      .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT |
                                     VK_SHADER_STAGE_FRAGMENT_BIT)))),
      sceneDescriptorSet_(
          descriptorPool_.AllocateDescriptorSet(sceneDescriptorSetLayout_)),
      sceneUniformBuffer_(AllocateLocalBuffer(
          sizeof(sceneData_), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)),
      renders_(),
      computeDescriptorSetLayout_(virtualDevice.CreateDescriptorSetLayout(
          DescriptorSetLayoutCreateInfoBuilder()
              .SetBindingCount(1)
              .SetPBindings(
                  DescriptorSetLayoutBindingBuilder()
                      .SetBinding(0)
                      .SetDescriptorCount(1)
                      .SetDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
                      .SetStageFlags(VK_SHADER_STAGE_COMPUTE_BIT)))),
      computeDescriptorSet_(
          descriptorPool_.AllocateDescriptorSet(computeDescriptorSetLayout_)),
      computeRootBuffer_(AllocateLocalBuffer(
          sizeof(ComputeContext), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)),
      particleComputes_() {
  for (u32 renderIndex = 0; renderIndex < swapchain.GetImageCount();
       ++renderIndex) {
    swapchainRenderData.push_back(
        {.commandBuffer = AllocatePrimaryRenderCommandBuffer(),
         .renderCompleteSemaphore = CreateSemaphore(),
         .submitCompleteFence = CreateFence(VK_FENCE_CREATE_SIGNALED_BIT)});
  }
  for (u32 renderIndex = 0; renderIndex < swapchain.GetImageCount();
       ++renderIndex) {
    computeCommandsBuffers_.push_back(
        computeCommandPool_.AllocatePrimaryCommandBuffer());
  }

  std::vector<DescriptorSet::WriteDescriptorSet> descriptorSetWrites;
  descriptorSetWrites.push_back(sceneUniformBuffer_.WriteBuffer(
      sceneDescriptorSet_, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0));
  descriptorSetWrites.push_back(computeRootBuffer_.WriteBuffer(
      computeDescriptorSet_, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0));

  virtualDevice.UpdateDescriptorSets(descriptorSetWrites);

  CalculateProjection();

  renders_.insert(std::move(
      std::make_pair(RenderType::Skybox,
                     std::move(CreateRender(SkyboxRenderConfiguration())))));
  renders_.insert(std::move(std::make_pair(
      RenderType::Sun, std::move(CreateRender(SunRenderConfiguration())))));
  renders_.insert(std::move(
      std::make_pair(RenderType::Spaceship,
                     std::move(CreateRender(SpaceshipRenderConfiguration())))));
  renders_.insert(std::move(
      std::make_pair(RenderType::Particle,
                     std::move(CreateRender(ParticleRenderConfiguration())))));

  constexpr std::array<VkDescriptorSetLayoutBinding, 4> computeBindings{
      DescriptorSetLayoutBindingBuilder()
          .SetBinding(0)
          .SetDescriptorCount(1)
          .SetDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
          .SetStageFlags(VK_SHADER_STAGE_COMPUTE_BIT),
      DescriptorSetLayoutBindingBuilder()
          .SetBinding(1)
          .SetDescriptorCount(1)
          .SetDescriptorType(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
          .SetStageFlags(VK_SHADER_STAGE_COMPUTE_BIT),
      DescriptorSetLayoutBindingBuilder()
          .SetBinding(2)
          .SetDescriptorCount(1)
          .SetDescriptorType(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
          .SetStageFlags(VK_SHADER_STAGE_COMPUTE_BIT),
      DescriptorSetLayoutBindingBuilder()
          .SetBinding(3)
          .SetDescriptorCount(1)
          .SetDescriptorType(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
          .SetStageFlags(VK_SHADER_STAGE_COMPUTE_BIT),
  };

  Compute compute =
      Compute{.descriptorSetLayout = virtualDevice.CreateDescriptorSetLayout(
                  DescriptorSetLayoutCreateInfoBuilder()
                      .SetBindingCount(computeBindings.size())
                      .SetPBindings(computeBindings.data()))};
  compute.pipeline = virtualDevice.CreateComputePipeline(
      pipelineCache,
      virtualDevice.CreatePipelineLayout(
          {&computeDescriptorSetLayout_, &compute.descriptorSetLayout},
          PipelineLayoutCreateInfoBuilder()),
      virtualDevice.LoadComputeShader("shaders/particles.comp.spv"));

  particleComputes_.insert(
      std::make_pair(ParticleBehaviour::SpaceshipExhaust, std::move(compute)));
}

Vulkan::~Vulkan() {
  virtualDevice.WaitIdle();
  file::WriteFile(PIPELINE_CACHE_FILENAME,
                  pipelineCache.GetPipelineCacheData());
}

void Vulkan::RecreateSwapchain() {
  oldSwapchain = std::move(swapchain);
  swapchain = oldSwapchain.RecreateSwapchain(*this);

  CalculateProjection();
}

void Vulkan::CalculateProjection() {
  const VkExtent2D swapchainExtent = swapchain.GetImageExtent();
  glm::mat4 projection =
      glm::perspective(glm::radians(55.0f),
                       static_cast<float>(swapchainExtent.width) /
                           static_cast<float>(swapchainExtent.height),
                       0.1f, 3000.0f);
  projection[1][1] *= -1.0f;
  sceneData_.projection = projection;
}

VkBool32 Vulkan::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
  if ((pCallbackData->messageIdNumber == 0xc036022f) ||
      (pCallbackData->messageIdNumber == 0x618ab1e7)) {
    return VK_FALSE;
  }

  ImmediateLog(pCallbackData->pMessage);
  return VK_FALSE;
}

CommandBuffer Vulkan::AllocatePrimaryRenderCommandBuffer() const {
  return renderCommandPool.AllocatePrimaryCommandBuffer();
}

Fence Vulkan::CreateFence(const VkFenceCreateFlags flags) const {
  return virtualDevice.CreateFence(flags);
}

Texture Vulkan::LoadTexture(const std::string_view filename) {
  const file::Image image = file::ReadPng(filename);

  const Buffer stagingBuffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(image.size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  const BoundDeviceMemory stagingBufferMemory = deviceAllocator.BindMemory(
      stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  stagingBufferMemory.MapCopy(image.data.data(), stagingBuffer.Size());

  Image textureImage =
      virtualDevice.CreateImage(ImageCreateInfoBuilder(IMAGE_2D)
                                    .SetFormat(VK_FORMAT_R8G8B8A8_SRGB)
                                    .SetExtent(Extent3DBuilder()
                                                   .SetWidth(image.width)
                                                   .SetHeight(image.height)
                                                   .SetDepth(1))
                                    .SetUsage(VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                              VK_IMAGE_USAGE_SAMPLED_BIT));
  BoundDeviceMemory textureImageMemory = deviceAllocator.BindMemory(
      textureImage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  shortExecutionCommandBuffer.BeginOneTimeSubmit();
  shortExecutionCommandBuffer.CmdImageMemoryBarrier(
      textureImage, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      VK_PIPELINE_STAGE_TRANSFER_BIT,
      ImageMemoryBarrierBuilder(IMAGE_MEMORY_BARRIER_NO_OWNERSHIP_TRANSFER)
          .SetSrcAccessMask(0)
          .SetDstAccessMask(VK_ACCESS_TRANSFER_WRITE_BIT)
          .SetOldLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetNewLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));
  shortExecutionCommandBuffer.CmdCopyBufferToImage(
      stagingBuffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      BufferImageCopyBuilder()
          .SetImageSubresource(SUBRESOURCE_LAYERS_COLOR_SINGLE_LAYER)
          .SetImageExtent(Extent3DBuilder(EXTENT3D_SINGLE_DEPTH)
                              .SetWidth(image.width)
                              .SetHeight(image.height)));
  shortExecutionCommandBuffer.CmdImageMemoryBarrier(
      textureImage, VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      ImageMemoryBarrierBuilder(IMAGE_MEMORY_BARRIER_NO_OWNERSHIP_TRANSFER)
          .SetSrcAccessMask(VK_ACCESS_TRANSFER_WRITE_BIT)
          .SetDstAccessMask(VK_ACCESS_SHADER_READ_BIT)
          .SetOldLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
          .SetNewLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));
  shortExecutionCommandBuffer.End().Submit(fence).Wait().Reset();

  ImageView textureView = textureImage.CreateView(
      ImageViewCreateInfoBuilder()
          .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
          .SetFormat(VK_FORMAT_R8G8B8A8_SRGB)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));

  return Texture{.image = BoundImage(std::move(textureImage),
                                     std::move(textureImageMemory)),
                 .view = std::move(textureView)};
}

BoundBuffer Vulkan::AllocateLocalBuffer(const std::size_t size,
                                        const VkBufferUsageFlags usage) {
  Buffer buffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE).SetSize(size).SetUsage(usage));
  BoundDeviceMemory memory = deviceAllocator.BindMemory(
      buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  return BoundBuffer(std::move(buffer), std::move(memory));
}

BoundBuffer Vulkan::AllocateDeviceBuffer(const void* const data,
                                         const std::size_t size,
                                         const VkBufferUsageFlags usage) {
  Buffer stagingBuffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  BoundDeviceMemory stagingBufferMemory = deviceAllocator.BindMemory(
      stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  stagingBufferMemory.MapCopy(data, stagingBuffer.Size());

  Buffer finalBuffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage));
  BoundDeviceMemory finalBufferMemory = deviceAllocator.BindMemory(
      finalBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  shortExecutionCommandBuffer.BeginOneTimeSubmit();
  shortExecutionCommandBuffer.CmdCopyBufferFull(stagingBuffer, finalBuffer);
  shortExecutionCommandBuffer.End().Submit(fence).Wait().Reset();

  return BoundBuffer(std::move(finalBuffer), std::move(finalBufferMemory));
}

IndexedVertexBuffer Vulkan::AllocateDrawBuffer(
    const StructuredVertexData::RawVertexData& vertexData) {
  const VkDeviceSize size =
      vertexData.verticesMemorySize + vertexData.indicesMemorySize;

  Buffer stagingBuffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  BoundDeviceMemory stagingBufferMemory = deviceAllocator.BindMemory(
      stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  void* memory = stagingBufferMemory.Map(0, stagingBuffer.Size());
  std::memcpy(memory, vertexData.vertices, vertexData.verticesMemorySize);
  std::memcpy(reinterpret_cast<u8*>(memory) + vertexData.verticesMemorySize,
              vertexData.indices, vertexData.indicesMemorySize);
  stagingBufferMemory.Unmap();

  Buffer finalBuffer = virtualDevice.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
                    VK_BUFFER_USAGE_INDEX_BUFFER_BIT));
  BoundDeviceMemory finalBufferMemory = deviceAllocator.BindMemory(
      finalBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  shortExecutionCommandBuffer.BeginOneTimeSubmit();
  shortExecutionCommandBuffer.CmdCopyBufferFull(stagingBuffer, finalBuffer);
  shortExecutionCommandBuffer.End().Submit(fence).Wait().Reset();

  return IndexedVertexBuffer(
      BoundBuffer(std::move(finalBuffer), std::move(finalBufferMemory)),
      vertexData.verticesMemorySize, vertexData.indexCount);
}

SwapchainCreateInfoBuilder Vulkan::SwapchainCreateInfo() const {
  const VkSurfaceCapabilitiesKHR surfaceCapabilities =
      windowSurface.GetCapabilities(targetPhysicalDevice);
  return SwapchainCreateInfoBuilder()
      .SetMinImageCount(swapchainImages)
      .SetImageFormat(surfaceFormat.format)
      .SetImageColorSpace(surfaceFormat.colorSpace)
      .SetImageExtent(SelectSwapExtent(surfaceCapabilities))
      .SetImageArrayLayers(1)
      .SetImageUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
      .SetImageSharingMode(VK_SHARING_MODE_EXCLUSIVE)
      .SetPreTransform(surfaceCapabilities.currentTransform)
      .SetCompositeAlpha(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
      .SetPresentMode(SelectSwapPresentMode(
          windowSurface.GetPresentModes(targetPhysicalDevice)))
      .SetClipped(VK_TRUE);
}

Swapchain Vulkan::CreateSwapchain() const {
  return virtualDevice.CreateSwapchain(windowSurface, SwapchainCreateInfo());
}

Swapchain Vulkan::CreateSwapchain(const Swapchain& oldSwapchain) const {
  return virtualDevice.CreateSwapchain(windowSurface, oldSwapchain,
                                       SwapchainCreateInfo());
}

BoundImage Vulkan::CreateDepthStencilAttachment(const Swapchain& swapchain) {
  Image image = virtualDevice.CreateImage(
      ImageCreateInfoBuilder(IMAGE_2D)
          .SetFormat(depthStencilFormat)
          .SetSamples(samples)
          .SetExtent(Extent3DBuilder(swapchain.GetImageExtent()).SetDepth(1))
          .SetUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT));
  BoundDeviceMemory memory =
      deviceAllocator.BindMemory(image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  return BoundImage(std::move(image), std::move(memory));
}

BoundImage Vulkan::CreateMultisamplingAttachment(const Swapchain& swapchain) {
  Image image = virtualDevice.CreateImage(
      ImageCreateInfoBuilder(IMAGE_2D)
          .SetFormat(swapchain.GetImageFormat())
          .SetExtent(Extent3DBuilder(swapchain.GetImageExtent()).SetDepth(1))
          .SetSamples(samples)
          .SetUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT));
  BoundDeviceMemory memory =
      deviceAllocator.BindMemory(image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  return BoundImage(std::move(image), std::move(memory));
}

std::vector<Framebuffer> Vulkan::GetFramebuffers(
    const Swapchain& swapchain,
    const std::vector<const ImageView*>& attachments) const {
  return swapchain.GetFramebuffers(renderPass, attachments);
}

Semaphore Vulkan::CreateSemaphore() const {
  return virtualDevice.CreateSemaphore();
}

MeshHandle Vulkan::LoadMesh(const RenderType renderType,
                            const MeshLoadParams& meshLoadParams) {
  std::unique_ptr<StructuredVertexData> vertexData;

  switch (renderType) {
    case RenderType::Skybox:
    case RenderType::Sun:
      vertexData = PositionTextureVertex::LoadVertexData(meshLoadParams.model);
      break;

    case RenderType::Spaceship:
      vertexData =
          PositionNormalTextureVertex::LoadVertexData(meshLoadParams.model);
      break;

    case RenderType::Particle:
      vertexData = PositionVertex::LoadVertexData(meshLoadParams.model);
      break;
  }

  if (meshLoadParams.meshTransform.has_value()) {
    vertexData->Apply(meshLoadParams.meshTransform.value());
  }

  std::vector<Texture> textures;
  if (meshLoadParams.texture.has_value()) {
    textures.push_back(std::move(LoadTexture(meshLoadParams.texture.value())));
  }
  if (meshLoadParams.emissive.has_value()) {
    textures.push_back(std::move(LoadTexture(meshLoadParams.emissive.value())));
  }

  meshes_.push_back(std::move(VulkanMesh{
      .renderType = renderType,
      .drawBuffer = AllocateDrawBuffer(vertexData->GetRawVertexData()),
      .textures = std::move(textures)}));

  return meshes_.size() - 1;
}

std::unique_ptr<Resource> Vulkan::SpawnLightSource(
    const Renderer::LightSourceInfo& lightSourceInfo) {
  const ResourceKey key = GenerateResourceKey();
  pointLights_.insert(std::make_pair(
      key,
      PointLightSource{.transform = lightSourceInfo.transformable,
                       .info = lightSourceInfo.lightSource.info.pointLight}));
  return std::make_unique<VulkanResource>(this, key);
}

std::unique_ptr<Resource> Vulkan::SpawnParticleSystem(
    const ParticleSystemInfo& particleSystemInfo) {
  constexpr u32 PARTICLES = 64;

  VulkanMesh& mesh = meshes_[particleSystemInfo.meshHandle];
  Render& render = renders_[mesh.renderType];
  Compute& compute = particleComputes_[particleSystemInfo.particleBehaviour];

  BoundBuffer spawnParamsBuffer = AllocateLocalBuffer(
      sizeof(ParticleSpawnParams), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
  ParticleSpawnParams particleSpawnParams;
  memset(&particleSpawnParams, 0, sizeof(particleSpawnParams));
  spawnParamsBuffer.MapCopy(&particleSpawnParams, sizeof(particleSpawnParams));

  Particle particles[PARTICLES];
  memset(particles, 0, sizeof(particles));
  BoundBuffer particleBuffer =
      AllocateDeviceBuffer(particles, sizeof(Particle) * PARTICLES,
                           VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

  ParticleRender renders[PARTICLES];
  memset(renders, 0, sizeof(renders));
  BoundBuffer renderBuffer =
      AllocateDeviceBuffer(renders, sizeof(ParticleRender) * PARTICLES,
                           VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

  Draw draw{.descriptorSet = descriptorPool_.AllocateDescriptorSet(
                render.descriptorSetLayout),
            .transformable = particleSystemInfo.transformable,
            .drawBuffer = &mesh.drawBuffer,
            .instances = PARTICLES};

  ComputeInstance computeInstance;
  computeInstance.descriptorSet =
      descriptorPool_.AllocateDescriptorSet(compute.descriptorSetLayout);

  std::vector<DescriptorSet::WriteDescriptorSet> descriptorSetWrites;
  descriptorSetWrites.push_back(renderBuffer.WriteBuffer(
      draw.descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 0));
  descriptorSetWrites.push_back(spawnParamsBuffer.WriteBuffer(
      computeInstance.descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0));
  descriptorSetWrites.push_back(particleBuffer.WriteBuffer(
      computeInstance.descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1));
  descriptorSetWrites.push_back(renderBuffer.WriteBuffer(
      computeInstance.descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2));
  virtualDevice.UpdateDescriptorSets(descriptorSetWrites);

  computeInstance.base = particleSystemInfo.transformable;
  computeInstance.spawn = particleSystemInfo.spawnControllable;
  computeInstance.spawnRegionLow = particleSystemInfo.spawnRegionLow;
  computeInstance.spawnRegionHigh = particleSystemInfo.spawnRegionHigh;
  computeInstance.particleSpawnParamsBuffer = std::move(spawnParamsBuffer);
  computeInstance.particleBuffer = std::move(particleBuffer);
  computeInstance.particleRenderBuffer = std::move(renderBuffer);

  const ResourceKey key = GenerateResourceKey();

  render.draws.insert(std::make_pair(key, std::move(draw)));
  compute.instances.insert(std::make_pair(key, std::move(computeInstance)));

  return std::make_unique<VulkanResource>(this, key);
}

std::unique_ptr<Resource> Vulkan::SpawnRenderable(RenderInfo renderInfo) {
  VulkanMesh& mesh = meshes_[renderInfo.meshHandle];
  Render& render = renders_[mesh.renderType];

  Draw draw{.descriptorSet = descriptorPool_.AllocateDescriptorSet(
                render.descriptorSetLayout),
            .transformable = renderInfo.transformable,
            .drawBuffer = &mesh.drawBuffer,
            .instances = 1};

  std::vector<DescriptorSet::WriteDescriptorSet> descriptorSetWrites;

  for (const Texture& texture : mesh.textures) {
    descriptorSetWrites.push_back(draw.descriptorSet.CreateImageSamplerWrite(
        texture.view, textureSampler, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        descriptorSetWrites.size()));
  }

  virtualDevice.UpdateDescriptorSets(descriptorSetWrites);

  const ResourceKey key = GenerateResourceKey();
  render.draws.insert(std::make_pair(key, std::move(draw)));

  return std::make_unique<VulkanResource>(this, key);
}

void Vulkan::ScheduleCompute(const ComputeContext& context) {
  // TODO: Remove by setting fence before this
  static bool firstTime = true;
  if (firstTime) {
    firstTime = false;
  } else {
    fence.Wait().Reset();
  }

  const GlobalComputeUniform sceneUniform = {.deltaTime = context.deltaTime};
  computeRootBuffer_.MapCopy(&sceneUniform, sizeof(sceneUniform));

  computeCommandBuffer_.Begin();
  computeCommandBuffer_.CmdBindDescriptorSet(
      VK_PIPELINE_BIND_POINT_COMPUTE,
      FirstValue(particleComputes_).pipeline.GetLayout(), 0,
      computeDescriptorSet_);
  for (const Compute& compute : IterateValues(particleComputes_)) {
    computeCommandBuffer_.CmdBindComputePipeline(compute.pipeline);
    for (const ComputeInstance& instance : IterateValues(compute.instances)) {
      const ParticleSpawnParams particleSpawnParams{
          .randomSeed = randomNumberGenerator_.RandomUint(0, UINT32_MAX),
          .enableRespawn =
              instance.spawn->GetEnableSpawn() ? VK_TRUE : VK_FALSE,
          .baseTransform = instance.base->GetTransform(),
          .spawnRegionLow = instance.spawnRegionLow,
          .spawnRegionHigh = instance.spawnRegionHigh,
      };
      instance.particleSpawnParamsBuffer.MapCopy(&particleSpawnParams,
                                                 sizeof(particleSpawnParams));

      computeCommandBuffer_.CmdBindDescriptorSet(VK_PIPELINE_BIND_POINT_COMPUTE,
                                                 compute.pipeline.GetLayout(),
                                                 1, instance.descriptorSet);
      computeCommandBuffer_.CmdDispatch(1, 1, 1);
    }
  }
  computeCommandBuffer_.End().SubmitCompute(fence);
}

void Vulkan::ScheduleRender(const game::Camera& camera,
                            const sys::Window& window) {
  for (const ResourceKey key : resourcesToDispose_) {
    ReleaseResources(key);
  }
  resourcesToDispose_.clear();

  const SwapchainWithResources::AcquireNextImageResult nextImageResult =
      swapchain.AcquireNextImage();

  if (nextImageResult.status == VK_ERROR_OUT_OF_DATE_KHR) {
    return;
  }

  imageIndex = nextImageResult.imageIndex;

  const SwapchainRenderPass& swapchainRender = swapchainRenderData[imageIndex];
  const CommandBuffer& commandBuffer = swapchainRender.commandBuffer;

  swapchainRender.submitCompleteFence.Wait().Reset();
  constexpr VkClearValue colorClear =
      ClearValueBuilder().SetColor(ClearColorValueBuilder()
                                       .SetFloat0(0.0f)
                                       .SetFloat1(0.0f)
                                       .SetFloat2(0.0f)
                                       .SetFloat3(1.0f));
  constexpr VkClearValue depthClear = ClearValueBuilder().SetDepthStencil(
      ClearDepthStencilValueBuilder().SetDepth(1.0f));

  constexpr std::array<VkClearValue, 3> clearValues{
      colorClear,
      colorClear,
      depthClear,
  };

  {
    GlobalRenderUniform& frame = sceneData_;
    frame.view = camera.GetViewMatrix();

    frame.material = {.ambient = glm::vec3(1.0f),
                      .diffuse = glm::vec3(1.0f),
                      .specular = glm::vec3(1.0f),
                      .shininess = 32.0f};

    u32 index = 0;
    for (const PointLightSource& light : IterateValues(pointLights_)) {
      GlobalRenderUniform::PointLight& pointLight = frame.pointLights[index];

      pointLight.position =
          light.transform->GetTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
      pointLight.ambient = light.info.ambient;
      pointLight.diffuse = light.info.diffuse;
      pointLight.specular = light.info.specular;
      pointLight.attenuation.constant = light.info.attenuation.constant;
      pointLight.attenuation.linear = light.info.attenuation.linear;
      pointLight.attenuation.quadratic = light.info.attenuation.quadratic;

      ++index;
    }

    frame.pointLightCount = pointLights_.size();

    sceneUniformBuffer_.MapCopy(&sceneData_, sizeof(sceneData_));
  }

  commandBuffer.Begin();
  {
    const glm::vec2 windowSizeFloat = window.GetRect().Size();
    const glm::ivec2 windowSizeInt = glm::ivec2(windowSizeFloat);
    commandBuffer.CmdSetViewport(ViewportBuilder(VIEWPORT_BASE)
                                     .SetWidth(windowSizeFloat.x)
                                     .SetHeight(windowSizeFloat.y));
    commandBuffer.CmdSetScissor(
        Rect2DBuilder()
            .SetOffset(OFFSET2D_ZERO)
            .SetExtent(Extent2DBuilder()
                           .SetWidth(windowSizeInt.x)
                           .SetHeight(windowSizeInt.y)));
  }
  commandBuffer.CmdBeginRenderPass(
      RenderPassBeginInfoBuilder()
          .SetRenderArea(Rect2DBuilder().SetExtent(swapchain.GetImageExtent()))
          .SetClearValueCount(clearValues.size())
          .SetPClearValues(clearValues.data()),
      VK_SUBPASS_CONTENTS_INLINE, renderPass, nextImageResult.framebuffer);
  commandBuffer.CmdBindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS,
                                     FirstValue(renders_).pipeline.GetLayout(),
                                     0, sceneDescriptorSet_);

  for (const Render& render : IterateValues(renders_)) {
    const GraphicsPipeline& pipeline = render.pipeline;
    const PipelineLayout& layout = pipeline.GetLayout();

    commandBuffer.CmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS,
                                  render.pipeline);

    for (const Draw& draw : IterateValues(render.draws)) {
      commandBuffer.CmdBindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS,
                                         layout, 1, draw.descriptorSet);
      const ModelTransform transform =
          ModelTransform{draw.transformable->GetTransform()};
      commandBuffer.CmdPushConstants(layout, VK_SHADER_STAGE_VERTEX_BIT, 0,
                                     sizeof(transform), &transform);
      draw.drawBuffer->DrawInstanced(commandBuffer, draw.instances);
    }
  }

  commandBuffer.CmdEndRenderPass();
  commandBuffer.End();
  commandBuffer.Submit(
      SynchronisationPack()
          .SetWaitSemaphore(&nextImageResult.semaphore)
          .SetSignalSemaphore(&swapchainRender.renderCompleteSemaphore)
          .SetSignalFence(&swapchainRender.submitCompleteFence));
  swapchain.Present(queue, SynchronisationPack().SetWaitSemaphore(
                               &swapchainRender.renderCompleteSemaphore));
}

void Vulkan::DisposeResource(const ResourceKey key) {
  resourcesToDispose_.push_back(key);
}

void Vulkan::ReleaseResources(const ResourceKey key) {
  const auto pointLightIterator = pointLights_.find(key);

  if (pointLightIterator != pointLights_.end()) {
    pointLights_.erase(pointLightIterator);
  }

  for (Compute& compute : IterateValues(particleComputes_)) {
    const auto iterator = compute.instances.find(key);

    if (iterator != compute.instances.end()) {
      compute.instances.erase(iterator);
    }
  }

  for (Render& render : IterateValues(renders_)) {
    const auto iterator = render.draws.find(key);

    if (iterator != render.draws.end()) {
      render.draws.erase(iterator);
    }
  }
}
