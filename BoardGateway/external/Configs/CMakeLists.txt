cmake_minimum_required(VERSION 3.18)

project(Configs VERSION "3.1.0" LANGUAGES CXX)
set(TESTS_TARGET_NAME ${PROJECT_NAME}Tests)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(cmake/project_setting.cmake)
include(cmake/conan_utils.cmake)
include(cmake/conan_dep.cmake)

if (STANDALONE_BUILD)
    include(cmake/external_dep.cmake)
endif()

if (BUILD_WITH_TESTS)
    add_subdirectory(tests)
endif()

# dont needs in header-only project
install(CODE "")
