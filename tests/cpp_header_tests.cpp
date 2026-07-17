#include "apexpdf/apexpdf.h"

#include <cassert>

int main() {
    apx_library_options options{};
    options.struct_size = sizeof(options);
    options.abi_major = APX_ABI_VERSION_MAJOR;
    options.abi_minor = APX_ABI_VERSION_MINOR;
    apx_library* library = nullptr;
    assert(apx_library_create(&options, &library) == APX_STATUS_OK);
    apx_cancel_source* cancellation = nullptr;
    assert(apx_cancel_source_create(library, &cancellation) == APX_STATUS_OK);
    assert(apx_cancel_source_is_requested(cancellation) == 0);
    apx_cancel_source_request(cancellation);
    assert(apx_cancel_source_is_requested(cancellation) == 1);
    apx_cancel_source_destroy(cancellation);
    apx_library_destroy(library);
    return 0;
}
