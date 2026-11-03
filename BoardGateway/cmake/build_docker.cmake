

add_custom_target(BuildDockerImage
    COMMAND docker build 
        --build-arg VERSION=${PROJECT_VERSION} 
        --build-arg SERVICE_EXE_NAME=${PROJECT_NAME} 
        --build-arg SERVICE_TESTS_NAME=${TESTS_TARGET_NAME}
        --build-arg SERVICE_BENCHS_NAME=${BENCHMARK_TARGET_NAME}
        -t ${DOCKER_IMAGE_NAME}:${PROJECT_VERSION} 
        --network=host 
        --add-host repository:172.16.0.30 . 
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)