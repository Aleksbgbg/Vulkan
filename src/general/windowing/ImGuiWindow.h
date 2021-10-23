#ifndef VULKAN_SRC_GENERAL_WINDOWING_IMGUIWINDOW_H
#define VULKAN_SRC_GENERAL_WINDOWING_IMGUIWINDOW_H

class ImGuiWindow {
 public:
  virtual ~ImGuiWindow() = default;

  virtual void InitImGui() const = 0;
  virtual void ShutdownImGui() const = 0;

  virtual void NewFrame() const = 0;
};

#endif  // VULKAN_SRC_GENERAL_WINDOWING_IMGUIWINDOW_H
