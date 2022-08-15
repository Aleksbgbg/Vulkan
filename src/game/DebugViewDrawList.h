#ifndef SRC_GAME_DEBUGVIEWDRAWLIST_H_
#define SRC_GAME_DEBUGVIEWDRAWLIST_H_

#include "core/text/FontAtlas.h"
#include "engine/RenderPerformanceTracker.h"
#include "engine/ui/DrawList.h"

class DebugViewDrawList : public DrawList {
 public:
  DebugViewDrawList(const FontAtlas& fontAtlas,
                    const RenderPerformanceTracker& renderPerformanceTracker);

  DrawData Generate(glm::vec2 windowSize) override;

 private:
  const FontAtlas& fontAtlas_;
  const RenderPerformanceTracker& renderPerformanceTracker_;
};

#endif  // SRC_GAME_DEBUGVIEWDRAWLIST_H_
