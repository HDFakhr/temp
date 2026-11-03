#pragma once

#include "zmq_rpc/json/json.hpp"
#include <string>
#include <cstdint>
#include <vector>

namespace nooyan::zmq_client::api::board::ok
{
    struct Get {
        using str_view = std::string_view;
        using str = std::string;

        static constexpr auto path{ "/api/board/ok" };
        static constexpr auto method{ "GET" };

        using Result = std::string;
    };
}