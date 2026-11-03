#include "board_gw/frz/capture_apis.hpp"
#include "board_zmq_client/pipeline.hpp"
#include "board_zmq_client/api/capture/start/handler.hpp"
#include "board_zmq_client/api/capture/stop/handler.hpp"
#include "board_zmq_client/api/capture/status/handler.hpp"
#include "board_zmq_client/api/capture/device/info/handler.hpp"

using pooyan::board::frz::CaptureApis;

void CaptureApis::initial(const conn_config_t& config) {
    nooyan::zmq_client::Pipeline(_tools)
        .initial_capture_client(config.ip, config.port);
}

auto CaptureApis::start(const Start::data_t& data, int timeout) -> Start::response_t {
    if (!_tools.capture.socket)
        throw std::runtime_error("capture apis is not initialized");
    return nooyan::zmq_client::api::capture::start::Handler(_tools.capture)
        .post(data, timeout);
}

auto CaptureApis::stop(int timeout) -> Stop::response_t {
    if (!_tools.capture.socket)
        throw std::runtime_error("capture apis is not initialized");
    return nooyan::zmq_client::api::capture::stop::Handler(_tools.capture)
        .post(timeout);
}

auto CaptureApis::status(int timeout) -> Status::response_t {
    if (!_tools.capture.socket)
        throw std::runtime_error("capture apis is not initialized");
    return nooyan::zmq_client::api::capture::status::Handler(_tools.capture)
        .get(timeout);   
}

auto CaptureApis::device_info(int timeout) -> Device::response_t {
    if (!_tools.capture.socket)
        throw std::runtime_error("capture apis is not initialized");
    return nooyan::zmq_client::api::capture::device::info::Handler(_tools.capture)
        .get(timeout);
}