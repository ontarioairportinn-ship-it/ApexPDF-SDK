# Contribution policy

Changes must preserve the C ABI rules, capability-truthfulness policy, privacy-minimal diagnostics, and approved-dependency boundary. Each change includes focused tests, updates affected public documentation, and records security or compatibility consequences. Public ABI additions require versioning review and export-allowlist updates. New dependencies require approval before their code is introduced.

Generated files, build output, proprietary standards text, customer documents, credentials, and reference-engine binaries are not committed. Reviewers must be independent of the author for security-sensitive parsers, writers, cryptography, memory ownership, and release automation.
