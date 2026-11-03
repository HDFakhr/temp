cmake_minimum_required(VERSION 3.18)

file(READ "${CMAKE_SOURCE_DIR}/VERSION" VERSION_CONTENT)
string(STRIP "${VERSION_CONTENT}" PROJECT_VERSION)
message(STATUS "VERSION: ${PROJECT_VERSION}")

project(ZMQRpc VERSION "${PROJECT_VERSION}" LANGUAGES CXX)
set(TESTS_TARGET_NAME ${PROJECT_NAME}Tests)
set(BENCHMARK_TARGET_NAME ${PROJECT_NAME}Benchs)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(project_setting)
include(cpp_flags)
include(conan_utils)
include(conan_dep)

if (STANDALONE_BUILD)
    include(external_dep)
endif()

include(find_packages)

add_subdirectory(zmq_rpc)

if (BUILD_WITH_TESTS)
    add_subdirectory(tests)
endif()

if (BUILD_WITH_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/zmq_rpc
    TYPE INCLUDE)