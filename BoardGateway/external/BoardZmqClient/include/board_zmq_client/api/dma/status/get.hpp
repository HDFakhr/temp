#pragma once

#include "zmq_rpc/json/json.hpp"
#include <string>
#include <cstdint>
#include <optional>

namespace nooyan::zmq_client::api::dma::status
{
    struct Get {
        using str_view = std::string_view;
        using str = std::string;

        static constexpr auto path{ "/api/dma/status" };
        static constexpr auto method{ "GET" };

        struct Result {
            struct Default {
                static constexpr std::int64_t size{ -1 };
                static constexpr std::int64_t bulk_size{ -1 };
                static constexpr std::int64_t max_size{ -1 };
            };

            struct Name {
                static constexpr auto size{ "size" };
                static constexpr auto bulk_size{ "bulk_size" };
                static constexpr auto max_size{ "max_size" };
            };

            std::int64_t size{ Default::size };
            std::int64_t bulk_size{ Default::bulk_size }; 
            std::int64_t max_size{ Default::max_size };
        };
    };

    static void from_json(const nlohmann::json& j, Get::Result& result) {
        using result_t = Get::Result;

        zmq_rpc::json::get_from_json_to(j, result_t::Name::size, result.size);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::bulk_size, result.bulk_size);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::max_size, result.max_size);
    }
}