# ApexPDF SDK Slice 2 plan

Status: planned; implementation not started.

## Objective

Implement the first owned PDF capability as a bounded, fail-closed structural parser and validator over the existing borrowed random-access stream ABI. Slice 2 establishes whether a PDF's byte-level structure can be safely indexed and traversed within an explicit supported subset. It does not render, interpret page content, extract text, edit, repair, write, decrypt, verify signatures, or claim ISO/PDF profile conformance.

No editor artifact may be published until every Slice 2 exit gate has objective evidence.

## Exact capability scope

Slice 2 may eventually advertise `APX_CAPABILITY_PARSE | APX_CAPABILITY_VALIDATE` only with the following meanings:

- `PARSE`: create an immutable structural document index for the supported syntax and cross-reference forms.
- `VALIDATE`: evaluate the structural rules listed here and return stable diagnostics. It does not mean full PDF, PDF/A, PDF/UA, PDF/X, security, signature, accessibility, or rendering conformance.

The implementation scope is limited to:

- `%PDF-1.x` headers through PDF 1.7 and `%PDF-2.0` headers.
- `startxref`, `%%EOF`, classic cross-reference tables, cross-reference streams, hybrid references, and bounded incremental-update `/Prev` chains.
- Indirect objects and generations; null, Boolean, integer, bounded real, name, literal string, hexadecimal string, array, dictionary, reference, and stream syntax.
- Trailer discovery and bounded validation of `/Root`, `/Info`, `/ID`, `/Encrypt`, `/Size`, `/Prev`, and `/XRefStm` types and references.
- Bounded catalog and page-tree structural traversal sufficient to verify node types, parent/child relationships, cycles, counts, and referenced object existence. Page contents are opaque.
- Flate decoding only where required for cross-reference streams and object streams. No general document-stream decoding or content interpretation is exposed.
- Object streams sufficient to locate embedded indirect objects, with bounded object count, offsets, and decompressed bytes.
- Detection of linearized files without claiming linearization correctness or optimized access.

Explicitly out of scope:

- Repair, fallback scanning for missing cross-reference data, heuristic recovery, or accepting a partially indexed document as valid.
- Page content operators, graphics, colors, images, fonts, text, annotations, forms, metadata semantics, embedded files, multimedia, optional content, actions, JavaScript, signatures, permissions, or redaction semantics.
- Rendering, extraction, editing, writing, incremental save, optimization, linearization, encryption/decryption, password handling, or provider replacement.
- Any standards-conformance claim beyond the named structural checks.

## Parsing and result model

Parsing is deterministic and fail closed. Success returns one immutable `apx_document` plus a report that may contain warnings. Failure returns no document and a bounded report with at least one error diagnostic. Callers never receive a partially usable document.

Validation is performed during parse; a separate report query exposes the results. A structurally parsed document may contain warnings but no structural errors. Unknown keys and unsupported feature dictionaries are preserved as opaque indexed objects and do not become capability claims.

The parser must not require a seekable OS file, path, shared cursor, or direct native handle. All bytes arrive through `apx_random_access_stream` callbacks.

## Malformed and encrypted behavior

- Malformed syntax, invalid offsets, cross-reference conflicts, cycles, illegal object-stream entries, premature EOF, decompression-limit violations, and resource-limit violations fail closed with no document.
- Trailing bytes after the final accepted `%%EOF` are diagnosed. Policy determines whether bounded whitespace/comments are accepted; nontrivial trailing data is an error in Slice 2.
- Multiple incremental revisions are accepted only when the complete bounded chain is internally consistent. The parser never silently selects a convenient revision after a broken newer revision.
- Repairable PDFs are still rejected. Slice 2 has no repair mode.
- Presence of a trailer `/Encrypt` entry returns `APX_STATUS_ENCRYPTED` and diagnostic `APX_DIAG_ENCRYPTED_UNSUPPORTED`. No password parameter is accepted, no encryption dictionary is interpreted beyond safe structural detection, and no document handle is returned.
- Unsupported filters needed to decode cross-reference or object streams return `APX_STATUS_UNSUPPORTED_FEATURE`, not `MALFORMED`.
- Callback I/O failures remain `APX_STATUS_IO_ERROR`; cancellation remains `APX_STATUS_CANCELLED`; limit exhaustion remains `APX_STATUS_LIMIT_EXCEEDED`.

## Cancellation and threading

