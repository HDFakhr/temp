

set(TESTS_SOURCE_DIR src)

set(TESTS_SOURCE_CODES
    ${TESTS_SOURCE_DIR}/main.cpp
    ${TESTS_SOURCE_DIR}/test_Version.cpp
    ${TESTS_SOURCE_DIR}/test_BoardApis.cpp
    ${TESTS_SOURCE_DIR}/test_SpectrumApis.cpp
    ${TESTS_SOURCE_DIR}/test_DmaApis.cpp
    ${TESTS_SOURCE_DIR}/test_CaptureApis.cpp
)

add_executable(${TESTS_TARGET_NAME} ${TESTS_SOURCE_CODES})
target_include_directories(${TESTS_TARGET_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_definitions(${TESTS_TARGET_NAME}
    PUBLIC PROJECT_VERSION="${PROJECT_VERSION}"
    PUBLIC BOARD_CONFIG_FILE="${BOARD_CONFIG_FILE_NAME}"
)
target_link_libraries(${TESTS_TARGET_NAME}
    PRIVATE ${PROJECT_NAME}
    PRIVATE ZmqRpcLib
    PRIVATE ${GTest_COMPONENTS}
    PRIVATE ${GLOG_COMPONENTS}
    PRIVATE ${FMT_COMPONENTS}
)

set_target_properties(
    ${TESTS_TARGET_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)

install(TARGETS ${TESTS_TARGET_NAME}
    CONFIGURATIONS Debug Release
    RUNTIME DESTINATION bin
)