#pragma once

#include <zmq.hpp>
#include "board_zmq_client/tools.hpp"
#include "board_gw/frz/conn_config.hpp"
#include "board_zmq_client/api/capture/start/post.hpp"
#include "board_zmq_client/api/capture/stop/post.hpp"
#include "board_zmq_client/api/capture/status/get.hpp"
#include "board_zmq_client/api/capture/device/info/get.hpp"
#include <memory>

namespace pooyan::board::frz
{
    class CaptureApis {
    public:
        using zmq_context_t = zmq::context_t;
        using tools_t = nooyan::zmq_client::Tools;
        using conn_config_t = ConnConfig;

        struct Start {
            using data_t = nooyan::zmq_client::api::capture::start::Post::Data;
            using response_t = nooyan::zmq_client::api::capture::start::Post::Result;
        };

        struct Stop {
            using response_t = nooyan::zmq_client::api::capture::stop::Post::Result;
        };

        struct Status {
            using response_t = nooyan::zmq_client::api::capture::status::Get::Result;
        };

        struct Device {
            using response_t = nooyan::zmq_client::api::capture::device::info::Get::Result;
        };

        CaptureApis(std::shared_ptr<zmq_context_t> zmq_ctx)
            : _tools{ zmq_ctx } {}

        void initial(const conn_config_t& config);
        Start::response_t start(const Start::data_t& data, int timeout = -1);
        Stop::response_t stop(int timeout = -1);
        Status::response_t status(int timeout = -1);
        Device::response_t device_info(int timeout = -1);
    private:
        tools_t _tools;
    };
}