#include "ImGuiInstance.h"

#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_vulkan.h>
#include <imgui.h>

ImGuiInstance::ImGuiInstance() : sdlWindow(nullptr) {}

ImGuiInstance::ImGuiInstance(SDL_Window* const sdlWindow,
                             const VulkanInstance& instance,
                             const PhysicalDevice& physicalDevice,
                             const VirtualDevice& virtualDevice,
                             const Queue& queue, const RenderPass& renderPass,
                             CommandBuffer& temporaryCommandBuffer,
                             const Fence& fence)
    : sdlWindow(sdlWindow) {
  constexpr u32 poolSize = 1000;
  VkDescriptorPoolSize poolSizes[] = {
      {VK_DESCRIPTOR_TYPE_SAMPLER, poolSize},
      {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, poolSize},
      {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, poolSize},
      {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, poolSize},
      {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, poolSize},
      {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, poolSize},
      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, poolSize},
      {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, poolSize},
      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, poolSize},
      {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, poolSize},
      {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, poolSize}};

  descriptorPool = virtualDevice.CreateDescriptorPool(
      DescriptorPoolCreateInfoBuilder()
          .SetFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
          .SetMaxSets(poolSize)
          .SetPoolSizeCount(std::size(poolSizes))
          .SetPPoolSizes(poolSizes));

  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForVulkan(sdlWindow);

  ImGui_ImplVulkan_InitInfo imguiVulkanInitInfo = {};
  imguiVulkanInitInfo.Instance = instance.instance;
  imguiVulkanInitInfo.PhysicalDevice = physicalDevice.physicalDevice;
  imguiVulkanInitInfo.Device = virtualDevice.device;
  imguiVulkanInitInfo.Queue = queue.queue;
  imguiVulkanInitInfo.DescriptorPool = descriptorPool.descriptorPool;
  imguiVulkanInitInfo.MinImageCount = 3;
  imguiVulkanInitInfo.ImageCount = 3;
  imguiVulkanInitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

  ImGui_ImplVulkan_Init(&imguiVulkanInitInfo, renderPass.renderPass);

  constexpr float textSize = 32.0f;
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->AddFontFromFileTTF("resources/SEGOEUI.TTF", textSize);
  ImFontConfig fontConfig;
  fontConfig.MergeMode = true;
  // io.Fonts->AddFontFromFileTTF("resources/IBMPlexSansKR-Regular.ttf",
  // textSize, &fontConfig, io.Fonts->GetGlyphRangesKorean());
  const ImWchar iconRanges[] = {0xf104, 0xf107};
  io.Fonts->AddFontFromFileTTF("resources/fontawesome-webfont.ttf", textSize,
                               &fontConfig, iconRanges);
  io.Fonts->Build();

  temporaryCommandBuffer.BeginOneTimeSubmit();
  ImGui_ImplVulkan_CreateFontsTexture(temporaryCommandBuffer.commandBuffer);
  temporaryCommandBuffer.End().Submit(fence).Wait().Reset();

  ImGui_ImplVulkan_DestroyFontUploadObjects();
}

ImGuiInstance::~ImGuiInstance() {
  if (sdlWindow != nullptr) {
    ImGui_ImplVulkan_Shutdown();
  }
}

ImGuiInstance::ImGuiInstance(ImGuiInstance&& other) noexcept
    : sdlWindow(other.sdlWindow),
      descriptorPool(std::move(other.descriptorPool)) {
  other.sdlWindow = nullptr;
}

ImGuiInstance& ImGuiInstance::operator=(ImGuiInstance&& other) noexcept {
  sdlWindow = other.sdlWindow;
  descriptorPool = std::move(other.descriptorPool);
  other.sdlWindow = nullptr;
  return *this;
}

void ImGuiInstance::ProcessEvent(const SDL_Event& event) const {
  ImGui_ImplSDL2_ProcessEvent(&event);
}

void ImGuiInstance::BeginFrame() const {
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplSDL2_NewFrame(sdlWindow);

  ImGui::NewFrame();
}

void ImGuiInstance::RenderFrame() const {
  ImGui::Render();
}

void ImGuiInstance::SubmitFrame(const CommandBuffer& commandBuffer) const {
  ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
                                  commandBuffer.commandBuffer);
}
