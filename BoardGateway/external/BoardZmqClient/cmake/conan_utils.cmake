function(create_conan_profile PROFILE_PATH_OUT)
    # Detect if conan is installed
    find_program(CONAN "conan" REQUIRED)
    
    # Set build type if not defined
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Debug")
    endif()
    
    # Detect compiler type and version
    if(MSVC)
        set(COMPILER_TYPE "msvc")
        if (MSVC_VERSION GREATER_EQUAL 1930)
            set(COMPILER_VERSION 193)
        elseif (MSVC_VERSION GREATER_EQUAL 1920)
            set(COMPILER_VERSION 192)
        elseif (MSVC_VERSION GREATER_EQUAL 1910)
            set(COMPILER_VERSION 191)
        elseif (MSVC_VERSION GREATER_EQUAL 1900)
            set(COMPILER_VERSION 190)
        elseif (MSVC_VERSION GREATER_EQUAL 1800)
            set(COMPILER_VERSION 180)
        else()
            message(FATAL_ERROR "Unsupported MSVC_VERSION: ${MSVC_VERSION}")
        endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(COMPILER_TYPE "clang")
        string(REGEX REPLACE "^([0-9]+)\\.*.*" "\\1" COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(COMPILER_TYPE "gcc")
        execute_process(
            COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
            OUTPUT_VARIABLE COMPILER_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    else()
        message(FATAL_ERROR "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
    endif()

    message(STATUS "Detected compiler: ${COMPILER_TYPE} ${COMPILER_VERSION}")

    # Create a custom profile based on project settings
    message(STATUS "Creating Conan profile based on project settings...")
    
    # Determine OS
    if(WIN32)
        set(OS_NAME "Windows")
    elseif(APPLE)
        set(OS_NAME "Macos")
    else()
        set(OS_NAME "Linux")
    endif()

    # Set compiler-specific libcxx setting
    if(COMPILER_TYPE STREQUAL "gcc")
        set(LIBCXX "libstdc++11")
    elseif(COMPILER_TYPE STREQUAL "clang")
        if(APPLE)
            set(LIBCXX "libc++")
        else()
            set(LIBCXX "libstdc++11")
        endif()
    else()
        # MSVC doesn't need libcxx setting
        set(LIBCXX "")
        # Detect MSVC runtime
        if(MSVC)
            if(CMAKE_MSVC_RUNTIME_LIBRARY MATCHES "MultiThreaded")
                set(MSVC_RUNTIME "static")
            elseif(CMAKE_MSVC_RUNTIME_LIBRARY MATCHES "MultiThreadedDebug")
                set(MSVC_RUNTIME "static")
            elseif(CMAKE_MSVC_RUNTIME_LIBRARY MATCHES "MultiThreadedDLL")
                set(MSVC_RUNTIME "dynamic")
            elseif(CMAKE_MSVC_RUNTIME_LIBRARY MATCHES "MultiThreadedDebugDLL")
                set(MSVC_RUNTIME "dynamic")
            else()
                # Default to MD/MDd based on build type
                set(MSVC_RUNTIME "dynamic")
            endif()
        endif()
    endif()

    # Set the profile path
    set(PROFILE_PATH "${CMAKE_BINARY_DIR}/conan_profile_project")

    # Write the profile file with conditional libcxx and MSVC runtime
    if(MSVC)
        file(WRITE ${PROFILE_PATH}
            "[settings]\n"
            "arch=x86_64\n"
            "build_type=${CMAKE_BUILD_TYPE}\n"
            "compiler=${COMPILER_TYPE}\n"
            "compiler.cppstd=${CMAKE_CXX_STANDARD}\n"
            "compiler.runtime=${MSVC_RUNTIME}\n"
            "compiler.runtime_type=${CMAKE_BUILD_TYPE}\n"
            "compiler.version=${COMPILER_VERSION}\n"
            "os=${OS_NAME}\n"
        )
    elseif(LIBCXX)
        file(WRITE ${PROFILE_PATH}
            "[settings]\n"
            "arch=x86_64\n"
            "build_type=${CMAKE_BUILD_TYPE}\n"
            "compiler=${COMPILER_TYPE}\n"
            "compiler.cppstd=${CMAKE_CXX_STANDARD}\n"
            "compiler.libcxx=${LIBCXX}\n"
            "compiler.version=${COMPILER_VERSION}\n"
            "os=${OS_NAME}\n"
        )
    else()
        file(WRITE ${PROFILE_PATH}
            "[settings]\n"
            "arch=x86_64\n"
            "build_type=${CMAKE_BUILD_TYPE}\n"
            "compiler=${COMPILER_TYPE}\n"
            "compiler.cppstd=${CMAKE_CXX_STANDARD}\n"
            "compiler.version=${COMPILER_VERSION}\n"
            "os=${OS_NAME}\n"
        )
    endif()

    # Set the output variable in parent scope
    set(${PROFILE_PATH_OUT} ${PROFILE_PATH} PARENT_SCOPE)
endfunction()

function(conan_install_command PROFILE_PATH CONAN_FILE_PATH REMOTE)
    find_program(CONAN "conan" REQUIRED)
    
    message(STATUS "Running Conan to install dependencies...")
    execute_process(
        COMMAND ${CONAN} install ${CONAN_FILE_PATH}
            --output-folder=${CMAKE_BINARY_DIR}
            --build=missing
            --profile:host=${PROFILE_PATH}
            --profile:build=${PROFILE_PATH}
            --remote ${REMOTE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE CONAN_INSTALL_RESULT
    )
    if(NOT CONAN_INSTALL_RESULT EQUAL 0)
        message(FATAL_ERROR "Conan install failed")
    endif()
endfunction()

function(get_conan_version CONAN_VERSION_OUT)
    execute_process(
        COMMAND conan --version
        OUTPUT_VARIABLE CONAN_VERSION_OUTPUT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string(REGEX MATCH "[0-9]+\\.[0-9]+" CONAN_VERSION ${CONAN_VERSION_OUTPUT})
    set(${CONAN_VERSION_OUT} ${CONAN_VERSION} PARENT_SCOPE)
endfunction() 

function(conan_add_remote)
    set(options)  # no TRUE/FALSE flags
    set(oneValueArgs NAME INDEX URL VERIFY_SSL)
    set(multiValueArgs)  # nothing multivalued here
    cmake_parse_arguments(CONAN_REMOTE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT CONAN_REMOTE_NAME)
        message(FATAL_ERROR "conan_add_remote: NAME is required")
    endif()
    if(NOT CONAN_REMOTE_URL)
        message(FATAL_ERROR "conan_add_remote: URL is required")
    endif()

    find_program(CONAN "conan" REQUIRED)

    # Build the base command
    set(cmd ${CONAN} remote add ${CONAN_REMOTE_NAME} ${CONAN_REMOTE_URL})

    # # Index is optional — only append if provided
    # if(DEFINED CONAN_REMOTE_INDEX)
    #     list(APPEND cmd --insert ${CONAN_REMOTE_INDEX})
    # endif()

    # # VERIFY_SSL handling
    # if(DEFINED CONAN_REMOTE_VERIFY_SSL)
    #     string(TOLOWER "${CONAN_REMOTE_VERIFY_SSL}" _verify)
    #     if(_verify STREQUAL "false" OR _verify STREQUAL "off" OR _verify STREQUAL "0")
    #         list(APPEND cmd --verify-ssl=False)
    #     elseif(_verify STREQUAL "true" OR _verify STREQUAL "on" OR _verify STREQUAL "1")
    #         list(APPEND cmd --verify-ssl=True)
    #     endif()
    # endif()

    # Try to add; if remote exists, update instead
    execute_process(
        COMMAND ${cmd}
        RESULT_VARIABLE res
        OUTPUT_VARIABLE out
        ERROR_VARIABLE err
    )
    if(res)
        message(STATUS "Remote '${CONAN_REMOTE_NAME}' may already exist — trying update")
        set(update_cmd ${CONAN} remote update ${CONAN_REMOTE_NAME} --url ${CONAN_REMOTE_URL})
        # if(DEFINED CONAN_REMOTE_INDEX)
        #     list(APPEND update_cmd --insert ${CONAN_REMOTE_INDEX})
        # endif()
        # if(DEFINED CONAN_REMOTE_VERIFY_SSL)
        #     string(TOLOWER "${CONAN_REMOTE_VERIFY_SSL}" _verify)
        #     if(_verify STREQUAL "false" OR _verify STREQUAL "off" OR _verify STREQUAL "0")
        #         list(APPEND update_cmd --verify-ssl=False)
        #     elseif(_verify STREQUAL "true" OR _verify STREQUAL "on" OR _verify STREQUAL "1")
        #         list(APPEND update_cmd --verify-ssl=True)
        #     endif()
        # endif()
        execute_process(COMMAND ${update_cmd})
    endif()
endfunction()
