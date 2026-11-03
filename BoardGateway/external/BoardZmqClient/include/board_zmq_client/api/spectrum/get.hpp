#pragma once

#include "zmq_rpc/json/json.hpp"
#include <string>
#include <cstdint>
#include <vector>

namespace nooyan::zmq_client::api::spectrum
{
    struct Get {
        using str_view = std::string_view;
        using str = std::string;

        static constexpr auto path{ "/api/spectrum" };
        static constexpr auto method{ "GET" };

        struct Result {
            std::vector<float> data;
            float start{ 0.0f };
            float step{ 0.0f };
        };
    };
}