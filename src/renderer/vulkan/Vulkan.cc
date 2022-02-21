#include "Vulkan.h"

#include <array>
#include <filesystem>
#include <stdexcept>

#include "GraphicsPipelineTemplateKey.h"
#include "ParticleComputeHandler.h"
#include "Pipeline.h"
#include "TransformDescriptorWriter.h"
#include "core/adapters/MapValueIterator.h"
#include "core/files/file.h"
#include "core/files/images/png.h"
#include "core/logging/log.h"
#include "core/math/clamp.h"
#include "engine/property/traits/default/AlwaysVisible.h"
#include "engine/resource/ReleaseListResource.h"
#include "memory/Alignment.h"
#include "renderer/vertices/PositionColorVertex.h"
#include "renderer/vertices/PositionNormalTextureVertex.h"
#include "renderer/vertices/PositionTextureVertex.h"
#include "renderer/vertices/PositionVertex.h"
#include "renderer/vertices/TextVertex.h"
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
#include "renderer/vulkan/render_graph/InsertPipelineBuilder.h"
#include "renderer/vulkan/render_graph/layout/builder/DescriptorReferenceBuilder.h"
#include "renderer/vulkan/render_graph/layout/builder/DescriptorSetBuilder.h"
#include "renderer/vulkan/render_graph/layout/builder/RenderGraphLayoutBuilder.h"
#include "renderer/vulkan/render_graph/layout/builder/ShaderBuilder.h"
#include "renderer/vulkan/render_graph/layout/builder/VertexAttributesBuilder.h"
#include "renderer/vulkan/render_graph/layout/builder/bindings.h"
#include "util/build_definition.h"
#include "util/filenames.h"

static constexpr u32 WANTED_SWAPCHAIN_IMAGES = 3u;

static const AlwaysVisible ALWAYS_VISIBLE;

vk::VulkanInstance CreateVulkanInstance(const VulkanSystem& vulkanSystem) {
  const std::vector<VkExtensionProperties> availableExtensions =
      vk::LoadArray(vk::VulkanInstance::LoadInstanceExtensionProperties);

  const std::vector<const char*> requiredExtensions = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    vulkanSystem.SurfaceExtensionName(),
#if defined(VALIDATION)
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
  };

  if (!vk::RequiredValuesAreAvailable(
          availableExtensions, requiredExtensions,
          [](const VkExtensionProperties& property) {
            return property.extensionName;
          })) {
    throw std::runtime_error("Required extensions not available.");
  }

  const std::vector<VkLayerProperties> availableLayers =
      vk::LoadArray(vk::VulkanInstance::LoadInstanceLayerProperties);
  const std::vector<const char*> requiredLayers = {
#ifdef VALIDATION
      "VK_LAYER_KHRONOS_validation",
#endif
  };

  if (!vk::RequiredValuesAreAvailable(availableLayers, requiredLayers,
                                      [](const VkLayerProperties& property) {
                                        return property.layerName;
                                      })) {
    throw std::runtime_error("Required layers not available.");
  }

  return vk::VulkanInstance(
      InstanceCreateInfoBuilder()
          .SetApplicationInfo(ApplicationInfoBuilder()
                                  .SetPApplicationName("Application")
                                  .SetApplicationVersion(1))
          .SetEnabledExtensionCount(requiredExtensions.size())
          .SetPpEnabledExtensionNames(requiredExtensions.data())
          .SetEnabledLayerCount(requiredLayers.size())
          .SetPpEnabledLayerNames(requiredLayers.data()));
}

vk::PhysicalDevice ChoosePhysicalDevice(const vk::VulkanInstance& instance) {
  std::vector<vk::PhysicalDevice> physicalDevices =
      instance.GetPhysicalDevices();

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

  return Clamp(WANTED_SWAPCHAIN_IMAGES, surfaceCapabilities.minImageCount,
               maxImageCount);
}

