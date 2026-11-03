ARG PROJECT_DIR=/root/BoardGateway
ARG INSTALL_DIR=/usr/local/bin

FROM ubuntu_with_ecal_builder:jammy AS build
ARG PROJECT_DIR
COPY . ${PROJECT_DIR}
WORKDIR ${PROJECT_DIR}
RUN mkdir build \
&& cmake -Bbuild -S. -DCMAKE_BUILD_TYPE:STRING=Release \
&& cmake --build build --target=all \
&& cmake --build build --target=install

FROM ubuntu_with_ecal:22.04 AS final
ARG PROJECT_DIR
ARG INSTALL_DIR
ARG VERSION
ARG SERVICE_EXE_NAME
ENV SERVICE_PATH=${INSTALL_DIR}/${SERVICE_EXE_NAME}
LABEL version=${VERSION}
COPY --from=build ${PROJECT_DIR}/install/bin/${SERVICE_EXE_NAME} ${SERVICE_PATH}
RUN cat <<EOF > /etc/supervisor/conf.d/inet_http_server.conf
[inet_http_server]
port=*:%(ENV_SUPERVISOR_INET_PORT)s
username=%(ENV_SUPERVISOR_INET_USERNAME)s
password=%(ENV_SUPERVISOR_INET_PASSWORD)s
EOF
RUN cat <<EOF > /etc/supervisor/conf.d/${SERVICE_EXE_NAME}.conf
[program:${SERVICE_EXE_NAME}]
command=/usr/local/bin/${SERVICE_EXE_NAME}
process_name=${SERVICE_EXE_NAME}
autostart=true
autorestart=true
stderr_logfile=%(ENV_SUPERVISOR_LOG_DIR)s/${SERVICE_EXE_NAME}_errors.log
stdout_logfile=%(ENV_SUPERVISOR_LOG_DIR)s/${SERVICE_EXE_NAME}_stdout.log
user=root
stopasgroup=true
stopsignal=QUIT
EOF

ENTRYPOINT ["/bin/sh", "-c", "/bin/supervisord -c /etc/supervisor/supervisord.conf && /bin/bash"]