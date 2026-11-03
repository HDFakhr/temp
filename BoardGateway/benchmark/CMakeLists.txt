
set(BENCHMARKS_SOURCE_DIR src)

set(BENCHMARKS_SOURCE_CODES
    ${BENCHMARKS_SOURCE_DIR}/main.cpp
)

add_executable(${BENCHMARK_TARGET_NAME} ${BENCHMARKS_SOURCE_CODES})
target_compile_definitions(${BENCHMARK_TARGET_NAME}
    PRIVATE SERVICE_VERSION="${PROJECT_VERSION}"
)
target_link_libraries(${BENCHMARK_TARGET_NAME}
    PRIVATE ${SERVICE_LIB_NAME}
    PRIVATE system::ecal
    PRIVATE conan::msgpack
    PRIVATE conan::mgsl
    PRIVATE conan::libenvpp
    PRIVATE conan::benchmark
    PRIVATE conan::glog
)

set_target_properties(
    ${BENCHMARK_TARGET_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release
)

install(TARGETS ${BENCHMARK_TARGET_NAME}
    CONFIGURATIONS Debug Release
    RUNTIME DESTINATION bin
)