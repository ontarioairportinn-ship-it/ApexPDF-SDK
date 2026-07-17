# Approved dependencies

Status: Slice 0 initial register.

## Production runtime

The current ApexPDF SDK foundation has no third-party production runtime dependency. It uses the Microsoft C/C++ runtime and Windows platform toolchain supplied under the product's eventual deployment and redistribution policy.

## Build and test

| Component | Purpose | Distribution | Approval state |
|---|---|---|---|
| CMake 3.28 or newer | Native configuration and generation | Build only | Approved for Slice 0 |
| Ninja | Local and CI native build execution | Build only | Approved for Slice 0 |
| MSVC 19.51 / Visual Studio 2026 toolchain | Windows x64 compilation | Build only | Approved for current development baseline |
| CTest | Native test orchestration | Build/test only | Approved for Slice 0 |

Complete PDF engines, including Syncfusion PDF, PDFium, MuPDF, Poppler, and Ghostscript, are not approved production dependencies of `ApexPdfSdk`. Existing editor-provider binaries remain outside this SDK subtree during migration and may be used only as development references and compatibility oracles until removed from the Gate 1 package.

Any new dependency requires recorded version, source, license, commercial-redistribution review, offline behavior, isolation boundary, replacement strategy, SBOM inclusion, notices, and vulnerability review before code may depend on it.
