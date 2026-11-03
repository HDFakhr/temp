

add_library(ZmqRpcLib INTERFACE)
if (STANDALONE_BUILD)
    add_dependencies(ZmqRpcLib ZmqRpc)
endif()
target_include_directories(ZmqRpcLib
    INTERFACE ${ZMQRPC_INCLUDE_DIR}
)
target_link_libraries(ZmqRpcLib
    INTERFACE ${ZMQRPC_LIB}
    INTERFACE ${ZMQ_COMPONENTS}
    INTERFACE ${NLOHMANN_JSON_COMPONENTS}
)