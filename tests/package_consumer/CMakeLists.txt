cmake_minimum_required(VERSION 3.28)
project(ApexPdfSdkPackageConsumer LANGUAGES C CXX)

if(NOT DEFINED APX_SDK_ROOT)
    message(FATAL_ERROR "APX_SDK_ROOT must identify the extracted ApexPDF SDK package root")
endif()

find_path(APX_INCLUDE_DIRECTORY
    NAMES apexpdf/apexpdf.h
    PATHS "${APX_SDK_ROOT}/include"
    NO_DEFAULT_PATH
    REQUIRED)
find_library(APX_IMPORT_LIBRARY
    NAMES ApexPdfSdk
    PATHS "${APX_SDK_ROOT}/lib"
    NO_DEFAULT_PATH
    REQUIRED)

add_executable(package_consumer_c consumer.c)
target_include_directories(package_consumer_c PRIVATE "${APX_INCLUDE_DIRECTORY}")
target_link_libraries(package_consumer_c PRIVATE "${APX_IMPORT_LIBRARY}")

add_executable(package_consumer_cpp consumer.cpp)
target_include_directories(package_consumer_cpp PRIVATE "${APX_INCLUDE_DIRECTORY}")
target_link_libraries(package_consumer_cpp PRIVATE "${APX_IMPORT_LIBRARY}")

enable_testing()
add_test(NAME package_consumer_c COMMAND package_consumer_c)
add_test(NAME package_consumer_cpp COMMAND package_consumer_cpp)
set_tests_properties(package_consumer_c package_consumer_cpp PROPERTIES
    ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${APX_SDK_ROOT}/bin")
