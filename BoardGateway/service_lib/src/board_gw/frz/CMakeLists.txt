

set(SOURCE_DIR src)

set(SOURCE_CODES
    ${SOURCE_DIR}/config_handler.cpp
    ${SOURCE_DIR}/board_apis.cpp
    ${SOURCE_DIR}/spectrum_apis.cpp
    ${SOURCE_DIR}/dma_apis.cpp
    ${SOURCE_DIR}/gateway.cpp
    ${SOURCE_DIR}/capture_apis.cpp
)

add_library(FRZGateway STATIC ${SOURCE_CODES})

target_link_libraries(FRZGateway
    PRIVATE BoardZmqClientLib
    PRIVATE ${FMT_COMPONENTS}
)

set_target_properties(FRZGateway
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/Debug
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/Release
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release
)


install(TARGETS FRZGateway
    CONFIGURATIONS Debug Release
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION bin
)