#ifndef VULKAN_SRC_GAME_VIEWMODELS_PAUSEMENUVIEWMODEL_H_
#define VULKAN_SRC_GAME_VIEWMODELS_PAUSEMENUVIEWMODEL_H_

#include "Settings.h"
#include "core/types.h"
#include "game/GraphicsSettingsConfigurator.h"
#include "game/ui/Bound.h"
#include "game/ui/ViewModel.h"

class PauseMenuViewModel : public ViewModel {
 public:
  PauseMenuViewModel(
      Settings& settings,
      GraphicsSettingsConfigurator& graphicsSettingsConfigurator);

 private:
  void SetSfxVolume(float value);
  void SetMusicVolume(float value);
  void SetMsaaSelectedIndex(u32 value);

 private:
  Settings& settings_;
  GraphicsSettingsConfigurator& graphicsSettingsConfigurator_;
  Bound<float>& sfxVolume_;
  Bound<float>& musicVolume_;
  Bound<u32>& msaaSelectedIndex_;
};

#endif  // VULKAN_SRC_GAME_VIEWMODELS_PAUSEMENUVIEWMODEL_H_
