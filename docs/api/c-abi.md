# ApexPDF SDK C ABI

Status: Slice 0 foundation; ABI 1.0.

## Compatibility

The C ABI uses fixed-width scalar types, opaque handles, explicit `__cdecl` on Windows, and size-versioned option structures. Callers must initialize every structure to zero, set `struct_size`, and request an ABI version no newer than the version returned by `apx_abi_version`.

No C++ standard-library type, exception, RTTI object, or native pointer owned by an implementation subsystem crosses the ABI. Status values are stable once released; new values may be appended but existing numeric meanings must not change.

## Library ownership

`apx_library_create` creates one application-level SDK instance. The matching `apx_library_destroy` releases it. Every child handle must be destroyed before its parent library. Destroy functions accept null. Other functions reject invalid required pointers with `APX_STATUS_INVALID_ARGUMENT`.

If a caller supplies an allocator, both allocation callbacks are mandatory and remain valid for the library lifetime. Memory is released through the same allocator that created it. The default allocator is private to the SDK.

## Logging and privacy

The log callback is synchronous and receives a stable numeric event ID, severity, and content-neutral English message. The SDK must not place document text, passwords, file paths, extracted metadata, or raw PDF bytes in default log messages. Callbacks must return promptly and must not call destruction functions for the object producing the callback.

## Cancellation and progress

A cancellation source is an opaque child of the library. `apx_cancel_source_request` and `apx_cancel_source_is_requested` are safe to use concurrently. Cancellation is cooperative; operations check at documented bounded intervals and return `APX_STATUS_CANCELLED` without claiming success.

Progress callbacks are synchronous on the thread performing the operation. `completed` never exceeds `total`, and `total` is nonzero. A callback may request cancellation but must not destroy handles participating in the active call.

## Random-access streams

The stream contract is borrowed: the SDK does not own the callback structure or its `user_data`. Both remain valid for the complete operation using them. Reads are position-independent and do not mutate a shared cursor. A successful short read is permitted at end of input. Callback failures must use a stable status and must not throw across the ABI.

The SDK may issue reads from worker threads in later slices. A stream shared across simultaneous operations must therefore make its callbacks and backing storage thread-safe.

## Current capability boundary

These APIs establish lifecycle and operation plumbing only. ABI 1.0 does not yet claim PDF parsing, rendering, text extraction, editing, encryption, validation, or save support.
