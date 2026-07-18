# ApexPDF SDK Slice 1 plan

Status: complete; Editor Integration Gate 0 passed 2026-07-18.

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

## Exit evidence

Editor commit `44c0bee` consumed the immutable Gate 0 bundle and passed locked restore, warning-clean Release build, 99 tests, AppContainer worker startup, 1,000 managed create/dispose cycles, MSIX inspection, clean-machine install/start/uninstall, and rollback validation. The worker observed SDK `0.1.0`, ABI `1.1.0`, and capabilities `None`; the active PDF provider was unchanged.

The managed package SHA-256 was `116C0C469ED8F9759650CBA82AC0EB1B5B7C5C7DF4E44907634C4B302D916D88`. The native ZIP SHA-256 was `AE58207CEACDC6F7A365101DF7E1FC72D8E007AD2BC192A6AFEB9905A0DD2469`.

## Remaining production gates

Gate 0 is an integration-stage developer package, not a production release. Managed and native SDK binaries remain unsigned, only x64 is accepted, and the SDK owns no PDF-processing capability. Digital signing and signature verification are required before production acceptance.
