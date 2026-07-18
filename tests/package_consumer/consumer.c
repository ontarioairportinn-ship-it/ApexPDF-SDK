#include "apexpdf/apexpdf.h"

#include <assert.h>

int main(void) {
    apx_library_options options = {0};
    apx_library* library = NULL;
    apx_version sdk = apx_sdk_version();
    apx_version abi = apx_abi_version();
    options.struct_size = sizeof(options);
    options.abi_major = abi.major;
    options.abi_minor = abi.minor;
    assert(sdk.major == APX_SDK_VERSION_MAJOR);
    assert(apx_library_create(&options, &library) == APX_STATUS_OK);
    assert(library != NULL);
    apx_library_destroy(library);
    return 0;
}
