#pragma once

#include "zmq_rpc/json/json.hpp"

namespace nooyan::zmq_client::api::board::nco
{
    struct Post {
        static constexpr auto path{ "/api/board/nco" };
        static constexpr auto method{ "POST" };

        struct Data {
            struct Default {
                static constexpr int channel{ -1 };
                static constexpr double nco{ 0.0 };
            };

            struct Name {
                static constexpr auto channel{ "channel" };
                static constexpr auto nco{ "nco" };
            };

            int channel{ Default::channel };
            double nco{ Default::nco };
        };

        struct Result {
            struct Default {
                static constexpr int channel{ -1 };
                static constexpr double nco{ 0.0 };
            };

            struct Name {
                static constexpr auto channel{ "channel" };
                static constexpr auto nco{ "nco" };
            };

            int channel{ Default::channel };
            double nco{ Default::nco };
        };
    };

    static void to_json(nlohmann::json& j, const Post::Data& data) {
        using data_t = Post::Data;
        zmq_rpc::json::add_to_json(j, data_t::Name::channel, data.channel, data_t::Default::channel);
        zmq_rpc::json::add_to_json(j, data_t::Name::nco, data.nco, data_t::Default::nco);
    }

    static void from_json(const nlohmann::json& j, Post::Result& result) {
        using result_t = Post::Result;

        zmq_rpc::json::get_from_json_to(j, result_t::Name::channel, result.channel);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::nco, result.nco);
    }
}