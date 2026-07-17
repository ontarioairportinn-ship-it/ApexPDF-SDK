cmake_minimum_required(VERSION 3.28)
project(ApexPdfSdk VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
include(GNUInstallDirs)

option(APX_ENABLE_ASAN "Enable AddressSanitizer for ApexPDF SDK targets" OFF)
option(APX_ENABLE_STATIC_ANALYSIS "Enable MSVC native static analysis" OFF)

add_library(ApexPdfSdk SHARED
    src/abi/apexpdf.cpp)

target_compile_features(ApexPdfSdk PRIVATE cxx_std_20)
target_include_directories(ApexPdfSdk
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_compile_definitions(ApexPdfSdk PRIVATE APX_BUILDING_LIBRARY WIN32_LEAN_AND_MEAN NOMINMAX)

if(MSVC)
    target_compile_options(ApexPdfSdk PRIVATE /W4 /WX /permissive- /sdl /guard:cf /utf-8)
    target_link_options(ApexPdfSdk PRIVATE /guard:cf /DYNAMICBASE /NXCOMPAT /HIGHENTROPYVA)
    if(APX_ENABLE_ASAN)
        target_compile_options(ApexPdfSdk PRIVATE /fsanitize=address)
        target_link_options(ApexPdfSdk PRIVATE /fsanitize=address)
    endif()
    if(APX_ENABLE_STATIC_ANALYSIS)
        target_compile_options(ApexPdfSdk PRIVATE /analyze)
    endif()
else()
    target_compile_options(ApexPdfSdk PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

install(TARGETS ApexPdfSdk
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_NAME "ApexPdfSdk")
set(CPACK_PACKAGE_VENDOR "ApexPDF")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME "ApexPdfSdk-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
include(CPack)

if(BUILD_TESTING)
    add_executable(ApexPdfSdk.AbiTests tests/abi_tests.c)
    target_link_libraries(ApexPdfSdk.AbiTests PRIVATE ApexPdfSdk)
    if(MSVC AND APX_ENABLE_ASAN)
        target_compile_options(ApexPdfSdk.AbiTests PRIVATE /fsanitize=address)
        target_link_options(ApexPdfSdk.AbiTests PRIVATE /fsanitize=address)
    endif()
    add_test(NAME ApexPdfSdk.AbiTests COMMAND ApexPdfSdk.AbiTests)

    add_executable(ApexPdfSdk.CppHeaderTests tests/cpp_header_tests.cpp)
    target_link_libraries(ApexPdfSdk.CppHeaderTests PRIVATE ApexPdfSdk)
    add_test(NAME ApexPdfSdk.CppHeaderTests COMMAND ApexPdfSdk.CppHeaderTests)
    if(MSVC AND APX_ENABLE_ASAN)
        # MSVC 19.51 can deadlock during ASan runtime shutdown when the same
        # instrumented DLL is loaded by a C++ test host. The C ABI lifecycle
        # test exercises the instrumented DLL; this target still compiles to
        # enforce C++ header compatibility and runs in Debug/Release presets.
        set_tests_properties(ApexPdfSdk.CppHeaderTests PROPERTIES DISABLED TRUE)
    endif()
endif()
