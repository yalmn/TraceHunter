# TraceHunter

TraceHunter is a prototype forensic analysis tool written in C. It depends on OpenSSL, Jansson and The Sleuth Kit (TSK).

## Building

Use `make` to build the project. The `Makefile` automatically uses `pkg-config` to find dependencies on Linux and macOS. If `pkg-config` is not available, it falls back to common Homebrew paths on macOS.

```
make
```

The resulting binary will be `bin/tracehunter`.

## API usage

Developers can integrate TraceHunter into their own tools via the new C API. The
header `src/api/tracehunter_api.h` exposes a minimal interface:

```c
#include "src/api/tracehunter_api.h"

// optional: callback for custom AI analysis
int my_ai(const char *file, const char *prompt) {
    /* analyse file */
    return 0;
}

th_set_ai_callback(my_ai);
th_run_analysis("disk.dd", "scope.json");
```

The callback is invoked for each matched file and receives the file path and the
prompt defined in the scope.
