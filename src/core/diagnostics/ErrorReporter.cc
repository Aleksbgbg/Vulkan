#include "ErrorReporter.h"

#include "ErrorReport.h"
#include "core/strings/format.h"
#include "log.h"
#include "util/build_definition.h"

#ifdef DIAGNOSTICS
#define DEBUG_LOG(CONTENT) ImmediateLog(CONTENT)
#else
#define DEBUG_LOG(CONTENT)
#endif

void ErrorReporter::ReportOpenAudioDeviceFailed(const ErrorReport report) {
  DEBUG_LOG(
      Format("[", report.location,
             "] Could not open audio device. Skipping sound output. Reason: ",
             report.explanation));
}
