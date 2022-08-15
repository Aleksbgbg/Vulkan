#include "RenderPerformanceTracker.h"

#include <numeric>

#include "core/types.h"

namespace {

constexpr u32 MAX_FRAMES = 100;

}  // namespace

float RenderPerformanceTracker::GetFps() const {
  const float averageFrameTime =
      std::accumulate(std::begin(frameTimes_), std::end(frameTimes_), 0.0f) /
      frameTimes_.size();

  return 1.0f / averageFrameTime;
}

void RenderPerformanceTracker::ReportFrame(const float deltaTime) {
  frameTimes_.push_back(deltaTime);

  if (frameTimes_.size() > MAX_FRAMES) {
    frameTimes_.pop_front();
  }
}
