#include "apexpdf/apexpdf.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct test_state {
    size_t allocations;
    size_t deallocations;
    size_t logs;
    uint32_t progress_completed;
    uint32_t progress_total;
} test_state;

typedef struct memory_stream {
    const unsigned char* data;
    size_t size;
} memory_stream;

static void* APX_CALL test_allocate(void* user_data, size_t size) {
    test_state* state = (test_state*)user_data;
    state->allocations++;
    return malloc(size);
}

static void APX_CALL test_deallocate(void* user_data, void* address) {
    test_state* state = (test_state*)user_data;
    state->deallocations++;
    free(address);
}

static void APX_CALL test_log(
    void* user_data,
    apx_log_level level,
    uint32_t event_id,
    const char* message) {
    test_state* state = (test_state*)user_data;
    assert(level == APX_LOG_LEVEL_INFORMATION);
    assert(event_id == 1000U || event_id == 1001U);
    assert(message != NULL);
    state->logs++;
}

static void APX_CALL test_progress(void* user_data, uint32_t completed, uint32_t total) {
    test_state* state = (test_state*)user_data;
    state->progress_completed = completed;
    state->progress_total = total;
}

static apx_status APX_CALL memory_get_size(void* user_data, uint64_t* size) {
    memory_stream* stream = (memory_stream*)user_data;
    *size = stream->size;
    return APX_STATUS_OK;
}

static apx_status APX_CALL memory_read_at(
    void* user_data,
    uint64_t offset,
    void* destination,
    size_t requested,
    size_t* read) {
    memory_stream* stream = (memory_stream*)user_data;
    size_t available;
    size_t count;
    if (offset > stream->size) {
        return APX_STATUS_OUT_OF_RANGE;
    }
    available = stream->size - (size_t)offset;
    count = requested < available ? requested : available;
    if (count != 0U) {
        memcpy(destination, stream->data + (size_t)offset, count);
    }
    *read = count;
    return APX_STATUS_OK;
}

int main(void) {
    apx_library* library = NULL;
    apx_cancel_source* cancellation = NULL;
    test_state state = {0};
    apx_library_options options = {
        sizeof(apx_library_options), APX_ABI_VERSION_MAJOR, APX_ABI_VERSION_MINOR, 0U,
        {sizeof(apx_allocator), &state, test_allocate, test_deallocate},
        &state, test_log};
    apx_operation_context context = {
        sizeof(apx_operation_context), NULL, &state, test_progress};
    const unsigned char source[] = {10U, 20U, 30U, 40U};
    unsigned char destination[3] = {0U};
    memory_stream memory = {source, sizeof(source)};
    apx_random_access_stream stream = {
        sizeof(apx_random_access_stream), &memory, memory_get_size, memory_read_at};
    uint64_t stream_size = 0U;
    size_t bytes_read = 0U;

    {
        apx_library* default_library = NULL;
        apx_library_options default_options = {0};
        default_options.struct_size = sizeof(default_options);
        default_options.abi_major = APX_ABI_VERSION_MAJOR;
        default_options.abi_minor = APX_ABI_VERSION_MINOR;
        assert(apx_library_create(&default_options, &default_library) == APX_STATUS_OK);
        apx_library_destroy(default_library);
    }

    assert(apx_sdk_version().major == APX_SDK_VERSION_MAJOR);
    assert(apx_owned_capabilities() == APX_CAPABILITY_NONE);
    assert(apx_library_create(NULL, &library) == APX_STATUS_INVALID_ARGUMENT);
    assert(apx_library_create(&options, NULL) == APX_STATUS_INVALID_ARGUMENT);
    assert(apx_library_create(&options, &library) == APX_STATUS_OK);
    assert(library != NULL);
    assert(state.allocations == 1U);
    assert(state.logs == 1U);
    assert(strcmp(apx_status_name(APX_STATUS_OK), "ok") == 0);

    assert(apx_cancel_source_create(library, &cancellation) == APX_STATUS_OK);
    assert(cancellation != NULL);
    assert(apx_cancel_source_is_requested(cancellation) == 0);
    context.cancellation = cancellation;
    assert(apx_operation_check(&context) == APX_STATUS_OK);
    assert(apx_operation_report_progress(&context, 2U, 5U) == APX_STATUS_OK);
    assert(state.progress_completed == 2U && state.progress_total == 5U);
    assert(apx_operation_report_progress(&context, 6U, 5U) == APX_STATUS_OUT_OF_RANGE);
    apx_cancel_source_request(cancellation);
    assert(apx_operation_check(&context) == APX_STATUS_CANCELLED);
    assert(apx_operation_report_progress(&context, 3U, 5U) == APX_STATUS_CANCELLED);

    assert(apx_stream_get_size(&stream, &stream_size) == APX_STATUS_OK);
    assert(stream_size == sizeof(source));
    assert(apx_stream_read_at(&stream, 1U, destination, sizeof(destination), &bytes_read)
           == APX_STATUS_OK);
    assert(bytes_read == sizeof(destination));
    assert(destination[0] == 20U && destination[2] == 40U);
    assert(apx_stream_read_at(&stream, 5U, destination, 1U, &bytes_read)
           == APX_STATUS_OUT_OF_RANGE);
    assert(apx_stream_read_at(&stream, 0U, NULL, 1U, &bytes_read)
           == APX_STATUS_INVALID_ARGUMENT);

    apx_cancel_source_destroy(cancellation);
    apx_library_destroy(library);
    assert(state.allocations == state.deallocations);
    assert(state.logs == 2U);
    return 0;
}
