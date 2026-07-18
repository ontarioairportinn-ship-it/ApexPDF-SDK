using Microsoft.Win32.SafeHandles;

namespace ApexPdf.Sdk;

internal sealed class SafeApexPdfLibraryHandle : SafeHandleZeroOrMinusOneIsInvalid
{
    internal SafeApexPdfLibraryHandle()
        : base(true)
    {
    }

    protected override bool ReleaseHandle()
    {
        NativeMethods.apx_library_destroy(handle);
        return true;
    }
}
