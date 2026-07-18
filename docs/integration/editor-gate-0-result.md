# Editor Integration Gate 0 result

Result: PASS. Date: 2026-07-18.

## Revisions and artifacts

- SDK handoff commit: `5578f1f`
- Editor integration commit: `44c0bee`
- Managed SHA-256: `116C0C469ED8F9759650CBA82AC0EB1B5B7C5C7DF4E44907634C4B302D916D88`
- Native SHA-256: `AE58207CEACDC6F7A365101DF7E1FC72D8E007AD2BC192A6AFEB9905A0DD2469`
- Observed contract: SDK `0.1.0`, ABI `1.1.0`, owned capabilities `None`

## Editor evidence

- Locked restore and warning-clean Release solution build passed.
- Full suite passed: 99 tests, 0 failed, 0 skipped.
- The AppContainer worker completed the SDK startup probe, 1,000 create/dispose cycles, and a subsequent ping.
- MSIX inspection found exactly one `ApexPdf.Sdk.dll` and one x64 `ApexPdfSdk.dll` beneath `PdfWorker`.
- Clean-machine package `0.1.0.4` passed install, AppContainer/job verification, startup without an external license, and uninstall cleanup.
- Rollback to editor point `1760f4f` remained data-migration-free and had previously passed 89 tests.
- The editor worker protocol advanced to v7 to carry authenticated SDK compatibility metadata.
- The current document provider remained active; no PDF document data was routed to the SDK.

## Failure diagnostics accepted by the editor

- `worker.sdk.native_missing`
- `worker.sdk.architecture_mismatch`
- `worker.sdk.native_contract_mismatch`
- `worker.sdk.version_mismatch`
- `worker.sdk.abi_mismatch`
- `worker.sdk.capability_mismatch`
- `worker.sdk.initialization_failed`
- `worker.sdk.handshake_mismatch`

## Open risks

The managed and native SDK binaries are unsigned, Gate 0 is x64-only, and the SDK owns no PDF-processing capabilities. Digitally signed binaries and editor-side signature verification are required before production acceptance.
