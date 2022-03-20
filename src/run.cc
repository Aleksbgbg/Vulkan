#include "run.h"

#include <stdexcept>

#include "App.h"
#include "Settings.h"
#include "core/diagnostics/ErrorReporter.h"
#include "core/diagnostics/log.h"
#include "core/diagnostics/profile.h"
#include "core/files/file.h"

std::unordered_map<SettingKey, Setting> DefaultSettings() {
  std::unordered_map<SettingKey, Setting> settings;
  DEFAULT_SETTING(settings, SfxVolume, float, 0.5f);
  DEFAULT_SETTING(settings, MusicVolume, float, 0.5f);
  DEFAULT_SETTING(settings, MsaaSelectedIndex, u32, 2);
  return settings;
}

int RunVulkanApp(sys::System& system) {
  PROFILING_BEGIN("Startup")

  ErrorReporter errorReporter;

  Settings settings(DefaultSettings());

  const std::unique_ptr<sys::Window> window = system.SpawnWindow(1920, 1080);
  sys::Sound sound(settings, errorReporter);

  const FontAtlas fontAtlas = FontAtlas::Create();

  Vulkan vulkan(system, *window, fontAtlas, settings);
  App app(*window, sound, vulkan, fontAtlas, settings);

  PROFILING_END

  return app.Run();
}

int run(sys::System& system) {
  try {
    return RunVulkanApp(system);
  } catch (const std::exception& exception) {
    const std::string errorString =
        std::string("Error during run:\n") + exception.what();
    ImmediateLog(errorString);
    file::WriteFile("RunDebug.txt", errorString);
    throw;
  }
}
