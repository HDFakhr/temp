
set(OLD_ENV_FILE 
    ${CMAKE_BINARY_DIR}/bin/Debug/.env
    ${CMAKE_BINARY_DIR}/bin/Release/.env)
if (OLD_ENV_FILE)
    file(REMOVE ${OLD_ENV_FILE})
endif()


set(ENV_FILE ${PROJECT_SOURCE_DIR}/.env)
file(COPY ${ENV_FILE} DESTINATION ${CMAKE_BINARY_DIR}/bin/Debug)
file(COPY ${ENV_FILE} DESTINATION ${CMAKE_BINARY_DIR}/bin/Release)