#include "apexpdf/apexpdf.h"

#include <atomic>
#include <cstdlib>
#include <new>

struct apx_library {
    uint32_t abi_minor;
    apx_allocator allocator;
    void* log_user_data;
    apx_log_fn log;
};

struct apx_cancel_source {
    apx_library* library;
    std::atomic<bool> requested;
};

namespace {
void* APX_CALL default_allocate(void*, size_t size) {
    return std::malloc(size);
}

void APX_CALL default_deallocate(void*, void* address) {
    std::free(address);
}

apx_allocator effective_allocator(const apx_library_options& options) {
    if (options.allocator.struct_size >= sizeof(apx_allocator) &&
        options.allocator.allocate != nullptr &&
        options.allocator.deallocate != nullptr) {
        return options.allocator;
    }
    return {sizeof(apx_allocator), nullptr, default_allocate, default_deallocate};
}

bool valid_operation_context(const apx_operation_context* context) {
    return context == nullptr || context->struct_size >= sizeof(apx_operation_context);
}
}

apx_version APX_CALL apx_sdk_version(void) {
    return {APX_SDK_VERSION_MAJOR, APX_SDK_VERSION_MINOR, APX_SDK_VERSION_PATCH};
}

apx_version APX_CALL apx_abi_version(void) {
    return {APX_ABI_VERSION_MAJOR, APX_ABI_VERSION_MINOR, 0U};
}

apx_capabilities APX_CALL apx_owned_capabilities(void) {
    return APX_CAPABILITY_NONE;
}

const char* APX_CALL apx_status_name(apx_status status) {
    switch (status) {
        case APX_STATUS_OK: return "ok";
        case APX_STATUS_INVALID_ARGUMENT: return "invalid_argument";
        case APX_STATUS_OUT_OF_MEMORY: return "out_of_memory";
        case APX_STATUS_NOT_INITIALIZED: return "not_initialized";
        case APX_STATUS_ALREADY_INITIALIZED: return "already_initialized";
        case APX_STATUS_UNSUPPORTED_ABI: return "unsupported_abi";
        case APX_STATUS_INTERNAL_ERROR: return "internal_error";
        case APX_STATUS_CANCELLED: return "cancelled";
        case APX_STATUS_IO_ERROR: return "io_error";
        case APX_STATUS_OUT_OF_RANGE: return "out_of_range";
        default: return "unknown_status";
    }
}

apx_status APX_CALL apx_library_create(
    const apx_library_options* options,
    apx_library** library) {
    if (options == nullptr || library == nullptr) {
        return APX_STATUS_INVALID_ARGUMENT;
    }
    *library = nullptr;
    if (options->struct_size < sizeof(apx_library_options)) {
        return APX_STATUS_INVALID_ARGUMENT;
    }
    if (options->abi_major != APX_ABI_VERSION_MAJOR ||
        options->abi_minor > APX_ABI_VERSION_MINOR) {
        return APX_STATUS_UNSUPPORTED_ABI;
    }

    const bool allocator_partially_configured =
        options->allocator.allocate != nullptr || options->allocator.deallocate != nullptr;
    if (allocator_partially_configured &&
        (options->allocator.struct_size < sizeof(apx_allocator) ||
         options->allocator.allocate == nullptr ||
         options->allocator.deallocate == nullptr)) {
        return APX_STATUS_INVALID_ARGUMENT;
    }

    apx_allocator allocator = effective_allocator(*options);
    void* storage = allocator.allocate(allocator.user_data, sizeof(apx_library));
    if (storage == nullptr) {
        return APX_STATUS_OUT_OF_MEMORY;
    }
    apx_library* instance = new (storage) apx_library{
        options->abi_minor, allocator, options->log_user_data, options->log};
    if (instance == nullptr) {
        return APX_STATUS_OUT_OF_MEMORY;
    }
    *library = instance;
    if (instance->log != nullptr) {
        instance->log(instance->log_user_data, APX_LOG_LEVEL_INFORMATION, 1000U,
                      "ApexPDF SDK library initialized");
    }
    return APX_STATUS_OK;
}

void APX_CALL apx_library_destroy(apx_library* library) {
    if (library == nullptr) {
        return;
    }
    if (library->log != nullptr) {
        library->log(library->log_user_data, APX_LOG_LEVEL_INFORMATION, 1001U,
                     "ApexPDF SDK library shutdown");
    }
    apx_allocator allocator = library->allocator;
    library->~apx_library();
    allocator.deallocate(allocator.user_data, library);
}

apx_status APX_CALL apx_cancel_source_create(
    apx_library* library,
    apx_cancel_source** source) {
    if (library == nullptr || source == nullptr) {
        return APX_STATUS_INVALID_ARGUMENT;
    }
    *source = nullptr;
    void* storage = library->allocator.allocate(
        library->allocator.user_data, sizeof(apx_cancel_source));
    if (storage == nullptr) {
        return APX_STATUS_OUT_OF_MEMORY;
    }
    *source = new (storage) apx_cancel_source{library, false};
    return APX_STATUS_OK;
}

void APX_CALL apx_cancel_source_destroy(apx_cancel_source* source) {
    if (source == nullptr) {
        return;
    }
    apx_library* library = source->library;
    source->~apx_cancel_source();
    library->allocator.deallocate(library->allocator.user_data, source);
}

void APX_CALL apx_cancel_source_request(apx_cancel_source* source) {
    if (source != nullptr) {
        source->requested.store(true, std::memory_order_release);
    }
}

int32_t APX_CALL apx_cancel_source_is_requested(const apx_cancel_source* source) {
    return source != nullptr && source->requested.load(std::memory_order_acquire) ? 1 : 0;
}

apx_status APX_CALL apx_operation_check(const apx_operation_context* context) {
    if (!valid_operation_context(context)) {
        return APX_STATUS_INVALID_ARGUMENT;
    }
    if (context != nullptr && apx_cancel_source_is_requested(context->cancellation) != 0) {
        return APX_STATUS_CANCELLED;
    }
    return APX_STATUS_OK;
}

apx_status APX_CALL apx_operation_report_progress(
    const apx_operation_context* context,
    uint32_t completed,
    uint32_t total) {
    apx_status status = apx_operation_check(context);
    if (status != APX_STATUS_OK) {
        return status;
    }
    if (total == 0U || completed > total) {
        return APX_STATUS_OUT_OF_RANGE;
    }
    if (context != nullptr && context->progress != nullptr) {
        context->progress(context->progress_user_data, completed, total);
    }
    return APX_STATUS_OK;
}

apx_status APX_CALL apx_stream_get_size(
    const apx_random_access_stream* stream,
    uint64_t* size) {
    if (stream == nullptr || size == nullptr ||
        stream->struct_size < sizeof(apx_random_access_stream) ||
        stream->get_size == nullptr || stream->read_at == nullptr) {
        return APX_STATUS_INVALID_ARGUMENT;
    }
    *size = 0U;
    return stream->get_size(stream->user_data, size);
}

apx_status APX_CALL apx_stream_read_at(
    const apx_random_access_stream* stream,
    uint64_t offset,
    void* destination,
    size_t requested,
    size_t* read) {
    if (stream == nullptr || read == nullptr ||
        stream->struct_size < sizeof(apx_random_access_stream) ||
        stream->get_size == nullptr || stream->read_at == nullptr ||
        (requested != 0U && destination == nullptr)) {
        return APX_STATUS_INVALID_ARGUMENT;
    }
    *read = 0U;
    return stream->read_at(stream->user_data, offset, destination, requested, read);
}
