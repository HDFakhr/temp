cmake_minimum_required(VERSION 3.18)

file(READ "${CMAKE_SOURCE_DIR}/VERSION" VERSION_CONTENT)
string(STRIP "${VERSION_CONTENT}" PROJECT_VERSION)
message(STATUS "VERSION: ${PROJECT_VERSION}")

project(BoardGateway VERSION "${PROJECT_VERSION}" LANGUAGES CXX)
set(SERVICE_LIB_NAME ${PROJECT_NAME}Lib)
set(TESTS_TARGET_NAME ${PROJECT_NAME}Tests)
set(BENCHMARK_TARGET_NAME ${PROJECT_NAME}Benchs)
set(DOCKER_IMAGE_NAME faraz/board_gateway)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(project_setting)
include(cpp_flags)
include(conan_utils)
include(conan_dep)
include(external_dep)
include(find_packages)
include(proto_files)
include(copy_env_file)
include(build_docker)

add_subdirectory(service_lib)
add_subdirectory(service)
add_subdirectory(board_gw)

if (BUILD_WITH_TESTS)
    add_subdirectory(tests)
endif()

if (BUILD_WITH_BENCHMARKS)
    add_subdirectory(benchmark)
endif()
    
install(FILES ${CONFIG_FILE} DESTINATION bin)