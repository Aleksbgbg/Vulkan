#ifndef SRC_ENGINE_RENDERPERFORMANCETRACKER_H_
#define SRC_ENGINE_RENDERPERFORMANCETRACKER_H_

#include <deque>

class RenderPerformanceTracker {
 public:
  float GetFps() const;

  void ReportFrame(float deltaTime);

 private:
  std::deque<float> frameTimes_;
};

#endif  // SRC_ENGINE_RENDERPERFORMANCETRACKER_H_
