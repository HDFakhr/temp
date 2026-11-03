

get_conan_version(CONAN_VERSION)

if (CONAN_VERSION VERSION_GREATER_EQUAL "2.0")
    set(CONAN_PROFILE_PATH "")
    conan_add_remote(
        NAME conan-repository
        INDEX 0
        URL "http://conan-repository:8081/artifactory/api/conan/conan-local"
        VERIFY_SSL True
    )
    create_conan_profile(CONAN_PROFILE_PATH)
    conan_install_command(${CONAN_PROFILE_PATH} ${CMAKE_SOURCE_DIR}/conanfile.txt conan-repository)

    if (BUILD_WITH_TESTS OR BUILD_WITH_BENCHMARKS)
        conan_install_command(${CONAN_PROFILE_PATH} ${CMAKE_SOURCE_DIR}/conanfile_dev.txt conan-repository)
    endif()

    include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake OPTIONAL RESULT_VARIABLE CONAN_TOOLCHAIN_INCLUDED)
    if (NOT CONAN_TOOLCHAIN_INCLUDED)
        message(FATAL_ERROR "Conan toolchain file not found. Conan install may have failed")
    endif()
else()
    message(FATAL_ERROR "Conan Version 2.0 or higher is required. Found version: ${CONAN_VERSION}")
endif()
