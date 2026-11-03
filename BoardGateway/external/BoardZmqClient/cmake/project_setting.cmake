
include_directories(include)

if (NOT CMAKE_INSTALL_PREFIX OR STANDALONE_BUILD)
    set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
endif()
set(CMAKE_DEBUG_POSTFIX d)