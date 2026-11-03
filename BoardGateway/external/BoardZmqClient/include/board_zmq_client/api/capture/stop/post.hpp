#pragma once

#include "zmq_rpc/json/json.hpp"
#include "board_zmq_client/api/capture/status/get.hpp"
#include <string>
#include <cstdint>
#include <optional>

namespace nooyan::zmq_client::api::capture::stop
{
    struct Post {
        using str_view = std::string_view;
        using str = std::string;

        static constexpr auto path{ "/api/capture/stop" };
        static constexpr auto method{ "POST" };

        using Result = status::Get::Result;
    };
}