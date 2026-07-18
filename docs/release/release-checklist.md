# Release checklist

Status: Slice 0 baseline; 2026-07-17.

A release candidate is approved only when every applicable item has linked evidence and unresolved findings are explicitly accepted by the release owner.

- The version and ABI compatibility decision are reviewed.
- The repository is clean and the candidate commit is immutable and identifiable.
- Release and sanitizer configurations build; enabled tests pass on Windows x64.
- Public C and C++ consumers compile, lifecycle allocation balance passes, and exported symbols match the allowlist.
- Native analysis completes without an unreviewed warning.
- The approved-dependency register matches the source and package contents.
- The SPDX SBOM and third-party notices are present in the package and reviewed.
- Package contents contain no development-only PDF engine or reference implementation.
- Commands, CMake/MSVC versions, architecture, test results, package hash, and unresolved findings are retained as release evidence.
- Security, license, privacy, and conformance claims are reviewed.
- The ZIP is installed and exercised from a clean directory before publication.

Published artifacts are immutable. A correction receives a new semantic version; an existing package is never silently replaced.
