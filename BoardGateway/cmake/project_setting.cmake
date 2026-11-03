
include_directories(include)
include_directories(include/board_gw/frz)

set(SERVICE_NAMESPACE_PREFIX srv)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
set(CMAKE_DEBUG_POSTFIX d)

set(ENV_VARIABLES_PREFIX "SRV")
set(DEBUG_ENV_FILE ".env" CACHE STRING "DEBUG_ENV_FILE")