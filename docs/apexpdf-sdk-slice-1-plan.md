# ApexPDF SDK Slice 1 plan

Status: in progress.

## Objective

Produce the first editor-integration package without replacing the current PDF provider: a managed safe-handle binding that verifies native loading, SDK/ABI compatibility, deterministic lifecycle, and truthful owned-capability discovery.

## Capability boundary

Slice 1 reports `None`. It does not parse, preserve, render, extract, edit, write, or validate PDF data. The editor may use this package only as a startup/integration probe alongside its current isolated worker.

## Exit gate

- Native ABI 1.1 exposes stable capability flags and reports no unsupported capability.
- The managed package exposes no native pointer and owns library lifetime through `SafeHandle`.
- A net10.0 consumer builds and executes 1,000 create/dispose cycles against the packaged native DLL.
- The NuGet package contains the managed assembly and `runtimes/win-x64/native/ApexPdfSdk.dll`.
- CI publishes immutable native ZIP, NuGet, hashes, and integration evidence.
- The editor can consume the immutable package without changing its active provider.
