# Standards and conformance register

Status: Slice 0 governance baseline; 2026-07-17.

## Claim policy

ApexPDF uses standards as implementation requirements, not marketing claims. A feature may be described as conforming only when its normative requirements are mapped to owned implementation code, positive and negative tests, and reviewed evidence. Partial parsing, preservation, rendering, or validation must be named precisely and must not be presented as full conformance.

Development references and compatibility oracles do not establish conformance. Ambiguous requirements are resolved conservatively, recorded in the errata register, and tested before a claim changes.

## Register

| Standard or specification | Intended SDK area | Slice 0 state | Claim permitted |
|---|---|---|---|
| ISO 32000-1:2008 (PDF 1.7) | Parser, object model, writer, rendering | Registered; not implemented | No |
| ISO 32000-2:2020 (PDF 2.0) and published errata | Parser, object model, writer, rendering | Registered; not implemented | No |
| ISO 19005 family (PDF/A) | Validation and archival output | Future scope; not implemented | No |
| ISO 14289 family (PDF/UA) | Accessibility validation and authoring | Future scope; not implemented | No |
| ISO 15930 family (PDF/X) | Prepress validation and output | Future scope; not implemented | No |
| FIPS 197 and NIST cryptographic guidance | Encryption primitives and approved use | Requirements pending security design | No |

Normative documents must be obtained through licensed or authorized sources. Their text is not copied into this repository.

## Implementation matrix

| Capability | Parse | Preserve | Render | Extract | Edit/write | Validate |
|---|---:|---:|---:|---:|---:|---:|
| Library lifecycle and ABI plumbing | N/A | N/A | N/A | N/A | N/A | Tested |
| Owned capability discovery | N/A | N/A | N/A | N/A | N/A | Tested; reports none |
| PDF file structure | No | No | No | No | No | No |
| Page content and graphics | No | No | No | No | No | No |
| Fonts and text | No | No | No | No | No | No |
| Encryption and signatures | No | No | No | No | No | No |
| Metadata, tagging, and accessibility | No | No | No | No | No | No |

`No` means no owned production capability is claimed. This matrix is updated only with linked acceptance evidence.

## Errata and interpretations

No standards errata or disputed interpretations have yet been adopted by the implementation. Future entries must identify the affected standard and clause, source, engineering interpretation, security and compatibility effect, tests, reviewer, and adoption date.