- Parsing is synchronous on the calling thread in Slice 2.
- The parser checks cancellation before and after every stream callback, before each indirect-object parse, during every page-tree and `/Prev` traversal, and at least every 64 KiB of tokenization or decompression.
- After cancellation, no new stream callback may begin. The call releases all owned temporary memory, returns no document, and emits no success diagnostic.
- A borrowed stream and operation context remain valid for the complete call. The parser retains neither.
- An immutable returned document supports concurrent read-only summary and diagnostic queries. Destruction requires external exclusion from simultaneous calls.

## Memory and allocation requirements

- Every allocation uses the library allocator selected at `apx_library_create`; there is no hidden process-global parser heap.
- Allocation overflow, size multiplication, and offset arithmetic are checked before allocation or read.
- The parser enforces both caller-selected limits and non-overridable hard ceilings from `docs/security/parser-resource-limits.md`.
- Temporary decompression buffers count against the same memory budget as indexes and diagnostics.
- Failure, cancellation, and every diagnostic path must return allocator balance to zero after report/document destruction.
- Input bytes and strings are not copied unless required for bounded token or index ownership. Slice 2 exposes no document string content through the public ABI.

## Stable diagnostics

Diagnostics are numeric, append-only, content-neutral, and defined in `docs/api/abi-1.2-parser-proposal.md`. A diagnostic contains severity, stable code, byte offset when known, object number/generation when known, and revision index when known. It never contains file paths, passwords, document strings, names, metadata values, or raw bytes.

Reports are capped and record whether additional diagnostics were suppressed. Diagnostic order is deterministic: revision, byte offset, object number, then code.

## Fuzzing and hostile-corpus gates

- libFuzzer-compatible targets for tokenizer, cross-reference parser, object parser, object-stream decoder, page-tree traversal, and whole-document parse.
- Seed corpora cover every supported cross-reference form, incremental revisions, object streams, empty/large containers, boundary numeric values, malformed delimiters, cycles, truncations, unsupported filters, encryption markers, and limit edges.
- Dictionary-based mutation includes PDF delimiters, structural keywords, trailer keys, filters, and object-stream keys.
- Continuous sanitizer fuzzing has no crash, hang, leak, allocator imbalance, undefined behavior, or unbounded-growth finding.
- Before exit, each target completes at least 24 cumulative sanitizer-hours and 10 million executions, whichever takes longer; whole-document parsing completes at least 48 cumulative sanitizer-hours.
- A checksum-locked hostile corpus includes malformed public samples, generated adversarial files, decompression bombs, deep nesting, cross-reference cycles/conflicts, overlapping objects, extreme sparse offsets, and incremental-update attacks.
- Corpus results are deterministic across two clean runs and record toolchain, architecture, limits, elapsed time, peak memory, status, and diagnostic codes without content.
- Every fixed parser defect adds a minimized regression input unless licensing or sensitivity prohibits storage; prohibited inputs receive a deterministic generator and hash record.

## Package and version plan

- Product package version advances from `0.1.0` to `0.2.0` only when Slice 2 exits.
- ABI advances from `1.1` to `1.2`; ABI major remains `1` because additions are append-only.
- During implementation, capability discovery continues to return `None`. The final evidence commit may enable only `PARSE | VALIDATE` with the narrow semantics above.
- Native ZIP and managed NuGet versions remain identical.
- Packages include an updated SPDX SBOM, notices, ABI/export report, capability manifest, limits manifest, diagnostics registry, known-limitations manifest, fuzz/corpus summary, security evidence, hashes, and release notes.
- No editor handoff artifact is assembled or uploaded before the complete exit audit passes.

## Slice 2 exit gate

- Warning-clean Release and Debug builds with exact ABI export audit.
- C and C++ ABI consumers plus managed safe-handle binding tests.
- Unit and integration tests for every supported syntax form, diagnostic, limit, cancellation boundary, allocator failure, and ownership path.
- Sanitizer, native analysis, fuzz-duration/execution, hostile-corpus, deterministic-result, and leak gates pass.
- Package-consumer tests operate only from extracted ZIP/NuGet artifacts.
- SBOM, notices, package contents, hashes, version alignment, capability manifest, and absence of unapproved production dependencies are verified.
- Security review confirms bounded time/memory/recursion, checked arithmetic, no repair ambiguity, privacy-safe diagnostics, and fail-closed encrypted/malformed handling.
- The exit audit records commands, toolchain versions, architecture, corpus hashes, findings, suppressions, and residual risks.

Only after those gates pass may the shadow-validation handoff in `docs/integration/editor-shadow-validation-handoff-draft.md` be finalized and published.
