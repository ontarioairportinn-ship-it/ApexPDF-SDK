# ABI 1.2 structural parser proposal

Status: design proposal; not exported or claimed.

## New status values

Existing numeric values remain unchanged. Proposed append-only values:

| Name | Value | Meaning |
|---|---:|---|
| `APX_STATUS_MALFORMED` | 10 | Structural PDF error |
| `APX_STATUS_ENCRYPTED` | 11 | Encryption detected; unsupported in Slice 2 |
| `APX_STATUS_UNSUPPORTED_FEATURE` | 12 | Required structural feature/filter is outside Slice 2 |
| `APX_STATUS_LIMIT_EXCEEDED` | 13 | Caller limit or hard ceiling reached |

## New opaque handles and structures

- `apx_document`: immutable successfully parsed structural index.
- `apx_parse_report`: immutable bounded diagnostics and summary for success or failure.
- `apx_parse_options`: size-versioned ABI request plus every caller-adjustable resource limit.
- `apx_document_summary`: size-versioned PDF header version, revision count, indirect-object count, page count when structurally known, cross-reference kind flags, linearization marker, and encryption marker. It exposes no document strings.
- `apx_diagnostic`: size-versioned severity, stable code, byte offset, object number, generation, revision, and flags indicating which locations are known.

## Proposed functions

```c
APX_API apx_status APX_CALL apx_document_open(
    apx_library* library,
    const apx_random_access_stream* stream,
    const apx_parse_options* options,
    const apx_operation_context* operation,
    apx_document** document,
    apx_parse_report** report);

APX_API void APX_CALL apx_document_destroy(apx_document* document);
APX_API void APX_CALL apx_parse_report_destroy(apx_parse_report* report);

APX_API apx_status APX_CALL apx_document_get_summary(
    const apx_document* document,
    apx_document_summary* summary);

APX_API apx_status APX_CALL apx_parse_report_get_summary(
    const apx_parse_report* report,
    apx_parse_report_summary* summary);

APX_API apx_status APX_CALL apx_parse_report_get_diagnostic(
    const apx_parse_report* report,
    uint32_t index,
    apx_diagnostic* diagnostic);
```

`apx_document_open` initializes both outputs to null before work. On success it returns a document and report. On malformed, encrypted, unsupported, limit, cancellation, I/O, or allocation failure it returns no document and returns a report when sufficient memory exists. The report is owned independently from the document. Destroy functions accept null.

## Stable diagnostic codes

Numeric assignments are append-only. Ranges reserve ownership by subsystem.

| Code | Symbol | Severity |
|---:|---|---|
| 2000 | `APX_DIAG_HEADER_MISSING` | Error |
| 2001 | `APX_DIAG_HEADER_UNSUPPORTED_VERSION` | Error |
| 2002 | `APX_DIAG_STARTXREF_MISSING` | Error |
| 2003 | `APX_DIAG_STARTXREF_INVALID` | Error |
| 2004 | `APX_DIAG_EOF_MISSING` | Error |
| 2005 | `APX_DIAG_TRAILING_DATA` | Error |
| 2010 | `APX_DIAG_XREF_MALFORMED` | Error |
| 2011 | `APX_DIAG_XREF_OFFSET_OUT_OF_RANGE` | Error |
| 2012 | `APX_DIAG_XREF_ENTRY_CONFLICT` | Error |
| 2013 | `APX_DIAG_XREF_STREAM_INVALID` | Error |
| 2014 | `APX_DIAG_XREF_PREV_CYCLE` | Error |
| 2015 | `APX_DIAG_XREF_HYBRID_CONFLICT` | Error |
| 2020 | `APX_DIAG_OBJECT_MALFORMED` | Error |
| 2021 | `APX_DIAG_OBJECT_NUMBER_MISMATCH` | Error |
| 2022 | `APX_DIAG_OBJECT_MISSING` | Error |
| 2023 | `APX_DIAG_REFERENCE_INVALID` | Error |
| 2024 | `APX_DIAG_STREAM_LENGTH_INVALID` | Error |
| 2025 | `APX_DIAG_OBJECT_STREAM_INVALID` | Error |
| 2030 | `APX_DIAG_TRAILER_INVALID` | Error |
| 2031 | `APX_DIAG_ROOT_MISSING` | Error |
| 2032 | `APX_DIAG_ROOT_INVALID` | Error |
| 2040 | `APX_DIAG_PAGE_TREE_INVALID` | Error |
| 2041 | `APX_DIAG_PAGE_TREE_CYCLE` | Error |
| 2042 | `APX_DIAG_PAGE_COUNT_MISMATCH` | Error |
| 2050 | `APX_DIAG_ENCRYPTED_UNSUPPORTED` | Error |
| 2051 | `APX_DIAG_FILTER_UNSUPPORTED` | Error |
| 2060 | `APX_DIAG_LIMIT_INPUT` | Error |
| 2061 | `APX_DIAG_LIMIT_MEMORY` | Error |
| 2062 | `APX_DIAG_LIMIT_OBJECTS` | Error |
| 2063 | `APX_DIAG_LIMIT_RECURSION` | Error |
| 2064 | `APX_DIAG_LIMIT_DECOMPRESSION` | Error |
| 2065 | `APX_DIAG_LIMIT_WORK` | Error |
| 2070 | `APX_DIAG_LINEARIZATION_UNVERIFIED` | Warning |
| 2071 | `APX_DIAG_DIAGNOSTICS_SUPPRESSED` | Warning |

Diagnostics describe structure only. They contain no free-form document-derived message. Managed bindings map numeric codes to SDK-owned constant names without changing values.

## ABI compatibility requirements

- `apx_parse_options`, summaries, and diagnostics use `struct_size`; callers zero all fields and set size.
- New fields append only. ABI 1.2 never reads beyond the caller's declared size.
- Handles never expose implementation pointers to managed callers; the managed package uses `SafeHandle` for document and report ownership.
- All public functions catch internal exceptions and return stable status values.
- The export allowlist and C/C++ header consumers must be updated atomically with implementation.
