#include "App.h"

#include <array>
#include <thread>

#include "general/logging/log.h"
#include "vulkan/structures/ClearValue.h"

App::App(wnd::Window& window, std::unique_ptr<Vulkan> appVulkan)
    : window(window),
      vulkan(std::move(appVulkan)),
      scene(std::make_unique<Scene>(*vulkan, *vulkan, *vulkan, *vulkan, *vulkan,
                                    *vulkan, window, imageIndex)),
      swapchain(*vulkan),
      previousTime(std::chrono::high_resolution_clock::time_point::min()),
      threadMessenger(),
      imageIndex(0) {
  const VkExtent2D swapchainExtent = swapchain.GetImageExtent();
  scene->UpdateAspect(static_cast<float>(swapchainExtent.width) /
                      static_cast<float>(swapchainExtent.height));

  for (u32 renderIndex = 0; renderIndex < swapchain.GetImageCount();
       ++renderIndex) {
    swapchainRenderData.emplace_back(SwapchainRenderPass{
        .commandBuffer = vulkan->AllocatePrimaryRenderCommandBuffer(),
        .renderCompleteSemaphore = vulkan->CreateSemaphore(),
        .submitCompleteFence =
            vulkan->CreateFence(VK_FENCE_CREATE_SIGNALED_BIT)});
  }
}

App::~App() {
  vulkan->WaitIdle();
}

int App::Run() {
  std::thread renderThread(&App::RenderThread, this);
  MainThread();
  renderThread.join();
  return 0;
}

void App::MainThread() {
  while (true) {
    switch (window.WaitAndProcessEvent()) {
      case wnd::Window::Event::Exit:
        threadMessenger.PostMessage(EventNotification::Unpaused);
        threadMessenger.PostMessage(EventNotification::Exited);
        return;

      case wnd::Window::Event::Minimized:
        threadMessenger.PostMessage(EventNotification::Paused);
        break;

      case wnd::Window::Event::Restored:
        threadMessenger.PostMessage(EventNotification::Unpaused);
        break;

      case wnd::Window::Event::SizeChanged:
        threadMessenger.PostMessage(EventNotification::Resized);
        break;
    }
  }
}

void App::RenderThread() {
  try {
    while (true) {
      MainLoop();

      while (threadMessenger.HasMessage()) {
        EventNotification message = threadMessenger.PopMessage();

        switch (message) {
          case EventNotification::Exited:
            return;

          case EventNotification::Paused:
            threadMessenger.WaitMessage(EventNotification::Unpaused);
            break;

          case EventNotification::Resized:
            oldSwapchain = std::move(swapchain);
            swapchain = oldSwapchain.RecreateSwapchain(*vulkan);
            const VkExtent2D swapchainExtent = swapchain.GetImageExtent();
            scene->UpdateAspect(static_cast<float>(swapchainExtent.width) /
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
  if (previousTime == std::chrono::high_resolution_clock::time_point::min()) {
    previousTime = std::chrono::high_resolution_clock::now();
  }

  const auto timeNow = std::chrono::high_resolution_clock::now();
  const float deltaTime =
      std::chrono::duration<float, std::chrono::seconds::period>(timeNow -
                                                                 previousTime)
          .count();

  UpdateModel(deltaTime);
  Render();

  previousTime = timeNow;
}

void App::UpdateModel(const float deltaTime) {
  controls.Update(window.GetKeyboard(), window.GetMouse(),
                  window.GetRect().Size());
  window.EndFrame();

  const UpdateContext updateContext{.deltaTime = deltaTime,
                                    .controls = controls};
  scene->UpdateModel(updateContext);
}

void App::Render() {
  const SwapchainWithResources::AcquireNextImageResult nextImageResult =
      swapchain.AcquireNextImage();

  if (nextImageResult.status == VK_ERROR_OUT_OF_DATE_KHR) {
    return;
  }

  imageIndex = nextImageResult.imageIndex;

  SwapchainRenderPass& swapchainRender = swapchainRenderData[imageIndex];

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
  vulkan->CmdBeginRenderPass(
      swapchainRender.commandBuffer,
      RenderPassBeginInfoBuilder()
          .SetRenderArea(Rect2DBuilder().SetExtent(swapchain.GetImageExtent()))
          .SetClearValueCount(clearValues.size())
          .SetPClearValues(clearValues.data()),
      swapchain.CurrentFramebuffer());
  scene->Render(swapchainRender.commandBuffer);
  swapchainRender.commandBuffer.CmdEndRenderPass();
  swapchainRender.commandBuffer.End();
  swapchainRender.commandBuffer.Submit(
      SynchronisationPack()
          .SetWaitSemaphore(&nextImageResult.semaphore)
          .SetSignalSemaphore(&swapchainRender.renderCompleteSemaphore)
          .SetSignalFence(&swapchainRender.submitCompleteFence));
  vulkan->Present(swapchain, SynchronisationPack().SetWaitSemaphore(
                                 &swapchainRender.renderCompleteSemaphore));

  swapchain.MoveToNextFrame();
}
