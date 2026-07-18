#ifndef APEXPDF_H
#define APEXPDF_H

#include "apexpdf_types.h"
#include "apexpdf_version.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct apx_library_options {
    uint32_t struct_size;
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t reserved;
    apx_allocator allocator;
    void* log_user_data;
    apx_log_fn log;
} apx_library_options;

APX_API apx_version APX_CALL apx_sdk_version(void);
APX_API apx_version APX_CALL apx_abi_version(void);
APX_API apx_capabilities APX_CALL apx_owned_capabilities(void);
APX_API const char* APX_CALL apx_status_name(apx_status status);
APX_API apx_status APX_CALL apx_library_create(
    const apx_library_options* options,
    apx_library** library);
APX_API void APX_CALL apx_library_destroy(apx_library* library);
APX_API apx_status APX_CALL apx_cancel_source_create(
    apx_library* library,
    apx_cancel_source** source);
APX_API void APX_CALL apx_cancel_source_destroy(apx_cancel_source* source);
APX_API void APX_CALL apx_cancel_source_request(apx_cancel_source* source);
APX_API int32_t APX_CALL apx_cancel_source_is_requested(
    const apx_cancel_source* source);
APX_API apx_status APX_CALL apx_operation_check(
    const apx_operation_context* context);
APX_API apx_status APX_CALL apx_operation_report_progress(
    const apx_operation_context* context,
    uint32_t completed,
    uint32_t total);
APX_API apx_status APX_CALL apx_stream_get_size(
    const apx_random_access_stream* stream,
    uint64_t* size);
APX_API apx_status APX_CALL apx_stream_read_at(
    const apx_random_access_stream* stream,
    uint64_t offset,
    void* destination,
    size_t requested,
    size_t* read);

#ifdef __cplusplus
}
#endif

#endif
