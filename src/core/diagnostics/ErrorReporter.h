#ifndef VULKAN_SRC_CORE_DIAGNOSTICS_ERRORREPORTER_H_
#define VULKAN_SRC_CORE_DIAGNOSTICS_ERRORREPORTER_H_

#include "ErrorReport.h"
#include "SoundErrorReporter.h"

class ErrorReporter : public SoundErrorReporter {
 public:
  void ReportOpenAudioDeviceFailed(ErrorReport report) override;
};

#endif  // VULKAN_SRC_CORE_DIAGNOSTICS_ERRORREPORTER_H_
