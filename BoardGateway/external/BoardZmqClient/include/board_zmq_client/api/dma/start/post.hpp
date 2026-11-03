#pragma once

#include "zmq_rpc/json/json.hpp"
#include <string>
#include <cstdint>
#include <optional>

namespace nooyan::zmq_client::api::dma::start
{
    struct Post {
        using str_view = std::string_view;
        using str = std::string;

        static constexpr auto path{ "/api/dma/start" };
        static constexpr auto method{ "POST" };

        struct Data {
            struct Default {
                static constexpr std::int64_t size{ -1 };
            };

            struct Name {
                static constexpr auto size{ "size" };
            };

            std::int64_t size{ Default::size };
        };

        struct Result {
            struct Default {
                static constexpr std::int64_t size{ -1 };
                static constexpr std::int64_t max_size{ -1 };
            };

            struct Name {
                static constexpr auto size{ "size" };
                static constexpr auto max_size{ "max_size" };
            };

            std::int64_t size{ Default::size };
            std::int64_t max_size{ Default::max_size };
        };
    };

    static void to_json(nlohmann::json& j, const Post::Data& data) {
        using data_t = Post::Data;
        zmq_rpc::json::add_to_json(j, data_t::Name::size, data.size, data_t::Default::size);
    }

    static void from_json(const nlohmann::json& j, Post::Result& result) {
        using result_t = Post::Result;

        zmq_rpc::json::get_from_json_to(j, result_t::Name::size, result.size);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::max_size, result.max_size);
    }
}