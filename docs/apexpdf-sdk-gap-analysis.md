# ApexPDF SDK gap analysis

Status: initial assessment for Slice 0; 2026-07-17.

## Decision

The existing editor remains the integration host and security boundary. The company-owned, standalone `ApexPdfSdk` repository becomes the production PDF core. Syncfusion and PDFium remain development references only while the new SDK is built and must be removed from production packages before Editor Integration Gate 1.

## Assets to retain

- AppContainer worker isolation, authenticated handle-only IPC, job limits, and restart behavior.
- Atomic handle-relative save publication, validation, rollback, and failure-injection harnesses.
- Engine-neutral managed contracts, bounded tile cache, render scheduling, and WinUI integration.
- Privacy-minimal diagnostics, DPAPI recovery, packaging, and clean-machine acceptance work.

## Material gaps against the master specification

| Area | Current state | Required action |
|---|---|---|
| Production PDF core | Syncfusion/PDFium provider | Implement owned parser, object model, writer, content interpreter, renderer, text extraction, and page engine. |
| C ABI | Native scaffold uses C++-only types in an `extern C` block | Adopt genuinely C-compilable opaque handles, fixed-width values, explicit calling convention, and versioned structs. |
| SDK lifecycle | Provider-dependent creation path | Provide real library lifecycle, allocator/logging/cancellation/progress/stream contracts. |
| Repository governance | Editor-oriented docs | Add SDK standards, dependencies, security, API, testing, release, and integration records. |
| Dependency policy | Proprietary SDK and complete reference engine ship today | Record development-only status and enforce a production-package exclusion check. |
| Native testing | Minimal native scaffold | Add C and C++ header compilation, unit/integration tests, sanitizers, fuzz harnesses, and ABI reports. |
| Managed package | Engine-neutral app abstractions only | Add stable .NET binding over safe handles without exposing native pointers. |
| Capability truthfulness | Provider capabilities dominate | Publish owned-SDK capability discovery with parsed/preserved/rendered/extracted/editable distinctions. |

## Migration constraint

The editor must continue to build during SDK construction. Provider removal is a gated integration change, not an early destructive rewrite. No existing capability may be represented as owned SDK functionality until its owned implementation and acceptance tests pass.
