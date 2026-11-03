

set(PROTOBUF_DIR ${PROJECT_SOURCE_DIR}/proto)
set(PROTOBUF_FILES
)

if (PROTOBUF_FILES)
    get_target_property(PROTOC_EXE protobuf::protoc IMPORTED_LOCATION)

    set(GENERATED_SRCS "")
    set(GENERATED_HDRS "")

    foreach(PROTO_FILE ${PROTOBUF_FILES})
        get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)

        set(GENERATED_SRC ${CMAKE_BINARY_DIR}/${PROTO_NAME}.pb.cc)
        set(GENERATED_HDR ${CMAKE_BINARY_DIR}/${PROTO_NAME}.pb.h)

        add_custom_command(
            OUTPUT
                ${GENERATED_SRC}
                ${GENERATED_HDR}
            COMMAND
                ${PROTOC_EXE}
                --proto_path=${PROTOBUF_DIR}
                --cpp_out=${CMAKE_BINARY_DIR}
                ${PROTO_FILE}
            DEPENDS
                ${PROTO_FILE}
            VERBATIM
        )

        list(APPEND GENERATED_SRCS ${GENERATED_SRC})
        list(APPEND GENERATED_HDRS ${GENERATED_HDR})
    endforeach()

    add_library(proto_lib
        ${GENERATED_SRCS}
    )

    target_include_directories(proto_lib
        PUBLIC ${CMAKE_BINARY_DIR}
    )

    target_link_libraries(proto_lib
        PUBLIC conan::protobuf
    )

    add_library(proto::files ALIAS proto_lib)
endif()
