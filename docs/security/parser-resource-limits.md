# Slice 2 parser resource limits

Status: proposed contract; implementation not started.

Every limit has a secure default and a non-overridable hard ceiling. Callers may lower values. Raising a default requires an explicit options structure but can never exceed the ceiling. Zero requests the default, not unlimited operation.

| Resource | Secure default | Hard ceiling |
|---|---:|---:|
| Input length | 512 MiB | 2 GiB |
| Total SDK-owned parser memory | 256 MiB | 1 GiB |
| Indirect objects across revisions | 250,000 | 2,000,000 |
| Cross-reference entries | 500,000 | 4,000,000 |
| Incremental revisions and `/Prev` depth | 64 | 256 |
| Syntactic container nesting | 64 | 256 |
| Indirect-reference traversal depth | 64 | 256 |
| Page-tree depth | 64 | 256 |
| Array elements per array | 100,000 | 1,000,000 |
| Dictionary entries per dictionary | 16,384 | 65,536 |
| Name token bytes after decoding escapes | 1 KiB | 4 KiB |
| Literal/hex string token bytes | 16 MiB | 64 MiB |
| Numeric token bytes | 128 | 1 KiB |
| General keyword token bytes | 1 KiB | 4 KiB |
| Object stream embedded objects | 100,000 | 1,000,000 |
| Decompressed bytes per required structural stream | 64 MiB | 256 MiB |
| Total decompressed structural bytes | 256 MiB | 1 GiB |
| Compression expansion ratio | 200:1 | 1,000:1 |
| Random-access callback count | 1,000,000 | 8,000,000 |
| Total bytes requested through callbacks | 4 GiB | 16 GiB |
| Diagnostics retained | 256 | 4,096 |

Additional invariants:

- The hard input ceiling is fixed at the signed 32-bit boundary, but all offsets and lengths remain checked `uint64_t` values and never rely on signed truncation.
- Duplicate references and revisits do not reset depth, work, callback, decompression, or memory budgets.
- Failed allocations and allocator-injected failures are terminal and deterministic.
- A single logical read is capped at 1 MiB; larger regions are chunked with cancellation checks.
- Parser work uses a monotonically decreasing budget derived from tokens, objects, references, decompressed bytes, and callbacks. The implementation must not rely on wall-clock time for safety.
- Hitting any caller or hard limit returns `APX_STATUS_LIMIT_EXCEEDED` and a resource-specific diagnostic without partial success.
