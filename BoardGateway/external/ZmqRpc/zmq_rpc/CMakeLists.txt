
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.cpp.in
    ${CMAKE_BINARY_DIR}/version.cpp
)

set(LIB_SOURCE_DIR src)

set(LIB_SOURCE_CODES
    ${CMAKE_BINARY_DIR}/version.cpp
    ${LIB_SOURCE_DIR}/server/pipeline.cpp
    ${LIB_SOURCE_DIR}/client/pipeline.cpp
    ${LIB_SOURCE_DIR}/thread/thread.cpp
    ${LIB_SOURCE_DIR}/controller/request.cpp
    ${LIB_SOURCE_DIR}/controller/response.cpp
    ${LIB_SOURCE_DIR}/controller/api.cpp
    ${LIB_SOURCE_DIR}/controller/api_handler.cpp
    ${LIB_SOURCE_DIR}/controller/controller.cpp
    ${LIB_SOURCE_DIR}/client_pipeline.cpp
    ${LIB_SOURCE_DIR}/server_pipeline.cpp
)

add_library(${PROJECT_NAME} STATIC ${LIB_SOURCE_CODES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${Microsoft.GSL_COMPONENTS}
    PRIVATE ${ZMQ_COMPONENTS}
    PRIVATE ${NLOHMANN_JSON_COMPONENTS}
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release
)


install(TARGETS ${PROJECT_NAME}
    CONFIGURATIONS Debug Release
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)