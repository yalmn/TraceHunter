# TraceHunter

TraceHunter is a prototype forensic analysis tool written in C. It depends on OpenSSL, Jansson and The Sleuth Kit (TSK).

## Building

Use `make` to build the project. The `Makefile` automatically uses `pkg-config` to find dependencies on Linux and macOS. If `pkg-config` is not available, it falls back to common Homebrew paths on macOS.

```
make
```

The resulting binary will be `bin/tracehunter`.
