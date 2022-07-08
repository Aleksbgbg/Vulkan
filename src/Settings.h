#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_

#include <cstring>
#include <memory>
#include <unordered_map>

#include "core/types.h"

#define DEFAULT_SETTING(MAP, KEY, TYPE, DEFAULT)                               \
  {                                                                            \
    constexpr TYPE Default##KEY##Value = DEFAULT;                              \
    std::unique_ptr<u8[]> default##KEY = std::make_unique<u8[]>(sizeof(TYPE)); \
    std::memcpy(default##KEY.get(), &Default##KEY##Value, sizeof(TYPE));       \
    (MAP).insert(                                                              \
        {SettingKey::KEY, Setting{.size = sizeof(TYPE),                        \
                                  .defaultValue = std::move(default##KEY)}});  \
  }

enum class SettingKey : u32 {
  SfxVolume,
  MusicVolume,
  MsaaSelectedIndex,
};

struct Setting {
  u32 size;
  std::unique_ptr<u8[]> defaultValue;
};

class Settings {
 public:
  Settings(std::unordered_map<SettingKey, Setting> settings);

  Settings(Settings&) = delete;
  Settings(Settings&&) = delete;

  ~Settings();

  Settings& operator=(Settings&) = delete;
  Settings& operator=(Settings&&) = delete;

  template <typename T>
  T Get(const SettingKey key) const {
    return *GetPointer<T>(key);
  }

  template <typename T>
  const T* GetPointer(const SettingKey key) const {
    return reinterpret_cast<T*>(settingsStore_.at(key).value.get());
  }

  void Set(SettingKey key, const void* value);

 private:
  struct StoredSetting {
    u32 size;
    std::unique_ptr<u8[]> value;
  };

  std::unordered_map<SettingKey, StoredSetting> settingsStore_;
};

#endif  // SRC_SETTINGS_H_
