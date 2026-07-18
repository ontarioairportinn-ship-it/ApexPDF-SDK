if(NOT EXISTS "${DUMPBIN}")
    message(FATAL_ERROR "MSVC dumpbin was not found: ${DUMPBIN}")
endif()
if(NOT EXISTS "${LIBRARY}")
    message(FATAL_ERROR "SDK library was not found: ${LIBRARY}")
endif()

execute_process(
    COMMAND "${DUMPBIN}" /nologo /exports "${LIBRARY}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "dumpbin failed (${result}): ${error}")
endif()

set(expected
    apx_abi_version
    apx_cancel_source_create
    apx_cancel_source_destroy
    apx_cancel_source_is_requested
    apx_cancel_source_request
    apx_library_create
    apx_library_destroy
    apx_operation_check
    apx_operation_report_progress
    apx_sdk_version
    apx_status_name
    apx_stream_get_size
    apx_stream_read_at)

string(REPLACE "\r\n" "\n" output "${output}")
string(REPLACE "\n" ";" lines "${output}")
set(actual)
foreach(line IN LISTS lines)
    if(line MATCHES "^[ \\t]+[0-9]+[ \\t]+[0-9A-Fa-f]+[ \\t]+[0-9A-Fa-f]+[ \\t]+(apx_[A-Za-z0-9_]+)([ \\t]+=[^;]*)?[ \\t]*$")
        list(APPEND actual "${CMAKE_MATCH_1}")
    endif()
endforeach()
list(SORT actual)
list(SORT expected)

if(NOT actual STREQUAL expected)
    message(FATAL_ERROR "ABI export mismatch. Expected: ${expected}. Actual: ${actual}")
endif()
message(STATUS "ApexPDF ABI exports match the approved list: ${actual}")
