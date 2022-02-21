#ifndef VULKAN_SRC_GAME_GRAPHICSSETTINGSCONFIGURATOR_H_
#define VULKAN_SRC_GAME_GRAPHICSSETTINGSCONFIGURATOR_H_

#include "core/types.h"

class GraphicsSettingsConfigurator {
 public:
  virtual ~GraphicsSettingsConfigurator() = default;

  virtual void SetMsaaIndex(u32 index) = 0;
};

#endif  // VULKAN_SRC_GAME_GRAPHICSSETTINGSCONFIGURATOR_H_
