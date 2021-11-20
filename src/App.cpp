#include "App.h"

#include <array>
#include <thread>

#include "general/logging/log.h"
#include "vulkan/structures/ClearValue.h"

App::App(wnd::Window& window, std::unique_ptr<Vulkan> vulkan,
         std::unique_ptr<AppNetwork> appNetwork)
    : window_(window),
      appNetwork_(std::move(appNetwork)),
      controls_(),
      mainPlayerController_(appNetwork_->ActorIndexForCurrentApp()),
      vulkan_(std::move(vulkan)),
      scene_(std::make_unique<Scene>(*vulkan_, *vulkan_, *vulkan_, *vulkan_,
                                     *vulkan_, mainPlayerController_, *vulkan_,
                                     window, imageIndex_)),
      swapchain_(*vulkan_),
      previousTime_(std::chrono::high_resolution_clock::time_point::min()),
      threadMessenger_(),
      imageIndex_(0) {
  const VkExtent2D swapchainExtent = swapchain_.GetImageExtent();
  scene_->UpdateAspect(static_cast<float>(swapchainExtent.width) /
                       static_cast<float>(swapchainExtent.height));

  for (u32 renderIndex = 0; renderIndex < swapchain_.GetImageCount();
       ++renderIndex) {
    swapchainRenderData_.emplace_back(SwapchainRenderPass{
        .commandBuffer = vulkan_->AllocatePrimaryRenderCommandBuffer(),
        .renderCompleteSemaphore = vulkan_->CreateSemaphore(),
        .submitCompleteFence =
            vulkan_->CreateFence(VK_FENCE_CREATE_SIGNALED_BIT)});
  }
}

App::~App() {
  vulkan_->WaitIdle();
}

int App::Run() {
  std::thread renderThread(&App::RenderThread, this);
  MainThread();
  renderThread.join();
  return 0;
}

void App::MainThread() {
  while (true) {
    switch (window_.WaitAndProcessEvent()) {
      case wnd::Window::Event::Exit:
        threadMessenger_.PostMessage(EventNotification::Unpaused);
        threadMessenger_.PostMessage(EventNotification::Exited);
        return;

      case wnd::Window::Event::Minimized:
        threadMessenger_.PostMessage(EventNotification::Paused);
        break;

      case wnd::Window::Event::Restored:
        threadMessenger_.PostMessage(EventNotification::Unpaused);
        break;

      case wnd::Window::Event::SizeChanged:
        threadMessenger_.PostMessage(EventNotification::Resized);
        break;
    }
  }
}

void App::RenderThread() {
  try {
    while (true) {
      MainLoop();

      while (threadMessenger_.HasMessage()) {
        EventNotification message = threadMessenger_.PopMessage();

        switch (message) {
          case EventNotification::Exited:
            return;

          case EventNotification::Paused:
            threadMessenger_.WaitMessage(EventNotification::Unpaused);
            break;

          case EventNotification::Resized:
            oldSwapchain_ = std::move(swapchain_);
            swapchain_ = oldSwapchain_.RecreateSwapchain(*vulkan_);
            const VkExtent2D swapchainExtent = swapchain_.GetImageExtent();
            scene_->UpdateAspect(static_cast<float>(swapchainExtent.width) /
                                 static_cast<float>(swapchainExtent.height));
            break;
        }
      }
    }
  } catch (const std::exception& exception) {
    ImmediateLog(exception.what());
    throw;
  }
}

void App::MainLoop() {
  if (previousTime_ == std::chrono::high_resolution_clock::time_point::min()) {
    previousTime_ = std::chrono::high_resolution_clock::now();
  }

  const auto timeNow = std::chrono::high_resolution_clock::now();
  const float deltaTime =
      std::chrono::duration<float, std::chrono::seconds::period>(timeNow -
                                                                 previousTime_)
          .count();

  UpdateModel(deltaTime);
  Render();

  previousTime_ = timeNow;
}

void App::UpdateModel(const float deltaTime) {
  const UpdateContext updateContext{.deltaTime = deltaTime,
                                    .controls = controls_};
  controls_.Update(window_.GetKeyboard(), window_.GetMouse(),
                   window_.GetRect().Size());
  appNetwork_->Update(updateContext);
  appNetwork_->ExecuteSpawns(*scene_);
  mainPlayerController_.Update(*appNetwork_);
  window_.EndFrame();
  scene_->UpdateModel(updateContext);
}

void App::Render() {
  const SwapchainWithResources::AcquireNextImageResult nextImageResult =
      swapchain_.AcquireNextImage();

  if (nextImageResult.status == VK_ERROR_OUT_OF_DATE_KHR) {
    return;
  }

  imageIndex_ = nextImageResult.imageIndex;

  SwapchainRenderPass& swapchainRender = swapchainRenderData_[imageIndex_];

  swapchainRender.submitCompleteFence.Wait().Reset();
  constexpr const VkClearValue colorClear =
      ClearValueBuilder().SetColor(ClearColorValueBuilder()
                                       .SetFloat0(0.0f)
                                       .SetFloat1(0.0f)
                                       .SetFloat2(0.0f)
                                       .SetFloat3(1.0f));
  constexpr const VkClearValue depthClear = ClearValueBuilder().SetDepthStencil(
      ClearDepthStencilValueBuilder().SetDepth(1.0f));

  constexpr const std::array<VkClearValue, 3> clearValues{
      colorClear,
      colorClear,
      depthClear,
  };

  swapchainRender.commandBuffer.Begin();
  vulkan_->CmdBeginRenderPass(
      swapchainRender.commandBuffer,
      RenderPassBeginInfoBuilder()
          .SetRenderArea(Rect2DBuilder().SetExtent(swapchain_.GetImageExtent()))
          .SetClearValueCount(clearValues.size())
          .SetPClearValues(clearValues.data()),
      swapchain_.CurrentFramebuffer());
  scene_->Render(swapchainRender.commandBuffer);
  swapchainRender.commandBuffer.CmdEndRenderPass();
  swapchainRender.commandBuffer.End();
  swapchainRender.commandBuffer.Submit(
      SynchronisationPack()
          .SetWaitSemaphore(&nextImageResult.semaphore)
          .SetSignalSemaphore(&swapchainRender.renderCompleteSemaphore)
          .SetSignalFence(&swapchainRender.submitCompleteFence));
  vulkan_->Present(swapchain_, SynchronisationPack().SetWaitSemaphore(
                                   &swapchainRender.renderCompleteSemaphore));

  swapchain_.MoveToNextFrame();
}