u32 ChooseQueueFamily(const vk::PhysicalDevice& physicalDevice,
                      const vk::Surface& windowSurface) {
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

u32 ChooseComputeFamily(const vk::PhysicalDevice& physicalDevice) {
  const std::optional<u32> queueFamilyIndex =
      physicalDevice.FindAppropriateQueueFamily(VK_QUEUE_TRANSFER_BIT |
                                                VK_QUEUE_COMPUTE_BIT);

  if (!queueFamilyIndex.has_value()) {
    throw std::runtime_error("Could not find compute queue.");
  }

  return queueFamilyIndex.value();
}

vk::VirtualDevice CreateVirtualDevice(const vk::PhysicalDevice& physicalDevice,
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

vk::PipelineCache LoadOrCreatePipelineCache(
    const vk::VirtualDevice& virtualDevice) {
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

VkPresentModeKHR Vulkan::SelectSwapPresentMode(
    const std::vector<VkPresentModeKHR>& availablePresentModes) {
  BufferedLog("Selecting vertically synced swapchain present mode.");
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkFormat Vulkan::SelectDepthStencilFormat(
    const std::vector<VkFormat>& potentialFormats) const {
  for (const VkFormat format : potentialFormats) {
    const VkFormatProperties formatProperties =
        targetPhysicalDevice_.GetFormatProperties(format);

    if (formatProperties.optimalTilingFeatures &
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
      return format;
    }
  }

  throw std::runtime_error("Could not find suitable depth stencil format.");
}

VkSampleCountFlagBits Vulkan::SelectMsaaSamples(
    const Settings& settings) const {
  VkSampleCountFlagBits preferredSamples = static_cast<VkSampleCountFlagBits>(
      VK_SAMPLE_COUNT_1_BIT
      << settings.Get<u32>(SettingKey::MsaaSelectedIndex));

  // TODO: No anti-aliasing is not currently supported
  if (preferredSamples == VK_SAMPLE_COUNT_1_BIT) {
    preferredSamples = VK_SAMPLE_COUNT_2_BIT;
  }

  const VkSampleCountFlags supportedSamples =
      physicalDeviceProperties_.limits.framebufferColorSampleCounts &
      physicalDeviceProperties_.limits.framebufferDepthSampleCounts;

  for (VkSampleCountFlagBits samples = preferredSamples;
       samples > VK_SAMPLE_COUNT_1_BIT;
       samples = static_cast<VkSampleCountFlagBits>(samples >> 1)) {
    if ((supportedSamples & samples) == samples) {
      return samples;
    }
  }

  return VK_SAMPLE_COUNT_2_BIT;
}

vk::RenderPass Vulkan::CreateRenderPass() const {
  const VkFormat swapchainImageFormat = surfaceFormat_.format;
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
          .SetSamples(samples_)
          .SetLoadOp(VK_ATTACHMENT_LOAD_OP_CLEAR)
          .SetStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetStencilLoadOp(VK_ATTACHMENT_LOAD_OP_DONT_CARE)
          .SetStencilStoreOp(VK_ATTACHMENT_STORE_OP_DONT_CARE)
          .SetInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetFinalLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL),
      AttachmentDescriptionBuilder()
          .SetFormat(depthStencilFormat_)
          .SetSamples(samples_)
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

  return virtualDevice_.CreateRenderPass(
      RenderPassCreateInfoBuilder()
          .SetAttachmentCount(attachmentDescriptions.size())
          .SetPAttachments(attachmentDescriptions.data())
          .SetSubpassCount(subpasses.size())
          .SetPSubpasses(subpasses.data()));
}

vk::DescriptorPool MakeDescriptorPool(const vk::VirtualDevice& virtualDevice) {
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

RenderGraph Vulkan::CreateRenderGraph() {
  return RenderGraph(
      *this, swapchain_.GetImageCount(),
      RenderGraphLayoutBuilder()
          .GraphicsPipelineTemplate(
              GRAPHICS_PIPELINE_TEMPLATE_KEY_RENDER_3D,
              GraphicsPipelineCreateInfoBuilder()
                  .SetPDepthStencilState(
                      PipelineDepthStencilStateCreateInfoBuilder()
                          .SetDepthTestEnable(VK_TRUE)
                          .SetDepthWriteEnable(VK_TRUE)
                          .SetDepthCompareOp(VK_COMPARE_OP_LESS))
                  .SetPRasterizationState(
                      PipelineRasterizationStateCreateInfoBuilder()
                          .SetCullMode(VK_CULL_MODE_BACK_BIT)
                          .SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE)
                          .SetPolygonMode(VK_POLYGON_MODE_FILL)
                          .SetLineWidth(1.0f))
                  .SetPColorBlendState(
                      PipelineColorBlendStateCreateInfoBuilder()
                          .SetAttachmentCount(1)
                          .SetPAttachments(
                              PipelineColorBlendAttachmentStateBuilder()
                                  .SetColorWriteMask(
                                      VK_COLOR_COMPONENT_R_BIT |
                                      VK_COLOR_COMPONENT_G_BIT |
                                      VK_COLOR_COMPONENT_B_BIT))))
          .GraphicsPipelineTemplate(
              GRAPHICS_PIPELINE_TEMPLATE_KEY_UI,
              GraphicsPipelineCreateInfoBuilder()
                  .SetPDepthStencilState(
                      PipelineDepthStencilStateCreateInfoBuilder())
                  .SetPRasterizationState(
                      PipelineRasterizationStateCreateInfoBuilder()
                          .SetPolygonMode(VK_POLYGON_MODE_FILL)
                          .SetLineWidth(1.0f))
                  .SetPColorBlendState(
                      PipelineColorBlendStateCreateInfoBuilder()
                          .SetAttachmentCount(1)
                          .SetPAttachments(
                              PipelineColorBlendAttachmentStateBuilder()
                                  .SetBlendEnable(VK_TRUE)
                                  .SetColorBlendOp(VK_BLEND_OP_ADD)
                                  .SetSrcColorBlendFactor(
                                      VK_BLEND_FACTOR_SRC_ALPHA)
                                  .SetDstColorBlendFactor(
                                      VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA)
                                  .SetAlphaBlendOp(VK_BLEND_OP_ADD)
                                  .SetSrcAlphaBlendFactor(VK_BLEND_FACTOR_ONE)
                                  .SetDstAlphaBlendFactor(VK_BLEND_FACTOR_ZERO)
                                  .SetColorWriteMask(
                                      VK_COLOR_COMPONENT_R_BIT |
                                      VK_COLOR_COMPONENT_G_BIT |
                                      VK_COLOR_COMPONENT_B_BIT |
                                      VK_COLOR_COMPONENT_A_BIT))))
          .ComputeGlobalDescriptors(DescriptorSetBuilder().AddBinding(
              UniformStructure<GlobalComputeUniform>(
                  STRUCTURE_FLAGS_HOST_ACCESSIBLE)))
          .ComputePipeline(
              PIPELINE_PARTICLE_COMPUTE, "particles",
              DescriptorSetBuilder()
                  .AddBinding(UniformStructure<ParticleSpawnParams>(
                      STRUCTURE_FLAGS_HOST_ACCESSIBLE))
                  .AddBinding(BufferStructurePerInstance<Particle>())
                  .AddBinding(BufferStructurePerInstance<ParticleRender>()),
              ShaderBuilder().AddComputeShader(
                  DescriptorReferenceBuilder()
                      .AddGlobalSetBindings(0)
                      .AddLocalSetBindings({0, 1, 2})))
          .RenderGlobalDescriptors(DescriptorSetBuilder().AddBinding(
              UniformStructure<GlobalRenderUniform>(
                  STRUCTURE_FLAGS_HOST_ACCESSIBLE)))
          .RenderPipeline(
              PIPELINE_SKYBOX_RENDER, GRAPHICS_PIPELINE_TEMPLATE_KEY_RENDER_3D,
              "skybox",
              VertexInputBindingDescriptionBuilder()
                  .SetBinding(0)
                  .SetStride(sizeof(PositionTextureVertex))
                  .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX),
              VertexAttributesBuilder()
                  .AddDescription(
                      VertexInputAttributeDescriptionBuilder()
                          .SetBinding(0)
                          .SetLocation(0)
                          .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
                          .SetOffset(offsetof(PositionTextureVertex, position)))
                  .AddDescription(VertexInputAttributeDescriptionBuilder()
                                      .SetBinding(0)
                                      .SetLocation(1)
                                      .SetFormat(VK_FORMAT_R32G32_SFLOAT)
                                      .SetOffset(offsetof(PositionTextureVertex,
                                                          textureCoordinate))),
              DescriptorSetBuilder().AddBinding(
                  TextureSampler(textureSampler_)),
              ShaderBuilder()
                  .AddVertexShader(
                      DescriptorReferenceBuilder().AddGlobalSetBindings(0))
                  .AddFragmentShader(
                      DescriptorReferenceBuilder().AddLocalSetBindings(0)))
          .RenderPipeline(
              PIPELINE_LIGHT_RENDER, GRAPHICS_PIPELINE_TEMPLATE_KEY_RENDER_3D,
              "light",
              VertexInputBindingDescriptionBuilder()
                  .SetBinding(0)
                  .SetStride(sizeof(PositionTextureVertex))
                  .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX),
              VertexAttributesBuilder()
                  .AddDescription(
                      VertexInputAttributeDescriptionBuilder()
                          .SetBinding(0)
                          .SetLocation(0)
                          .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
                          .SetOffset(offsetof(PositionTextureVertex, position)))
                  .AddDescription(VertexInputAttributeDescriptionBuilder()
                                      .SetBinding(0)
                                      .SetLocation(1)
                                      .SetFormat(VK_FORMAT_R32G32_SFLOAT)
                                      .SetOffset(offsetof(PositionTextureVertex,
                                                          textureCoordinate))),
              DescriptorSetBuilder()
                  .AddBinding(UniformStructure<ModelTransform>(
                      STRUCTURE_FLAGS_HOST_ACCESSIBLE))
                  .AddBinding(TextureSampler(textureSampler_)),
              ShaderBuilder()
                  .AddVertexShader(DescriptorReferenceBuilder()
                                       .AddGlobalSetBindings(0)
                                       .AddLocalSetBindings(0))
                  .AddFragmentShader(
                      DescriptorReferenceBuilder().AddLocalSetBindings(1)))
          .RenderPipeline(
              PIPELINE_PARTICLE_RENDER,
              GRAPHICS_PIPELINE_TEMPLATE_KEY_RENDER_3D, "particles",
              VertexInputBindingDescriptionBuilder()
                  .SetBinding(0)
                  .SetStride(sizeof(PositionVertex))
                  .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX),
              VertexAttributesBuilder().AddDescription(
                  VertexInputAttributeDescriptionBuilder()
                      .SetBinding(0)
                      .SetLocation(0)
                      .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
                      .SetOffset(offsetof(PositionVertex, position))),
              DescriptorSetBuilder().AddBinding(
                  BufferStructurePerInstance<ParticleRender>()),
              ShaderBuilder()
                  .AddVertexShader(DescriptorReferenceBuilder()
                                       .AddGlobalSetBindings(0)
                                       .AddLocalSetBindings(0))
                  .AddFragmentShader())
          .RenderPipeline(
              PIPELINE_SPACESHIP_RENDER,
              GRAPHICS_PIPELINE_TEMPLATE_KEY_RENDER_3D, "spaceship",
              VertexInputBindingDescriptionBuilder()
                  .SetBinding(0)
                  .SetStride(sizeof(PositionNormalTextureVertex))
                  .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX),
              VertexAttributesBuilder()
                  .AddDescription(
                      VertexInputAttributeDescriptionBuilder()
                          .SetBinding(0)
                          .SetLocation(0)
                          .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
                          .SetOffset(
                              offsetof(PositionNormalTextureVertex, position)))
                  .AddDescription(VertexInputAttributeDescriptionBuilder()
                                      .SetBinding(0)
                                      .SetLocation(1)
                                      .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
                                      .SetOffset(offsetof(
                                          PositionNormalTextureVertex, normal)))
                  .AddDescription(
                      VertexInputAttributeDescriptionBuilder()
                          .SetBinding(0)
                          .SetLocation(2)
                          .SetFormat(VK_FORMAT_R32G32_SFLOAT)
                          .SetOffset(offsetof(PositionNormalTextureVertex,
                                              textureCoordinate))),
              DescriptorSetBuilder()
                  .AddBinding(UniformStructure<ModelTransform>(
                      STRUCTURE_FLAGS_HOST_ACCESSIBLE))
                  .AddBinding(TextureSampler(textureSampler_))
                  .AddBinding(TextureSampler(textureSampler_)),
              ShaderBuilder()
                  .AddVertexShader(DescriptorReferenceBuilder()
                                       .AddGlobalSetBindings(0)
                                       .AddLocalSetBindings(0))
                  .AddFragmentShader(DescriptorReferenceBuilder()
                                         .AddGlobalSetBindings(0)
                                         .AddLocalSetBindings({1, 2})))
          .RenderPipeline(
              PIPELINE_UI_RENDER, GRAPHICS_PIPELINE_TEMPLATE_KEY_UI, "ui",
              VertexInputBindingDescriptionBuilder()
                  .SetBinding(0)
                  .SetStride(sizeof(PositionColorVertex))
                  .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX),
              VertexAttributesBuilder()
                  .AddDescription(
                      VertexInputAttributeDescriptionBuilder()
                          .SetBinding(0)
                          .SetLocation(0)
                          .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
                          .SetOffset(offsetof(PositionColorVertex, position)))
                  .AddDescription(
                      VertexInputAttributeDescriptionBuilder()
                          .SetBinding(0)
                          .SetLocation(1)
                          .SetFormat(VK_FORMAT_R32G32B32A32_SFLOAT)
                          .SetOffset(offsetof(PositionColorVertex, color))),
              DescriptorSetBuilder(),
              ShaderBuilder().AddVertexShader().AddFragmentShader())
          .RenderPipeline(
              PIPELINE_TEXT_RENDER, GRAPHICS_PIPELINE_TEMPLATE_KEY_UI, "text",
              VertexInputBindingDescriptionBuilder()
                  .SetBinding(0)
                  .SetStride(sizeof(TextVertex))
                  .SetInputRate(VK_VERTEX_INPUT_RATE_VERTEX),
              VertexAttributesBuilder()
                  .AddDescription(
                      VertexInputAttributeDescriptionBuilder()
                          .SetBinding(0)
                          .SetLocation(0)
                          .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
                          .SetOffset(offsetof(TextVertex, position)))
                  .AddDescription(
                      VertexInputAttributeDescriptionBuilder()
                          .SetBinding(0)
                          .SetLocation(1)
                          .SetFormat(VK_FORMAT_R32G32_SFLOAT)
                          .SetOffset(offsetof(TextVertex, textureCoordinate)))
                  .AddDescription(VertexInputAttributeDescriptionBuilder()
                                      .SetBinding(0)
                                      .SetLocation(2)
                                      .SetFormat(VK_FORMAT_R32G32B32_SFLOAT)
                                      .SetOffset(offsetof(TextVertex, color))),
              DescriptorSetBuilder().AddBinding(TextureSampler(fontSampler_)),
              ShaderBuilder().AddVertexShader().AddFragmentShader(
                  DescriptorReferenceBuilder().AddLocalSetBindings(0)))
          .Build());
}

