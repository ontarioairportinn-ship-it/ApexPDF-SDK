namespace ApexPdf.Sdk;

public readonly record struct ApexPdfVersion(uint Major, uint Minor, uint Patch)
{
    public override string ToString() => $"{Major}.{Minor}.{Patch}";
}
