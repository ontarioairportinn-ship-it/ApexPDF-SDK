# ApexPDF SDK Slice 0 plan

Status: complete; exit gate closed 2026-07-17.

## Objective

Establish a reproducible, security-oriented commercial SDK foundation that can evolve without changing the editor-facing contracts or depending on a complete third-party PDF engine in production.

## Work packages

1. Native foundation: CMake presets, warning-clean x64 builds, stable C ABI, opaque handles, version/lifecycle APIs, symbol visibility, allocation and diagnostics contracts.
2. Governance: approved-dependency register, SBOM and notice generation, ABI/version policy, contribution policy, security response policy, and release checklist.
3. Standards: standards register, implementation matrix, errata register, and conservative conformance-claims policy.
4. Verification: C/C++ public-header compilation, unit tests, load/unload tests, leak checks, static analysis, sanitizer configurations, and initial fuzz target plumbing.
5. Integration seam: safe-handle .NET binding skeleton and an adapter path that can coexist with the current worker until each owned capability is proven.

## Slice 0 exit gate

Slice 0 is not complete until all master-prompt acceptance items have objective evidence: clean build, CI tests, sanitizer configuration, C and C++ header compilation, approved dependencies only, queryable version, and leak-free initialization/shutdown. Reports must record commands, toolchain versions, architecture, and unresolved findings.

## Current increment

- Added a standalone `ApexPdfSdk` C/C++ build target.
- Added a C-compatible ABI with fixed-width status codes, version discovery, size/version-negotiated initialization, opaque ownership, and deterministic shutdown.
- Added allocator, privacy-safe logging, cancellation, progress, and random-access stream contracts.
- Added CMake x64 Debug and Release presets.
- Added AddressSanitizer and MSVC native-analysis presets plus an independent SDK Windows CI workflow.
- Added C and C++ ABI smoke tests covering allocation balance, lifecycle events, cancellation, progress bounds, and stream bounds.
- Added initial ABI/ownership/threading documentation and an approved-dependency register.
- Added a standards/conformance register and an explicit no-claims implementation matrix.
- Added exact Windows DLL export auditing and a 10,000-iteration lifecycle/allocation-balance stress test.
- Added security, contribution, and immutable-release policies with a release checklist.
- Added packaged SPDX 2.3 SBOM and explicit third-party notices.
- Added a deterministic checked-in stream fuzz corpus replay harness and CI toolchain-evidence capture.
- Added isolated C and C++ consumers that configure, build, and execute solely from the packaged SDK ZIP.
- Recorded the integration/migration gap without claiming parser or rendering support.

## Next recommended increment

Begin the next planned slice only after its capability scope and acceptance tests are approved. Slice 0 establishes plumbing and makes no PDF-processing capability claim.

## Toolchain observation

With MSVC 19.51, additional test hosts that load the instrumented SDK DLL (the C++ header smoke test, lifecycle stress test, and corpus replay harness) fail during AddressSanitizer runtime shutdown. The instrumented C ABI lifecycle test passes and covers allocation, cancellation, stream, and shutdown behavior; the additional targets still compile with instrumentation and run under Debug and Release. This test-host limitation remains recorded and must be rechecked after a toolchain update rather than hidden or treated as SDK capability evidence.
