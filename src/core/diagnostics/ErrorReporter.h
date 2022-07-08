#ifndef SRC_CORE_DIAGNOSTICS_ERRORREPORTER_H_
#define SRC_CORE_DIAGNOSTICS_ERRORREPORTER_H_

#include "ErrorReport.h"
#include "SoundErrorReporter.h"

class ErrorReporter : public SoundErrorReporter {
 public:
  void ReportOpenAudioDeviceFailed(ErrorReport report) override;
};

#endif  // SRC_CORE_DIAGNOSTICS_ERRORREPORTER_H_
