namespace ApexPdf.Sdk;

[Flags]
public enum ApexPdfCapabilities : ulong
{
    None = 0,
    Parse = 1UL << 0,
    Preserve = 1UL << 1,
    Render = 1UL << 2,
    ExtractText = 1UL << 3,
    Edit = 1UL << 4,
    Write = 1UL << 5,
    Validate = 1UL << 6,
}
