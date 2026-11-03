
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.cpp.in
    ${CMAKE_BINARY_DIR}/version.cpp
)

set(LIB_SOURCE_DIR src)

set(API_HANDLERS
    ${LIB_SOURCE_DIR}/api/board/ok/handler.cpp
    ${LIB_SOURCE_DIR}/api/board/adc/handler.cpp
    ${LIB_SOURCE_DIR}/api/board/adc/channel/user/handler.cpp
    ${LIB_SOURCE_DIR}/api/board/channel/adc/handler.cpp
    ${LIB_SOURCE_DIR}/api/board/nco/handler.cpp
    ${LIB_SOURCE_DIR}/api/board/attenuation/handler.cpp
    ${LIB_SOURCE_DIR}/api/spectrum/handler.cpp
    ${LIB_SOURCE_DIR}/api/dma/start/handler.cpp
    ${LIB_SOURCE_DIR}/api/dma/status/handler.cpp
    ${LIB_SOURCE_DIR}/api/dma/data/handler.cpp
    ${LIB_SOURCE_DIR}/api/capture/status/handler.cpp
    ${LIB_SOURCE_DIR}/api/capture/start/handler.cpp
    ${LIB_SOURCE_DIR}/api/capture/stop/handler.cpp
    ${LIB_SOURCE_DIR}/api/capture/device/info/handler.cpp
)

set(LIB_SOURCE_CODES
    ${CMAKE_BINARY_DIR}/version.cpp
    ${LIB_SOURCE_DIR}/tools.cpp
    ${LIB_SOURCE_DIR}/pipeline.cpp
    ${API_HANDLERS}
)

add_library(${PROJECT_NAME} STATIC ${LIB_SOURCE_CODES})
target_link_libraries(${PROJECT_NAME}
    PRIVATE ZmqRpcLib
    PRIVATE ${FMT_COMPONENTS}
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/Debug
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/Release
)


install(TARGETS ${PROJECT_NAME}
    CONFIGURATIONS Debug Release
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)