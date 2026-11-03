
set(TESTS_SOURCE_DIR src)

set(TESTS_SOURCE_CODES ${TESTS_SOURCE_DIR}/main.cpp)

add_executable(${TESTS_TARGET_NAME} ${TESTS_SOURCE_CODES})

target_include_directories(${TESTS_TARGET_NAME} PRIVATE include)

target_compile_definitions(${TESTS_TARGET_NAME}
    PRIVATE -DYAML_ENABLE=true
    PRIVATE -DJSON_ENABLE=true
    PRIVATE -DENV_ENABLE=true
    PRIVATE -DARGS_ENABLE=true
    )

find_package(GTest REQUIRED)
find_package(glog REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(libenvpp REQUIRED)
find_package(CLI11 REQUIRED)

target_link_libraries(${TESTS_TARGET_NAME}
    # PRIVATE ${PROJECT_NAME}
    PRIVATE GTest::gtest
    PRIVATE GTest::gmock
    PRIVATE glog::glog
    PRIVATE yaml-cpp::yaml-cpp
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE libenvpp::libenvpp
    PRIVATE CLI11::CLI11
    )

set_target_properties(
    ${TESTS_TARGET_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)

# copy testing config files to output
configure_file(config_test.yaml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config_test.yaml COPYONLY)
configure_file(config_test.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config_test.json COPYONLY)
configure_file(config_test.env ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config_test.env COPYONLY)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/.vscode)
configure_file(launch.json.in ${PROJECT_SOURCE_DIR}/.vscode/launch.json @ONLY)
configure_file(settings.json.in ${PROJECT_SOURCE_DIR}/.vscode/settings.json @ONLY)
