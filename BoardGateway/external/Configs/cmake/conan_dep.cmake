

get_conan_version(CONAN_VERSION)

option(CONAN_REMOTE_ADD "add conan remote" ON)
set(CONAN_REMOTE conan-repository CACHE STRING "conan remote name")
set(CONAN_REMOTE_URL http://conan-repository:8081/artifactory/api/conan/conan-local CACHE STRING "conan remote url")

if (CONAN_VERSION VERSION_GREATER_EQUAL "2.0")
    set(CONAN_PROFILE_PATH "")
    if (CONAN_REMOTE_ADD)
        message(STATUS "add conan remote")
        conan_add_remote(
            NAME ${CONAN_REMOTE}
            INDEX 0
            URL ${CONAN_REMOTE_URL}
            VERIFY_SSL True
        )
    endif()
    create_conan_profile(CONAN_PROFILE_PATH)
    conan_install_command(${CONAN_PROFILE_PATH} ${CMAKE_SOURCE_DIR}/conanfile.txt ${CONAN_REMOTE})

    if (BUILD_WITH_TESTS OR BUILD_WITH_BENCHMARKS)
        conan_install_command(${CONAN_PROFILE_PATH} ${CMAKE_SOURCE_DIR}/conanfile_dev.txt ${CONAN_REMOTE})
    endif()

    include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake OPTIONAL RESULT_VARIABLE CONAN_TOOLCHAIN_INCLUDED)
    if (NOT CONAN_TOOLCHAIN_INCLUDED)
        message(FATAL_ERROR "Conan toolchain file not found. Conan install may have failed")
    endif()
else()
    message(FATAL_ERROR "Conan Version 2.0 or higher is required. Found version: ${CONAN_VERSION}")
endif()
