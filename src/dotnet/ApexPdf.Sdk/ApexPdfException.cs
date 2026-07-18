namespace ApexPdf.Sdk;

public sealed class ApexPdfException : Exception
{
    internal ApexPdfException(int status, string statusName)
        : base($"ApexPDF SDK operation failed: {statusName} ({status}).")
    {
        Status = status;
        StatusName = statusName;
    }

    public int Status { get; }

    public string StatusName { get; }
}
