# ApexPdf.Sdk

This package provides safe managed access to the ApexPDF SDK lifecycle, version, and owned-capability discovery APIs. Version 0.1.0 intentionally reports no PDF-processing capabilities and must coexist with the editor's current document provider.

Create and dispose an `ApexPdfLibrary` to validate native loading and lifecycle ownership. Do not infer parsing, rendering, extraction, editing, writing, or validation support unless the corresponding `OwnedCapabilities` flag is present.
