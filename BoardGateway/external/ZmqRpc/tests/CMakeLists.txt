

set(TESTS_SOURCE_DIR src)

set(TESTS_SOURCE_CODES
    ${TESTS_SOURCE_DIR}/main.cpp
    ${TESTS_SOURCE_DIR}/test_Version.cpp
    ${TESTS_SOURCE_DIR}/test_Resquest.cpp
    ${TESTS_SOURCE_DIR}/test_Controller.cpp
    ${TESTS_SOURCE_DIR}/test_FunctionTraits.cpp
    ${TESTS_SOURCE_DIR}/test_Api.cpp
    ${TESTS_SOURCE_DIR}/test_ServerAndClient.cpp
)

add_executable(${TESTS_TARGET_NAME} ${TESTS_SOURCE_CODES})
target_compile_definitions(${TESTS_TARGET_NAME}
    PUBLIC PROJECT_VERSION="${PROJECT_VERSION}"
)
target_include_directories(${TESTS_TARGET_NAME} PUBLIC include)
target_link_libraries(${TESTS_TARGET_NAME}
    PRIVATE ${PROJECT_NAME}
    PRIVATE ${GTest_COMPONENTS}
    PRIVATE ${Microsoft.GSL_COMPONENTS}
    PRIVATE ${GLOG_COMPONENTS}
    PRIVATE ${ZMQ_COMPONENTS}
    PRIVATE ${NLOHMANN_JSON_COMPONENTS}
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