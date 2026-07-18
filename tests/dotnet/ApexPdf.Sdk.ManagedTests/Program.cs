using ApexPdf.Sdk;
using System.Runtime.InteropServices;

string? diagnosticPath = Environment.GetEnvironmentVariable("APX_MANAGED_TEST_LOG");
void Mark(string message)
{
    if (diagnosticPath is not null)
    {
        File.AppendAllText(diagnosticPath, $"{DateTimeOffset.UtcNow:O} {message}{Environment.NewLine}");
    }
}

try
{
    Mark($"start framework={Environment.Version} architecture={RuntimeInformation.ProcessArchitecture}");
    ApexPdfVersion sdkVersion = ApexPdfLibrary.SdkVersion;
    Mark($"sdk-version={sdkVersion}");
    if (sdkVersion != new ApexPdfVersion(0, 1, 0))
    {
        throw new InvalidOperationException($"Unexpected SDK version: {sdkVersion}");
    }

    ApexPdfVersion abiVersion = ApexPdfLibrary.AbiVersion;
    Mark($"abi-version={abiVersion}");
    if (abiVersion != new ApexPdfVersion(1, 1, 0))
    {
        throw new InvalidOperationException($"Unexpected ABI version: {abiVersion}");
    }

    ApexPdfCapabilities capabilities = ApexPdfLibrary.OwnedCapabilities;
    Mark($"capabilities={capabilities}");
    if (capabilities != ApexPdfCapabilities.None)
    {
        throw new InvalidOperationException("Slice 1 binding must not claim PDF-processing capabilities.");
    }

    for (int iteration = 0; iteration < 1_000; iteration++)
    {
        using ApexPdfLibrary library = ApexPdfLibrary.Create();
    }
    Mark("complete iterations=1000");
}
catch (Exception exception)
{
    Mark($"failure {exception}");
    throw;
}
