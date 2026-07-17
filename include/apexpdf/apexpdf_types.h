#ifndef APEXPDF_TYPES_H
#define APEXPDF_TYPES_H

#include <stddef.h>
#include <stdint.h>

#if defined(_WIN32)
#  if defined(APX_BUILDING_LIBRARY)
#    define APX_API __declspec(dllexport)
#  else
#    define APX_API __declspec(dllimport)
#  endif
#  define APX_CALL __cdecl
#else
#  define APX_API
#  define APX_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t apx_status;

enum {
    APX_STATUS_OK = 0,
    APX_STATUS_INVALID_ARGUMENT = 1,
    APX_STATUS_OUT_OF_MEMORY = 2,
    APX_STATUS_NOT_INITIALIZED = 3,
    APX_STATUS_ALREADY_INITIALIZED = 4,
    APX_STATUS_UNSUPPORTED_ABI = 5,
    APX_STATUS_INTERNAL_ERROR = 6,
    APX_STATUS_CANCELLED = 7,
    APX_STATUS_IO_ERROR = 8,
    APX_STATUS_OUT_OF_RANGE = 9
};

typedef struct apx_version {
    uint32_t major;
    uint32_t minor;
    uint32_t patch;
} apx_version;

typedef struct apx_library apx_library;
typedef struct apx_cancel_source apx_cancel_source;

typedef void* (APX_CALL* apx_allocate_fn)(void* user_data, size_t size);
typedef void (APX_CALL* apx_deallocate_fn)(void* user_data, void* address);

typedef struct apx_allocator {
    uint32_t struct_size;
    void* user_data;
    apx_allocate_fn allocate;
    apx_deallocate_fn deallocate;
} apx_allocator;

typedef int32_t apx_log_level;

enum {
    APX_LOG_LEVEL_ERROR = 1,
    APX_LOG_LEVEL_WARNING = 2,
    APX_LOG_LEVEL_INFORMATION = 3,
    APX_LOG_LEVEL_DEBUG = 4
};

typedef void (APX_CALL* apx_log_fn)(
    void* user_data,
    apx_log_level level,
    uint32_t event_id,
    const char* message);

typedef void (APX_CALL* apx_progress_fn)(
    void* user_data,
    uint32_t completed,
    uint32_t total);

typedef struct apx_operation_context {
    uint32_t struct_size;
    const apx_cancel_source* cancellation;
    void* progress_user_data;
    apx_progress_fn progress;
} apx_operation_context;

typedef apx_status (APX_CALL* apx_stream_size_fn)(
    void* user_data,
    uint64_t* size);
typedef apx_status (APX_CALL* apx_stream_read_at_fn)(
    void* user_data,
    uint64_t offset,
    void* destination,
    size_t requested,
    size_t* read);

typedef struct apx_random_access_stream {
    uint32_t struct_size;
    void* user_data;
    apx_stream_size_fn get_size;
    apx_stream_read_at_fn read_at;
} apx_random_access_stream;

#ifdef __cplusplus
}
#endif

#endif
