#ifndef SRC_CORE_DIAGNOSTICS_SOUNDERRORREPORTER_H_
#define SRC_CORE_DIAGNOSTICS_SOUNDERRORREPORTER_H_

#include "ErrorReport.h"

class SoundErrorReporter {
 public:
  virtual ~SoundErrorReporter() = default;

  virtual void ReportOpenAudioDeviceFailed(ErrorReport report) = 0;
};

#endif  // SRC_CORE_DIAGNOSTICS_SOUNDERRORREPORTER_H_
