# ApexPDF SDK

`ApexPdfSdk` is the standalone, company-owned PDF SDK repository. It is built,
tested, versioned, packaged, and released independently from the ApexPDF Windows
editor. The editor may consume only published, immutable, versioned SDK
artifacts; it must not copy this repository's implementation source or use its
private interfaces.

## Current status

Slice 0 is in progress. The repository currently provides the native library
foundation, a versioned C ABI, public headers, and C/C++ ABI smoke tests. It does
not yet claim PDF parsing, rendering, editing, security, redaction, OCR, or
validation capability.

## Local validation

Run these commands from an x64 Visual Studio developer shell:

```powershell
cmake --preset windows-x64-release
cmake --build --preset windows-x64-release
ctest --preset windows-x64-release
cpack --preset windows-x64-release
```

Build and package outputs remain under `artifacts/` inside this repository.

## Versioning

The product version follows semantic versioning and is currently declared by
the CMake project. The public native ABI version is tracked separately in
`include/apexpdf/apexpdf_version.h`.

See `docs/apexpdf-sdk-slice-0-plan.md` and
`docs/apexpdf-sdk-gap-analysis.md` for the staged implementation plan and known
limitations. Standards and conformance claims are governed by
`docs/standards/standards-register.md`.

Packaged releases include an SPDX SBOM and third-party notices. Release and
security requirements are recorded under `docs/release/` and `docs/security/`.
The current exit-gate evidence and unresolved toolchain limitation are recorded
in `docs/release/slice-0-exit-audit.md`.
