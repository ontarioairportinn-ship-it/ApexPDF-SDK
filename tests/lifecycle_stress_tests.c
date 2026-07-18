#include "apexpdf/apexpdf.h"

#include <assert.h>
#include <stdlib.h>

enum { ITERATIONS = 10000 };

typedef struct allocation_state {
    size_t live;
    size_t peak;
} allocation_state;

static void* APX_CALL tracked_allocate(void* user_data, size_t size) {
    allocation_state* state = (allocation_state*)user_data;
    void* memory = malloc(size);
    if (memory != NULL) {
        state->live++;
        if (state->live > state->peak) {
            state->peak = state->live;
        }
    }
    return memory;
}

static void APX_CALL tracked_deallocate(void* user_data, void* address) {
    allocation_state* state = (allocation_state*)user_data;
    assert(address != NULL);
    assert(state->live != 0U);
    state->live--;
    free(address);
}

int main(void) {
    allocation_state state = {0};
    apx_library_options options = {0};
    int iteration;

    options.struct_size = sizeof(options);
    options.abi_major = APX_ABI_VERSION_MAJOR;
    options.abi_minor = APX_ABI_VERSION_MINOR;
    options.allocator.struct_size = sizeof(options.allocator);
    options.allocator.user_data = &state;
    options.allocator.allocate = tracked_allocate;
    options.allocator.deallocate = tracked_deallocate;

    for (iteration = 0; iteration < ITERATIONS; iteration++) {
        apx_library* library = NULL;
        apx_cancel_source* cancellation = NULL;
        assert(apx_library_create(&options, &library) == APX_STATUS_OK);
        assert(apx_cancel_source_create(library, &cancellation) == APX_STATUS_OK);
        apx_cancel_source_request(cancellation);
        assert(apx_cancel_source_is_requested(cancellation) == 1);
        apx_cancel_source_destroy(cancellation);
        apx_library_destroy(library);
        assert(state.live == 0U);
    }

    assert(state.peak == 2U);
    return 0;
}
