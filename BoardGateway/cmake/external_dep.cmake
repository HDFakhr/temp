
if (UPDATE_GIT_SUBMODULE)
    find_package(Git)
    message(STATUS "execute git submodule update")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if (NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update failed with ${GIT_SUBMOD_RESULT}")
    endif()
endif()

# ExternalProject_Add(ReceiverModule
#     SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/ReceiverModule
#     BINARY_DIR ${CMAKE_BINARY_DIR}/receiver_build
#     CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
# )
# set(RECEIVERMODULE_INSTALL_DIR ${CMAKE_SOURCE_DIR}/external/ReceiverModule/install)
# set(RECEIVERMODULE_INCLUDE_DIR ${RECEIVERMODULE_INSTALL_DIR}/include)

include(ExternalProject)

### -------------------------- 'Configs' ----------------------------------
set(CONFIGS_COMPONENT Configs)
set(CONFIGS_DIR ${CMAKE_SOURCE_DIR}/external/Configs)
set(CONFIGS_INCLUDE_DIR ${CONFIGS_DIR}/include)

ExternalProject_Add(${CONFIGS_COMPONENT}
    SOURCE_DIR ${CONFIGS_DIR}
    BINARY_DIR ${CMAKE_BINARY_DIR}/${CONFIGS_COMPONENT}_build
    CONFIGURE_COMMAND "" BUILD_COMMAND "" INSTALL_COMMAND ""
)
add_definitions(-DYAML_ENABLE=false -DJSON_ENABLE=false -DENV_ENABLE=false -DARGS_ENABLE=true)
### -----------------------------------------------------------------------


### -------------------------- 'ZmqRpc' -----------------------------------
set(ZMQRPC_COMPONENT ZmqRpc)
set(ZMQRPC_DIR ${CMAKE_SOURCE_DIR}/external/ZmqRpc)
set(ZMQRPC_INSTALL_DIR ${ZMQRPC_DIR}/install)
set(ZMQRPC_INCLUDE_DIR ${ZMQRPC_INSTALL_DIR}/include)
set(ZMQRPC_LIB_DIR ${ZMQRPC_INSTALL_DIR}/lib)
if (WIN32)
    set(ZMQRPC_LIB ${ZMQRPC_LIB_DIR}/ZMQRpc$<$<CONFIG:Debug>:d>.lib)
else()
    set(ZMQRPC_LIB ${ZMQRPC_LIB_DIR}/libZMQRpc$<$<CONFIG:Debug>:d>.a)
endif()

ExternalProject_Add(${ZMQRPC_COMPONENT}
    SOURCE_DIR ${ZMQRPC_DIR}
    BINARY_DIR ${CMAKE_BINARY_DIR}/${ZMQRPC_COMPONENT}_build
    CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=${ZMQRPC_INSTALL_DIR}
        -DCMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}
        -DCMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}
)

set(ZMQRPC_PACKAGE ${ZMQRPC_COMPONENT}Package)
add_library(${ZMQRPC_PACKAGE} INTERFACE)
add_dependencies(${ZMQRPC_PACKAGE} ${ZMQRPC_COMPONENT})
target_include_directories(${ZMQRPC_PACKAGE} INTERFACE ${ZMQRPC_INCLUDE_DIR})
target_link_libraries(${ZMQRPC_PACKAGE}
    INTERFACE ${ZMQRPC_LIB}
    INTERFACE conan::cppzmq
    INTERFACE conan::nlohmann_json)
### -----------------------------------------------------------------------

### -------------------------- 'BoardZmqClient' ---------------------------
set(BOARDZMQCLIENT_COMPONENT BoardZmqClient)
set(BOARDZMQCLIENT_DIR ${CMAKE_SOURCE_DIR}/external/BoardZmqClient)
set(BOARDZMQCLIENT_INSTALL_DIR ${BOARDZMQCLIENT_DIR}/install)
set(BOARDZMQCLIENT_INCLUDE_DIR ${BOARDZMQCLIENT_INSTALL_DIR}/include)
set(BOARDZMQCLIENT_LIB_DIR ${BOARDZMQCLIENT_INSTALL_DIR}/lib)
if (WIN32)
    set(BOARDZMQCLIENT_LIB ${BOARDZMQCLIENT_LIB_DIR}/BoardZmqClient$<$<CONFIG:Debug>:d>.lib)
else()
    set(BOARDZMQCLIENT_LIB ${BOARDZMQCLIENT_LIB_DIR}/libBoardZmqClient$<$<CONFIG:Debug>:d>.a)
endif()

ExternalProject_Add(${BOARDZMQCLIENT_COMPONENT}
    DEPENDS ${ZMQRPC_COMPONENT}
    SOURCE_DIR ${BOARDZMQCLIENT_DIR}
    BINARY_DIR ${CMAKE_BINARY_DIR}/${BOARDZMQCLIENT_COMPONENT}_build
    CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=${BOARDZMQCLIENT_INSTALL_DIR}
        -DCMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}
        -DCMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}
        -DZMQRPC_INCLUDE_DIR=${ZMQRPC_INCLUDE_DIR}
)

add_library(BoardZmqClientLib INTERFACE)
add_dependencies(BoardZmqClientLib ${BOARDZMQCLIENT_COMPONENT})
target_include_directories(BoardZmqClientLib INTERFACE ${BOARDZMQCLIENT_INCLUDE_DIR})
target_link_libraries(BoardZmqClientLib
    INTERFACE ${BOARDZMQCLIENT_LIB}
    INTERFACE ${ZMQRPC_PACKAGE}
    #INTERFACE conan::cppzmq
    )
### -----------------------------------------------------------------------
