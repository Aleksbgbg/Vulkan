#include "Settings.h"

#include <fstream>

static constexpr const char* SETTINGS_FILENAME = "settings";

SettingKey ReadKey(std::ifstream& stream) {
  SettingKey key;
  stream.read(reinterpret_cast<char*>(&key), sizeof(key));
  return key;
}

Settings::Settings(std::unordered_map<SettingKey, Setting> settings)
    : settingsStore_() {
  std::ifstream stream(SETTINGS_FILENAME, std::ios::binary);

  while (stream.peek() != -1) {
    const SettingKey key = ReadKey(stream);
    const u32 size = settings.at(key).size;

    std::unique_ptr<u8[]> value = std::make_unique<u8[]>(size);
    stream.read(reinterpret_cast<char*>(value.get()), size);

    settingsStore_.insert(std::make_pair(
        key, StoredSetting{.size = size, .value = std::move(value)}));
  }

  for (auto& pair : settings) {
    if (!settingsStore_.contains(pair.first)) {
      settingsStore_.insert(std::make_pair(
          pair.first,
          StoredSetting{.size = pair.second.size,
                        .value = std::move(pair.second.defaultValue)}));
    }
  }
}

Settings::~Settings() {
  std::ofstream stream(SETTINGS_FILENAME, std::ios::binary);

  for (const auto& pair : settingsStore_) {
    stream.write(reinterpret_cast<const char*>(&pair.first),
                 sizeof(pair.first));
    stream.write(reinterpret_cast<const char*>(pair.second.value.get()),
                 pair.second.size);
  }
}

void Settings::Set(const SettingKey key, const void* const value) {
  const StoredSetting& setting = settingsStore_.at(key);
  std::memcpy(setting.value.get(), value, setting.size);
}
