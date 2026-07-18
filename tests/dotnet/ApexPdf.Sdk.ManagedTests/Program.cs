using ApexPdf.Sdk;

if (ApexPdfLibrary.SdkVersion != new ApexPdfVersion(0, 1, 0))
{
    throw new InvalidOperationException($"Unexpected SDK version: {ApexPdfLibrary.SdkVersion}");
}

if (ApexPdfLibrary.AbiVersion != new ApexPdfVersion(1, 1, 0))
{
    throw new InvalidOperationException($"Unexpected ABI version: {ApexPdfLibrary.AbiVersion}");
}

if (ApexPdfLibrary.OwnedCapabilities != ApexPdfCapabilities.None)
{
    throw new InvalidOperationException("Slice 1 binding must not claim PDF-processing capabilities.");
}

for (int iteration = 0; iteration < 1_000; iteration++)
{
    using ApexPdfLibrary library = ApexPdfLibrary.Create();
}
