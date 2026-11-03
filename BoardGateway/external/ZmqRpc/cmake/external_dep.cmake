
find_package(Git)
message(STATUS "execute git submodule update")
execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE GIT_SUBMOD_RESULT)
if (NOT GIT_SUBMOD_RESULT EQUAL "0")
    message(FATAL_ERROR "git submodule update failed with ${GIT_SUBMOD_RESULT}")
endif()

# include(ExternalProject)
# ExternalProject_Add(ReceiverModule
#     SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/ReceiverModule
#     BINARY_DIR ${CMAKE_BINARY_DIR}/receiver_build
#     CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
# )
# set(RECEIVERMODULE_INSTALL_DIR ${CMAKE_SOURCE_DIR}/external/ReceiverModule/install)
# set(RECEIVERMODULE_INCLUDE_DIR ${RECEIVERMODULE_INSTALL_DIR}/include)