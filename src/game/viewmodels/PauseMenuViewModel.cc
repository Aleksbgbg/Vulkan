#include "PauseMenuViewModel.h"

PauseMenuViewModel::PauseMenuViewModel(
    Settings& settings,
    GraphicsSettingsConfigurator& graphicsSettingsConfigurator)
    : ViewModel(),
      settings_(settings),
      graphicsSettingsConfigurator_(graphicsSettingsConfigurator),
      sfxVolume_(BIND(SfxVolume)),
      musicVolume_(BIND(MusicVolume)),
      msaaSelectedIndex_(BIND(MsaaSelectedIndex)) {
  sfxVolume_.SetValue(settings.Get<float>(SettingKey::SfxVolume));
  musicVolume_.SetValue(settings.Get<float>(SettingKey::MusicVolume));
  msaaSelectedIndex_.SetValue(settings.Get<u32>(SettingKey::MsaaSelectedIndex));
}

void PauseMenuViewModel::SetSfxVolume(const float value) {
  settings_.Set(SettingKey::SfxVolume, &value);
}

void PauseMenuViewModel::SetMusicVolume(const float value) {
  settings_.Set(SettingKey::MusicVolume, &value);
}

void PauseMenuViewModel::SetMsaaSelectedIndex(const u32 value) {
  settings_.Set(SettingKey::MsaaSelectedIndex, &value);
  graphicsSettingsConfigurator_.SetMsaaIndex(value);
}