Vulkan::Vulkan(const VulkanSystem& vulkanSystem, const sys::Window& window,
               const FontAtlas& fontAtlas, const Settings& settings)
    : instance_(CreateVulkanInstance(vulkanSystem)),
#ifdef VALIDATION
      debugMessenger_(instance_.CreateDebugUtilsMessenger(
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
      windowSurface_(instance_.CreateSurface(window)),
      targetPhysicalDevice_(ChoosePhysicalDevice(instance_)),
      physicalDeviceProperties_(targetPhysicalDevice_.GetProperties()),
      swapchainImages_(CalculateSwapchainImages(
          windowSurface_.GetCapabilities(targetPhysicalDevice_))),
      graphicsQueueFamilyIndex_(
          ChooseQueueFamily(targetPhysicalDevice_, windowSurface_)),
      computeQueueFamilyIndex_(ChooseComputeFamily(targetPhysicalDevice_)),
      virtualDevice_(CreateVirtualDevice(targetPhysicalDevice_,
                                         graphicsQueueFamilyIndex_,
                                         computeQueueFamilyIndex_)),
      graphicsQueue_(virtualDevice_.GetQueue(graphicsQueueFamilyIndex_, 0)),
      computeQueue_(virtualDevice_.GetQueue(computeQueueFamilyIndex_, 0)),
      pipelineCache_(LoadOrCreatePipelineCache(virtualDevice_)),
      deviceAllocator_(&virtualDevice_,
                       targetPhysicalDevice_.GetMemoryProperties()),
      fence_(virtualDevice_.CreateFence()),
      shortExecutionCommandPool_(graphicsQueue_.CreateCommandPool(
          VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)),
      shortExecutionCommandBuffer_(
          shortExecutionCommandPool_.AllocatePrimaryCommandBuffer()),
      computeCommandPool_(computeQueue_.CreateCommandPool(
          VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)),
      computeMainCommandBuffer_(
          computeCommandPool_.AllocatePrimaryCommandBuffer()),
      computeTransferCommandBuffer_(
          computeCommandPool_.AllocateSecondaryCommandBuffer()),
      computeCommandBuffer_(
          computeCommandPool_.AllocateSecondaryCommandBuffer()),
      computeCompleteFence_(
          virtualDevice_.CreateFence(VK_FENCE_CREATE_SIGNALED_BIT)),
      surfaceFormat_(SelectSwapSurfaceFormat(
          windowSurface_.GetFormats(targetPhysicalDevice_))),
      depthStencilFormat_(SelectDepthStencilFormat(
          {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
           VK_FORMAT_D24_UNORM_S8_UINT})),
      samples_(SelectMsaaSamples(settings)),
      textureSampler_(virtualDevice_.CreateSampler(
          SamplerCreateInfoBuilder()
              .SetMagFilter(VK_FILTER_LINEAR)
              .SetMinFilter(VK_FILTER_LINEAR)
              .SetAddressModeU(VK_SAMPLER_ADDRESS_MODE_REPEAT)
              .SetAddressModeV(VK_SAMPLER_ADDRESS_MODE_REPEAT)
              .SetAddressModeW(VK_SAMPLER_ADDRESS_MODE_REPEAT)
              .SetAnisotropyEnable(VK_TRUE)
              .SetMaxAnisotropy(
                  physicalDeviceProperties_.limits.maxSamplerAnisotropy))),
      fontSampler_(virtualDevice_.CreateSampler(
          SamplerCreateInfoBuilder()
              .SetMagFilter(VK_FILTER_LINEAR)
              .SetMinFilter(VK_FILTER_LINEAR)
              .SetAddressModeU(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER)
              .SetAddressModeV(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER)
              .SetAddressModeW(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER))),
      renderPass_(CreateRenderPass()),
      subpass0_(vk::SubpassReference(renderPass_, 0)),
      renderCommandPool_(graphicsQueue_.CreateCommandPool(
          VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)),
      swapchain_(*this),
      oldSwapchain_(),
      descriptorPool_(MakeDescriptorPool(virtualDevice_)),
      renderGraph_(CreateRenderGraph()),
      sceneData_(),
      randomNumberGenerator_(),
      pointLights_(),
      uiTexture_() {
  for (u32 renderIndex = 0; renderIndex < swapchain_.GetImageCount();
       ++renderIndex) {
    swapchainRenderData_.push_back(
        {.main = renderCommandPool_.AllocatePrimaryCommandBuffer(),
         .transfer = renderCommandPool_.AllocateSecondaryCommandBuffer(),
         .graphics = renderCommandPool_.AllocateSecondaryCommandBuffer(),
         .renderCompleteSemaphore = virtualDevice_.CreateSemaphore(),
         .submitCompleteFence =
             virtualDevice_.CreateFence(VK_FENCE_CREATE_SIGNALED_BIT)});
  }

  uiTexture_.push_back(LoadTexture(fontAtlas.AsImage(), VK_FORMAT_R8_SRGB));

  CalculateProjection();
}

void Vulkan::SetMsaaIndex(const u32 index) {
  // TODO: Changing the selected samples requires support for recreating only
  // the graphics pipelines in the `RenderGraph`, which will be added in the
  // future.
}

Vulkan::~Vulkan() {
  virtualDevice_.WaitIdle();
  file::WriteFile(PIPELINE_CACHE_FILENAME,
                  pipelineCache_.GetPipelineCacheData());
}

void Vulkan::RecreateSwapchain() {
  oldSwapchain_ = std::move(swapchain_);
  swapchain_ = oldSwapchain_.RecreateSwapchain(*this);

  CalculateProjection();
}

void Vulkan::CalculateProjection() {
  const VkExtent2D swapchainExtent = swapchain_.GetImageExtent();
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

Texture Vulkan::LoadTexture(const std::string_view filename) {
  return LoadTexture(file::ReadPng(filename));
}

Texture Vulkan::LoadTexture(const Bitmap& image, const VkFormat format) {
  const vk::Buffer stagingBuffer = virtualDevice_.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(image.Size())
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  const vk::BoundDeviceMemory stagingBufferMemory = deviceAllocator_.BindMemory(
      stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  stagingBufferMemory.MapCopy(image.Data(), image.Size());

  vk::Image textureImage =
      virtualDevice_.CreateImage(ImageCreateInfoBuilder(IMAGE_2D)
                                     .SetFormat(format)
                                     .SetExtent(Extent3DBuilder()
                                                    .SetWidth(image.Width())
                                                    .SetHeight(image.Height())
                                                    .SetDepth(1))
                                     .SetUsage(VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                               VK_IMAGE_USAGE_SAMPLED_BIT));
  vk::BoundDeviceMemory textureImageMemory = deviceAllocator_.BindMemory(
      textureImage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  shortExecutionCommandBuffer_.Begin(COMMAND_BUFFER_ONE_TIME_SUBMIT);
  shortExecutionCommandBuffer_.CmdImageMemoryBarrier(
      textureImage, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      VK_PIPELINE_STAGE_TRANSFER_BIT,
      ImageMemoryBarrierBuilder(IMAGE_MEMORY_BARRIER_NO_OWNERSHIP_TRANSFER)
          .SetSrcAccessMask(0)
          .SetDstAccessMask(VK_ACCESS_TRANSFER_WRITE_BIT)
          .SetOldLayout(VK_IMAGE_LAYOUT_UNDEFINED)
          .SetNewLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));
  shortExecutionCommandBuffer_.CmdCopyBufferToImage(
      stagingBuffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      BufferImageCopyBuilder()
          .SetImageSubresource(SUBRESOURCE_LAYERS_COLOR_SINGLE_LAYER)
          .SetImageExtent(Extent3DBuilder(EXTENT3D_SINGLE_DEPTH)
                              .SetWidth(image.Width())
                              .SetHeight(image.Height())));
  shortExecutionCommandBuffer_.CmdImageMemoryBarrier(
      textureImage, VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      ImageMemoryBarrierBuilder(IMAGE_MEMORY_BARRIER_NO_OWNERSHIP_TRANSFER)
          .SetSrcAccessMask(VK_ACCESS_TRANSFER_WRITE_BIT)
          .SetDstAccessMask(VK_ACCESS_SHADER_READ_BIT)
          .SetOldLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
          .SetNewLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));
  shortExecutionCommandBuffer_.End().Submit(fence_).Wait().Reset();

  vk::ImageView textureView = textureImage.CreateView(
      ImageViewCreateInfoBuilder()
          .SetViewType(VK_IMAGE_VIEW_TYPE_2D)
          .SetFormat(format)
          .SetSubresourceRange(SUBRESOURCE_RANGE_COLOR_SINGLE_LAYER));

  return Texture{.image = BoundImage(std::move(textureImage),
                                     std::move(textureImageMemory)),
                 .view = std::move(textureView)};
}

IndexedVertexBuffer Vulkan::AllocateDrawBuffer(
    const StructuredVertexData::RawVertexData& vertexData) {
  const VkDeviceSize size =
      vertexData.verticesMemorySize + vertexData.indicesMemorySize;

  vk::Buffer stagingBuffer = virtualDevice_.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
  vk::BoundDeviceMemory stagingBufferMemory = deviceAllocator_.BindMemory(
      stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  void* memory = stagingBufferMemory.Map(0, stagingBuffer.Size());
  std::memcpy(memory, vertexData.vertices, vertexData.verticesMemorySize);
  std::memcpy(reinterpret_cast<u8*>(memory) + vertexData.verticesMemorySize,
              vertexData.indices, vertexData.indicesMemorySize);
  stagingBufferMemory.Unmap();

  vk::Buffer finalBuffer = virtualDevice_.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE)
          .SetSize(size)
          .SetUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
                    VK_BUFFER_USAGE_INDEX_BUFFER_BIT));
  vk::BoundDeviceMemory finalBufferMemory = deviceAllocator_.BindMemory(
      finalBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  shortExecutionCommandBuffer_.Begin(COMMAND_BUFFER_ONE_TIME_SUBMIT);
  shortExecutionCommandBuffer_.CmdCopyBufferFull(stagingBuffer, finalBuffer);
  shortExecutionCommandBuffer_.End().Submit(fence_).Wait().Reset();

  return IndexedVertexBuffer(
      BoundBuffer(std::move(finalBuffer), std::move(finalBufferMemory)),
      vertexData.verticesMemorySize, vertexData.indexCount);
}

SwapchainCreateInfoBuilder Vulkan::SwapchainCreateInfo() const {
  const VkSurfaceCapabilitiesKHR surfaceCapabilities =
      windowSurface_.GetCapabilities(targetPhysicalDevice_);
  return SwapchainCreateInfoBuilder()
      .SetMinImageCount(swapchainImages_)
      .SetImageFormat(surfaceFormat_.format)
      .SetImageColorSpace(surfaceFormat_.colorSpace)
      .SetImageExtent(surfaceCapabilities.currentExtent)
      .SetImageArrayLayers(1)
      .SetImageUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
      .SetImageSharingMode(VK_SHARING_MODE_EXCLUSIVE)
      .SetPreTransform(surfaceCapabilities.currentTransform)
      .SetCompositeAlpha(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
      .SetPresentMode(SelectSwapPresentMode(
          windowSurface_.GetPresentModes(targetPhysicalDevice_)))
      .SetClipped(VK_TRUE);
}

vk::Swapchain Vulkan::CreateSwapchain() const {
  return virtualDevice_.CreateSwapchain(windowSurface_, SwapchainCreateInfo());
}

vk::Swapchain Vulkan::CreateSwapchain(const vk::Swapchain& oldSwapchain) const {
  return virtualDevice_.CreateSwapchain(windowSurface_, oldSwapchain,
                                        SwapchainCreateInfo());
}

BoundImage Vulkan::CreateDepthStencilAttachment(
    const vk::Swapchain& swapchain) {
  vk::Image image = virtualDevice_.CreateImage(
      ImageCreateInfoBuilder(IMAGE_2D)
          .SetFormat(depthStencilFormat_)
          .SetSamples(samples_)
          .SetExtent(Extent3DBuilder(swapchain.GetImageExtent()).SetDepth(1))
          .SetUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT));
  vk::BoundDeviceMemory memory =
      deviceAllocator_.BindMemory(image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  return BoundImage(std::move(image), std::move(memory));
}

BoundImage Vulkan::CreateMultisamplingAttachment(
    const vk::Swapchain& swapchain) {
  vk::Image image = virtualDevice_.CreateImage(
      ImageCreateInfoBuilder(IMAGE_2D)
          .SetFormat(swapchain.GetImageFormat())
          .SetExtent(Extent3DBuilder(swapchain.GetImageExtent()).SetDepth(1))
          .SetSamples(samples_)
          .SetUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT));
  vk::BoundDeviceMemory memory =
      deviceAllocator_.BindMemory(image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  return BoundImage(std::move(image), std::move(memory));
}

std::vector<vk::Framebuffer> Vulkan::GetFramebuffers(
    const vk::Swapchain& swapchain,
    const std::vector<const vk::ImageView*>& attachments) const {
  return swapchain.GetFramebuffers(renderPass_, attachments);
}

vk::Semaphore Vulkan::CreateSemaphore() const {
  return virtualDevice_.CreateSemaphore();
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
  return pointLights_.Insert({
      .info = lightSourceInfo.lightSource.info.pointLight,
      .transform = lightSourceInfo.transformable,
  });
}

std::unique_ptr<Resource> Vulkan::SpawnParticleSystem(
    const ParticleSystemInfo& particleSystemInfo) {
  constexpr u32 PARTICLES = 64;

  const VulkanMesh& mesh = meshes_[particleSystemInfo.meshHandle];

  return renderGraph_.Insert(
      InsertPipelineBuilder()
          .Compute(
              PIPELINE_PARTICLE_COMPUTE,
              {.instances = PARTICLES,
               .descriptorWriter = std::make_unique<ParticleComputeHandler>(
                   randomNumberGenerator_, particleSystemInfo.transformable,
                   particleSystemInfo.visible,
                   particleSystemInfo.spawnRegionLow,
                   particleSystemInfo.spawnRegionHigh)})
          .Render(PIPELINE_PARTICLE_RENDER, {.visible = ALWAYS_VISIBLE,
                                             .instances = PARTICLES,
                                             .drawBuffer = &mesh.drawBuffer,
                                             .textures = &mesh.textures})
          .ConnectDescriptors(
              {PipelineDescriptorReference(PIPELINE_PARTICLE_COMPUTE, 2),
               PipelineDescriptorReference(PIPELINE_PARTICLE_RENDER, 0)}));
}

std::unique_ptr<Resource> Vulkan::SpawnRenderable(
    const RenderInfo& renderInfo) {
  const VulkanMesh& mesh = meshes_[renderInfo.meshHandle];

  Pipeline pipeline;
  std::unique_ptr<HostDescriptorWriter> descriptorWriter;

  switch (mesh.renderType) {
    case RenderType::Skybox:
      pipeline = PIPELINE_SKYBOX_RENDER;
      break;
    case RenderType::Sun:
      pipeline = PIPELINE_LIGHT_RENDER;
      descriptorWriter =
          std::make_unique<TransformDescriptorWriter>(renderInfo.transformable);
      break;
    case RenderType::Spaceship:
      pipeline = PIPELINE_SPACESHIP_RENDER;
      descriptorWriter =
          std::make_unique<TransformDescriptorWriter>(renderInfo.transformable);
      break;
  }

  return renderGraph_.Insert(InsertPipelineBuilder().Render(
      pipeline, {.visible = renderInfo.visible,
                 .instances = 1,
                 .drawBuffer = &mesh.drawBuffer,
                 .textures = &mesh.textures,
                 .descriptorWriter = std::move(descriptorWriter)}));
}

std::unique_ptr<Resource> Vulkan::SpawnUi(const Renderer::UiInfo& uiInfo) {
  std::unique_ptr<IndexedVertexBuffer> elementsBuffer =
      std::make_unique<IndexedVertexBuffer>();
  std::unique_ptr<IndexedVertexBuffer> textBuffer =
      std::make_unique<IndexedVertexBuffer>();
  return std::make_unique<game::ResourceCollection>(
      std::array{renderGraph_.Insert(InsertPipelineBuilder().Render(
                     PIPELINE_UI_RENDER, {.visible = uiInfo.visible,
                                          .instances = 1,
                                          .drawBuffer = elementsBuffer.get()})),
                 renderGraph_.Insert(InsertPipelineBuilder().Render(
                     PIPELINE_TEXT_RENDER, {.visible = uiInfo.visible,
                                            .instances = 1,
                                            .drawBuffer = textBuffer.get(),
                                            .textures = &uiTexture_})),
                 uiRenders_.Insert({.visible = uiInfo.visible,
                                    .drawList = uiInfo.drawList,
                                    .elementsBuffer = std::move(elementsBuffer),
                                    .textBuffer = std::move(textBuffer)})});
}

void Vulkan::ScheduleCompute(const ComputeContext& context) {
  const GlobalComputeUniform sceneUniform = {.deltaTime = context.deltaTime};

  computeCompleteFence_.Wait().Reset();

  computeTransferCommandBuffer_.Begin(COMMAND_BUFFER_ONE_TIME_SUBMIT_SECONDARY);
  computeCommandBuffer_.Begin(COMMAND_BUFFER_ONE_TIME_SUBMIT_SECONDARY);
  renderGraph_.ExecuteComputePipelines(computeTransferCommandBuffer_,
                                       computeCommandBuffer_, &sceneUniform);
  computeTransferCommandBuffer_.End();
  computeCommandBuffer_.End();

  computeMainCommandBuffer_.Begin(COMMAND_BUFFER_ONE_TIME_SUBMIT);
  computeMainCommandBuffer_.CmdExecuteCommands(computeTransferCommandBuffer_);
  computeMainCommandBuffer_.CmdGlobalMemoryBarrier(
      VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
      MemoryBarrierBuilder()
          .SetSrcAccessMask(VK_ACCESS_TRANSFER_WRITE_BIT)
          .SetDstAccessMask(VK_ACCESS_SHADER_READ_BIT));
  computeMainCommandBuffer_.CmdExecuteCommands(computeCommandBuffer_);
  computeMainCommandBuffer_.End().SubmitCompute(computeCompleteFence_);
}

void Vulkan::ScheduleRender(const game::Camera& camera,
                            const sys::Window& window) {
  pointLights_.ReleasePendingResources();

  const SwapchainWithResources::AcquireNextImageResult nextImageResult =
      swapchain_.AcquireNextImage();

  if (nextImageResult.status == VK_ERROR_OUT_OF_DATE_KHR) {
    return;
  }

  const SwapchainRenderPass& swapchainRender =
      swapchainRenderData_[nextImageResult.imageIndex];

  swapchainRender.submitCompleteFence.Wait().Reset();
  constexpr VkClearValue colorClear =
      ClearValueBuilder().SetColor(ClearColorValueBuilder()
                                       .SetFloat0(0.0f)
                                       .SetFloat1(0.0f)
                                       .SetFloat2(0.0f)
                                       .SetFloat3(1.0f));
  const VkClearValue depthClear = ClearValueBuilder().SetDepthStencil(
      ClearDepthStencilValueBuilder().SetDepth(1.0f));

  const std::array<VkClearValue, 3> clearValues{
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
    for (const PointLightSource& light : pointLights_) {
      GlobalRenderUniform::PointLight& pointLight = frame.pointLights[index];

      pointLight.position =
          light.transform.GetTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
      pointLight.ambient = light.info.ambient;
      pointLight.diffuse = light.info.diffuse;
      pointLight.specular = light.info.specular;
      pointLight.attenuation.constant = light.info.attenuation.constant;
      pointLight.attenuation.linear = light.info.attenuation.linear;
      pointLight.attenuation.quadratic = light.info.attenuation.quadratic;

      ++index;
    }

    frame.pointLightCount = pointLights_.size();
  }

  // Render UI
  {
    for (const UiRender& render : uiRenders_) {
      if (!render.visible.IsVisible()) {
        continue;
      }

      const DrawList::DrawData drawData =
          render.drawList->Generate(window.GetSize());

      *render.elementsBuffer =
          AllocateDrawBuffer(drawData.elements->GetRawVertexData());
      *render.textBuffer =
          AllocateDrawBuffer(drawData.text->GetRawVertexData());
    }
  }

  swapchainRender.transfer.Begin(COMMAND_BUFFER_ONE_TIME_SUBMIT_SECONDARY);
  swapchainRender.graphics.BeginWithInheritance(
      COMMAND_BUFFER_ONE_TIME_SUBMIT_RENDER_PASS_CONTINUE, subpass0_,
      nextImageResult.framebuffer);
  {
    const glm::vec2 windowSizeFloat = window.GetRect().Size();
    const glm::ivec2 windowSizeInt = glm::ivec2(windowSizeFloat);
    swapchainRender.graphics.CmdSetViewport(ViewportBuilder(VIEWPORT_BASE)
                                                .SetWidth(windowSizeFloat.x)
                                                .SetHeight(windowSizeFloat.y));
    swapchainRender.graphics.CmdSetScissor(
        Rect2DBuilder()
            .SetOffset(OFFSET2D_ZERO)
            .SetExtent(Extent2DBuilder()
                           .SetWidth(windowSizeInt.x)
                           .SetHeight(windowSizeInt.y)));
  }
  renderGraph_.ExecuteRenderPipelines(swapchainRender.transfer,
                                      swapchainRender.graphics, &sceneData_,
                                      nextImageResult.imageIndex);
  swapchainRender.transfer.End();
  swapchainRender.graphics.End();

  const vk::CommandBuffer& commandBuffer = swapchainRender.main;

  commandBuffer.Begin(COMMAND_BUFFER_ONE_TIME_SUBMIT);
  commandBuffer.CmdExecuteCommands(swapchainRender.transfer);
  commandBuffer.CmdGlobalMemoryBarrier(
      VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
      MemoryBarrierBuilder()
          .SetSrcAccessMask(VK_ACCESS_TRANSFER_WRITE_BIT)
          .SetDstAccessMask(VK_ACCESS_SHADER_READ_BIT));
  commandBuffer.CmdBeginRenderPass(
      RenderPassBeginInfoBuilder()
          .SetRenderArea(Rect2DBuilder().SetExtent(swapchain_.GetImageExtent()))
          .SetClearValueCount(clearValues.size())
          .SetPClearValues(clearValues.data()),
      VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS, renderPass_,
      nextImageResult.framebuffer);
  commandBuffer.CmdExecuteCommands(swapchainRender.graphics);
  commandBuffer.CmdEndRenderPass();
  commandBuffer.End();
  commandBuffer.Submit(
      vk::SynchronisationPack()
          .SetWaitSemaphore(&nextImageResult.semaphore)
          .SetSignalSemaphore(&swapchainRender.renderCompleteSemaphore)
          .SetSignalFence(&swapchainRender.submitCompleteFence));
  swapchain_.Present(graphicsQueue_,
                     vk::SynchronisationPack().SetWaitSemaphore(
                         &swapchainRender.renderCompleteSemaphore));
}

vk::DescriptorSetLayout Vulkan::CreateDescriptorSetLayout(
    const DescriptorSetLayoutCreateInfoBuilder& infoBuilder) const {
  return virtualDevice_.CreateDescriptorSetLayout(infoBuilder);
}

vk::DescriptorSet Vulkan::CreateDescriptorSet(
    const vk::DescriptorSetLayout& layout) const {
  return descriptorPool_.AllocateDescriptorSet(layout);
}

void Vulkan::UpdateDescriptorSets(
    const std::vector<vk::DescriptorSet::WriteDescriptorSet>&
        descriptorSetWrites) const {
  virtualDevice_.UpdateDescriptorSets(descriptorSetWrites);
}

BoundBuffer Vulkan::AllocateHostBuffer(const std::size_t size,
                                       const VkBufferUsageFlags usage) {
  vk::Buffer buffer = virtualDevice_.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE).SetSize(size).SetUsage(usage));
  vk::BoundDeviceMemory memory = deviceAllocator_.BindMemory(
      buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  return BoundBuffer(std::move(buffer), std::move(memory));
}

BoundBuffer Vulkan::AllocateDeviceBuffer(const std::size_t size,
                                         const VkBufferUsageFlags usage) {
  vk::Buffer buffer = virtualDevice_.CreateBuffer(
      BufferCreateInfoBuilder(BUFFER_EXCLUSIVE).SetSize(size).SetUsage(usage));
  vk::BoundDeviceMemory bufferMemory =
      deviceAllocator_.BindMemory(buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  shortExecutionCommandBuffer_.Begin(COMMAND_BUFFER_ONE_TIME_SUBMIT);
  shortExecutionCommandBuffer_.CmdFillBuffer(buffer, size, 0);
  shortExecutionCommandBuffer_.End().Submit(fence_).Wait().Reset();

  return BoundBuffer(std::move(buffer), std::move(bufferMemory));
}

vk::ShaderModule Vulkan::LoadComputeShader(const std::string_view name) const {
  return virtualDevice_.LoadComputeShader(name);
}

vk::ShaderModule Vulkan::LoadGraphicsShader(
    const std::string_view name, const VkShaderStageFlagBits stage) const {
  return virtualDevice_.LoadShader(stage, name);
}

vk::ComputePipeline Vulkan::CreateComputePipeline(
    const std::vector<const vk::DescriptorSetLayout*>& descriptorSetLayouts,
    vk::ShaderModule computeShader) const {
  return virtualDevice_.CreateComputePipeline(
      pipelineCache_,
      virtualDevice_.CreatePipelineLayout(descriptorSetLayouts,
                                          PipelineLayoutCreateInfoBuilder()),
      std::move(computeShader));
}

vk::GraphicsPipeline Vulkan::CreateGraphicsPipeline(
    const std::vector<const vk::DescriptorSetLayout*>& descriptorSetLayouts,
    std::vector<vk::ShaderModule> shaders,
    const VkVertexInputBindingDescription& vertexInputBindingDescription,
    const std::vector<VkVertexInputAttributeDescription>&
        vertexInputAttributeDescriptions,
    GraphicsPipelineCreateInfoBuilder infoBuilder) const {
  constexpr std::array<VkDynamicState, 2> dynamicStates = {
      VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
  return virtualDevice_.CreateGraphicsPipeline(
      pipelineCache_, shaders,
      virtualDevice_.CreatePipelineLayout(descriptorSetLayouts,
                                          PipelineLayoutCreateInfoBuilder()),
      subpass0_,
      infoBuilder
          .SetPVertexInputState(
              PipelineVertexInputStateCreateInfoBuilder()
                  .SetVertexBindingDescriptionCount(1)
                  .SetPVertexBindingDescriptions(&vertexInputBindingDescription)
                  .SetVertexAttributeDescriptionCount(
                      vertexInputAttributeDescriptions.size())
                  .SetPVertexAttributeDescriptions(
                      vertexInputAttributeDescriptions.data()))
          .SetPInputAssemblyState(
              PipelineInputAssemblyStateCreateInfoBuilder().SetTopology(
                  VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST))
          .SetPViewportState(PipelineViewportStateCreateInfoBuilder()
                                 .SetViewportCount(1)
                                 .SetScissorCount(1))
          .SetPDynamicState(PipelineDynamicStateCreateInfoBuilder()
                                .SetDynamicStateCount(dynamicStates.size())
                                .SetPDynamicStates(dynamicStates.data()))
          .SetPMultisampleState(PipelineMultisampleStateCreateInfoBuilder()
                                    .SetRasterizationSamples(samples_)
                                    .SetMinSampleShading(1.0f)));
}
