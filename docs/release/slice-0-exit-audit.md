# Slice 0 exit audit

Status: implementation complete; CI confirmation pending.

Audit date: 2026-07-17. Architecture: Windows x64. Local toolchain: CMake 4.3.1-msvc1, Ninja, MSVC 19.51.36248.0, Visual Studio Community 2026 18.7.3.

## Acceptance evidence

| Gate | Evidence | Result |
|---|---|---|
| Clean Release configuration and warning-clean build | `cmake --preset windows-x64-release`; `cmake --build --preset windows-x64-release` | Pass |
| C and C++ public-header compilation | `ApexPdfSdk.AbiTests`, `ApexPdfSdk.CppHeaderTests`, isolated package consumers | Pass |
| Queryable product and ABI versions | ABI and package-consumer tests | Pass |
| Initialization/shutdown allocation balance | ABI test and 10,000-iteration lifecycle stress test | Pass |
| Exact public export surface | `ApexPdfSdk.ExportedSymbols` checks 13 approved exports | Pass |
| Sanitizer configuration | ASan build plus instrumented ABI lifecycle test | Pass with toolchain limitation below |
| Static analysis | `cmake --preset windows-x64-analyze`; analysis build | Pass |
| Initial hostile-input/fuzz plumbing | Three-seed deterministic stream corpus replay | Pass |
| Approved production dependencies only | Register, source review, package content inspection | Pass; no third-party runtime dependency |
| SBOM and notices | SPDX 2.3 JSON validation and ZIP content inspection | Pass |
| Independent package consumption | ZIP extraction, isolated C/C++ configure, build, and run | Pass locally; CI confirmation pending |
| CI execution | `.github/workflows/ci.yml` Release, ASan, analysis, package, evidence jobs | Pending first remote run |

## Commands and artifacts

The Release ZIP is `artifacts/windows-x64-release/ApexPdfSdk-0.1.0-Windows-AMD64.zip`. Local package hash is recorded at validation time and CI emits `artifacts/evidence/package-sha256.txt`. CI also emits `toolchain.txt` with architecture and commands.

The clean consumer gate is:

```powershell
./tests/Test-PackageConsumer.ps1 `
  -Package artifacts/windows-x64-release/ApexPdfSdk-0.1.0-Windows-AMD64.zip `
  -WorkDirectory artifacts/package-consumer
```

## Unresolved finding

MSVC 19.51 has a repeatable AddressSanitizer runtime-shutdown failure for additional executables loading the instrumented DLL. The instrumented C ABI lifecycle test passes. The C++ header, stress, and corpus replay targets compile with ASan but are disabled during ASan CTest; all run in Release. Recheck this limitation after a toolchain update. It prevents claiming complete multi-host ASan execution but does not hide a failing SDK assertion or allocation-balance test.

## Decision

The local Slice 0 implementation and package-consumer gates are complete. Slice 0 must remain marked `in progress` until the workflow runs successfully from a candidate commit and its evidence artifacts are retained. No PDF parsing, rendering, editing, security, validation, or conformance capability is claimed.
