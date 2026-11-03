cmake_minimum_required(VERSION 3.18)

file(READ "${CMAKE_SOURCE_DIR}/VERSION" VERSION_CONTENT)
string(STRIP "${VERSION_CONTENT}" PROJECT_VERSION)
message(STATUS "VERSION: ${PROJECT_VERSION}")

project(BoardZmqClient VERSION "${PROJECT_VERSION}" LANGUAGES CXX)
set(TESTS_TARGET_NAME ${PROJECT_NAME}Tests)
set(BENCHMARK_TARGET_NAME ${PROJECT_NAME}Benchs)
set(BOARD_CONFIG_FILE_NAME board_config.json)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

option(STANDALONE_BUILD "Build as standalone project with all dependencies" OFF)
option(BUILD_WITH_TESTS "Build tests" OFF)
option(BUILD_WITH_BENCHMARKS "Build benchmakrs" OFF)
option(UPDATE_GIT_SUBMODULES "Cmake will update submodule" OFF)

include(project_setting)
include(cpp_flags)
include(conan_utils)
include(conan_dep)

if (STANDALONE_BUILD)
    include(external_dep)
endif()

include(find_packages)
include(external_targets)

add_subdirectory(board_zmq_client)

if (BUILD_WITH_TESTS)
    add_subdirectory(tests)
endif()

if (BUILD_WITH_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

if (BUILD_WITH_TESTS OR BUILD_WITH_BENCHMARKS)
    file(COPY ${BOARD_CONFIG_FILE_NAME} DESTINATION ${CMAKE_BINARY_DIR}/bin/Debug)
    file(COPY ${BOARD_CONFIG_FILE_NAME} DESTINATION ${CMAKE_BINARY_DIR}/bin/Release)
    install(FILES ${BOARD_CONFIG_FILE_NAME} DESTINATION bin)
endif()

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/board_zmq_client
    TYPE INCLUDE)