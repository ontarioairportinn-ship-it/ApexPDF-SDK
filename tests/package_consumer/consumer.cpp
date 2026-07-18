#include "apexpdf/apexpdf.h"

#include <cassert>

int main() {
    apx_library_options options{};
    const apx_version abi = apx_abi_version();
    options.struct_size = sizeof(options);
    options.abi_major = abi.major;
    options.abi_minor = abi.minor;
    apx_library* library = nullptr;
    assert(apx_library_create(&options, &library) == APX_STATUS_OK);
    assert(library != nullptr);
    apx_library_destroy(library);
    return 0;
}
