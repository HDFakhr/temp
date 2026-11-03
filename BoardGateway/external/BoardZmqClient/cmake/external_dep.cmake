
if (UPDATE_GIT_SUBMODULES)
    find_package(Git)
    message(STATUS "execute git submodule update")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if (NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update failed with ${GIT_SUBMOD_RESULT}")
    endif()
endif()

include(ExternalProject)

set(ZMQRPC_INSTALL_DIR ${CMAKE_SOURCE_DIR}/external/ZmqRpc/install)
ExternalProject_Add(ZmqRpc
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/ZmqRpc
    BINARY_DIR ${CMAKE_BINARY_DIR}/zmq_rpc_build
    CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=${ZMQRPC_INSTALL_DIR}
        -DCMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}
        -DCMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}
)
set(ZMQRPC_INCLUDE_DIR ${ZMQRPC_INSTALL_DIR}/include)
set(ZMQRPC_LIB_DIR ${ZMQRPC_INSTALL_DIR}/lib)
if (WIN32)
    set(ZMQRPC_LIB ${ZMQRPC_LIB_DIR}/ZMQRpc$<$<CONFIG:Debug>:d>.lib)
else()
    set(ZMQRPC_LIB ${ZMQRPC_LIB_DIR}/libZMQRpc$<$<CONFIG:Debug>:d>.a)
endif()