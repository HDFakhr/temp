

set(SERVICE_SOURCE_DIR src)

set(SERVICE_SOURCE_CODES
    ${SERVICE_SOURCE_DIR}/main.cpp
)

add_executable(${PROJECT_NAME} ${SERVICE_SOURCE_CODES})
target_compile_definitions(${PROJECT_NAME}
    PRIVATE SERVICE_VERSION="${PROJECT_VERSION}"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CONFIGS_INCLUDE_DIR}
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${SERVICE_LIB_NAME}
    PRIVATE ${ZMQRPC_PACKAGE}
    PRIVATE FRZGatewayLib
    PRIVATE system::ecal
    PRIVATE conan::msgpack
    PRIVATE conan::mgsl
    PRIVATE conan::libenvpp
    PRIVATE conan::CLI11
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release
)


install(TARGETS ${PROJECT_NAME}
    CONFIGURATIONS Debug Release
    RUNTIME DESTINATION bin
)