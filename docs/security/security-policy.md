# Security policy

Status: Slice 0 baseline; 2026-07-17.

Do not disclose suspected vulnerabilities in public issues. Report them through the private company security channel designated for ApexPDF releases. A complete report should identify the affected SDK version and architecture, reproduce the behavior with the smallest non-sensitive input possible, describe impact, and state whether exploit details are already public.

The response owner acknowledges a report, establishes severity and affected versions, preserves evidence, coordinates remediation and disclosure, and records the final disposition. Release artifacts are not described as fixed until the relevant tests pass and immutable replacement packages are published. Credentials, customer PDFs, passwords, document text, and proprietary proof-of-concept files must not be placed in source control, CI logs, crash reports, or ordinary diagnostics.

Every production dependency requires the review recorded in the approved-dependency register. Security-sensitive parsing and cryptography work requires threat modeling, bounded resource behavior, hostile-input tests, fuzzing, and independent review before a capability claim is enabled.
