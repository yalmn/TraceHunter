#ifndef TRACEHUNTER_API_H
#define TRACEHUNTER_API_H

#include "../core/analyzer.h"
#include "../report/reporter.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Callback prototype for custom AI analysis. The callback receives the path
 * to a file extracted by TraceHunter as well as the prompt from the scope.
 * The implementor can return 0 on success or non-zero on error.
 */
typedef int (*th_ai_callback)(const char *file_path, const char *prompt);

/** Register a callback which is invoked for each file match. */
void th_set_ai_callback(th_ai_callback cb);

/** Run the analysis using an existing scope JSON file. */
int th_run_analysis(const char *image_path, const char *scope_json);

#ifdef __cplusplus
}
#endif

#endif /* TRACEHUNTER_API_H */
