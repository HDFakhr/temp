

set(TESTS_SOURCE_DIR src)

set(TESTS_SOURCE_CODES
    ${TESTS_SOURCE_DIR}/main.cpp
)

add_executable(${TESTS_TARGET_NAME} ${TESTS_SOURCE_CODES})
target_compile_definitions(${TESTS_TARGET_NAME}
    PRIVATE SERVICE_VERSION="${PROJECT_VERSION}"
)
target_link_libraries(${TESTS_TARGET_NAME}
    PRIVATE ${SERVICE_LIB_NAME}
    PRIVATE system::ecal
    PRIVATE conan::msgpack
    PRIVATE conan::mgsl
    PRIVATE conan::libenvpp
    PRIVATE conan::glog
    PRIVATE conan::gtest
)

set_target_properties(
    ${TESTS_TARGET_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release
)

install(TARGETS ${TESTS_TARGET_NAME}
    CONFIGURATIONS Debug Release
    RUNTIME DESTINATION bin
)