# Editor Integration Gate 0 handoff

Status: SDK package candidate ready for editor evaluation.

## Purpose

Gate 0 proves that the editor's existing isolated worker can load and dispose the company-owned SDK package, verify version compatibility, and query truthful capabilities. It does not replace the active PDF provider or route document bytes into ApexPDF SDK.

## Immutable inputs

- Managed package: `ApexPdf.Sdk.0.1.0.nupkg`
- Native package: `ApexPdfSdk-0.1.0-Windows-AMD64.zip`
- SDK version: `0.1.0`
- ABI version: `1.1.0`
- Required capability result: `None`
- Architecture: Windows x64

Use only the files and SHA-256 values in the generated handoff bundle. Do not reference the SDK source tree or mutable SDK build output from the editor repository.

## Editor integration constraints

1. Load `ApexPdf.Sdk` only inside `ApexPdf.PdfEngine.Worker`, never in the WinUI process.
2. Keep Syncfusion as the active `IPdfEngine` implementation for all document operations.
3. Create and dispose one `ApexPdfLibrary` during worker startup before accepting document work.
4. Require SDK version `0.1.0` and ABI major `1`; accept ABI minor `1` for this package.
5. Require `ApexPdfLibrary.OwnedCapabilities == ApexPdfCapabilities.None`.
6. Treat missing native assets, load failure, ABI mismatch, unexpected capabilities, or initialization failure as a typed worker-startup failure with content-neutral diagnostics.
7. Do not send paths, passwords, document data, extracted text, or PDF bytes to the SDK during Gate 0.
8. Removal of the package reference and startup probe must restore the prior worker behavior without data migration.

## Required editor evidence

- Locked restore succeeds using an immutable local/package feed artifact.
- Release solution build succeeds with zero warnings and errors.
- Full editor test suite passes.
- Worker startup probe passes inside the AppContainer deployment model.
- A 1,000-cycle create/dispose integration test passes without leaked handles or worker instability.
- Missing DLL and wrong-architecture tests fail closed with a stable diagnostic code.
- Simulated incompatible ABI and unexpected-capability results fail closed.
- Existing open, render, search, save, print, restart, and recovery tests remain on the current provider and pass unchanged.
- MSIX contains the managed assembly and exactly one matching x64 `ApexPdfSdk.dll` in the worker-loadable location.
- Clean-machine install/start/uninstall passes and produces no new network access or content-bearing diagnostics.

## Out of scope

PDF parsing, preservation, rendering, text extraction, editing, writing, encryption, signatures, validation, and provider replacement are explicitly out of scope.

## Feedback template

Return this block to the SDK chat:

```text
Editor Integration Gate 0 result: PASS | FAIL | BLOCKED
Editor commit:
SDK NuGet SHA-256:
Native ZIP SHA-256:
Restore/build/test commands:
Test totals:
Worker startup probe:
MSIX package inspection:
Clean-machine result:
Rollback test:
Observed SDK version / ABI / capabilities:
Diagnostics added (codes only):
SDK defects or requested changes:
Editor-only findings:
Unresolved risks:
```
