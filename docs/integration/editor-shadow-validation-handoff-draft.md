# Editor shadow-validation handoff draft

Status: draft only; must not be packaged or sent to the editor before Slice 2 exit.

## Intended experiment

The existing provider remains authoritative. For an explicitly selected, non-sensitive test corpus, the AppContainer worker may invoke the owned SDK structural parser against the already brokered random-access source handle and record only status, stable diagnostic codes, bounded counts, duration bucket, and peak-memory bucket. No user-visible open decision or document behavior depends on the shadow result.

## Preconditions

- Slice 2 exit audit is complete and capabilities truthfully report `PARSE | VALIDATE` with the narrow structural semantics.
- Signed immutable native and managed packages, hashes, SBOM, notices, limits/capability manifests, fuzz summary, security evidence, and known limitations are available.
- The editor verifies package signatures, version, ABI, architecture, capability bits, and authenticated protocol metadata before shadow work.

## Integration constraints

- Worker/AppContainer only; never load parser code in the UI or broker process.
- Disabled by default outside controlled development/acceptance builds.
- No provider replacement, fallback, repair, render, extraction, edit, save, or validation authority.
- No paths, document text, metadata values, object strings, passwords, raw bytes, or unrestricted offsets in diagnostics or telemetry.
- Encrypted documents return the stable unsupported result without prompting or sharing a password.
- Apply the secure default limits; editor configuration may only lower them.
- Cancellation, worker watchdog, job memory, and restart behavior remain authoritative and are tested.

## Required comparison evidence

- Outcome matrix for SDK success/malformed/encrypted/unsupported/limit/cancel/I/O versus the current provider's open/validation outcome.
- Zero crashes, hangs, worker restarts, leaked handles, job-limit breaches, or privacy violations across the checksum-locked corpus.
- Deterministic SDK status and diagnostic codes across two clean runs.
- False-accept review: every SDK structural success/provider rejection is manually classified before any future authority proposal.
- False-reject review: every SDK rejection/provider success is minimized and assigned to SDK defect, intentional scope limit, or provider permissiveness.
- Existing editor behavior and full tests remain unchanged with shadow validation enabled and disabled.

## Feedback contract

The final handoff will request editor commit, package hashes/signatures, protocol version, commands, test totals, corpus hashes/counts, outcome matrix, peak worker memory, cancellation/watchdog results, diagnostic-code frequencies, minimized discrepancies, rollback result, and unresolved risks.
