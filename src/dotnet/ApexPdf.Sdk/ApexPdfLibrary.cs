using System.Runtime.InteropServices;

namespace ApexPdf.Sdk;

public sealed class ApexPdfLibrary : IDisposable
{
    private readonly SafeApexPdfLibraryHandle _handle;

    private ApexPdfLibrary(SafeApexPdfLibraryHandle handle)
    {
        _handle = handle;
    }

    public static ApexPdfVersion SdkVersion => NativeMethods.apx_sdk_version().ToManaged();

    public static ApexPdfVersion AbiVersion => NativeMethods.apx_abi_version().ToManaged();

    public static ApexPdfCapabilities OwnedCapabilities => NativeMethods.apx_owned_capabilities();

    public static ApexPdfLibrary Create()
    {
        ApexPdfVersion abi = AbiVersion;
        NativeMethods.NativeLibraryOptions options = new()
        {
            StructSize = checked((uint)Marshal.SizeOf<NativeMethods.NativeLibraryOptions>()),
            AbiMajor = abi.Major,
            AbiMinor = abi.Minor,
            Allocator = new NativeMethods.NativeAllocator
            {
                StructSize = checked((uint)Marshal.SizeOf<NativeMethods.NativeAllocator>()),
            },
        };
        int status = NativeMethods.apx_library_create(in options, out SafeApexPdfLibraryHandle handle);
        if (status != 0)
        {
            handle?.Dispose();
            string name = Marshal.PtrToStringUTF8(NativeMethods.apx_status_name(status)) ?? "unknown_status";
            throw new ApexPdfException(status, name);
        }

        return new ApexPdfLibrary(handle);
    }

    public void Dispose() => _handle.Dispose();
}
