

add_subdirectory(frz)


add_library(FRZGatewayLib INTERFACE)
add_dependencies(FRZGatewayLib FRZGateway)
target_include_directories(FRZGatewayLib INTERFACE include/board_gw/frz)
target_link_libraries(FRZGatewayLib
    INTERFACE FRZGateway
    INTERFACE BoardZmqClientLib
    INTERFACE conan::fmt
    )
