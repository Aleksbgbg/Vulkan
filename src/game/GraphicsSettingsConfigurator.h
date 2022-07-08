#ifndef SRC_GAME_GRAPHICSSETTINGSCONFIGURATOR_H_
#define SRC_GAME_GRAPHICSSETTINGSCONFIGURATOR_H_

#include "core/types.h"

class GraphicsSettingsConfigurator {
 public:
  virtual ~GraphicsSettingsConfigurator() = default;

  virtual void SetMsaaIndex(u32 index) = 0;
};

#endif  // SRC_GAME_GRAPHICSSETTINGSCONFIGURATOR_H_
