using System.Runtime.InteropServices;

namespace ApexPdf.Sdk;

internal static class NativeMethods
{
    private const string LibraryName = "ApexPdfSdk";

    [StructLayout(LayoutKind.Sequential)]
    internal struct NativeVersion
    {
        internal uint Major;
        internal uint Minor;
        internal uint Patch;

        internal readonly ApexPdfVersion ToManaged() => new(Major, Minor, Patch);
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct NativeAllocator
    {
        internal uint StructSize;
        internal nint UserData;
        internal nint Allocate;
        internal nint Deallocate;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct NativeLibraryOptions
    {
        internal uint StructSize;
        internal uint AbiMajor;
        internal uint AbiMinor;
        internal uint Reserved;
        internal NativeAllocator Allocator;
        internal nint LogUserData;
        internal nint Log;
    }

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeVersion apx_sdk_version();

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern NativeVersion apx_abi_version();

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern ApexPdfCapabilities apx_owned_capabilities();

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern nint apx_status_name(int status);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern int apx_library_create(
        in NativeLibraryOptions options,
        out SafeApexPdfLibraryHandle library);

    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    internal static extern void apx_library_destroy(nint library);
}
