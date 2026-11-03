#pragma once

#include "zmq_rpc/json/json.hpp"

namespace nooyan::zmq_client::api::board::attenuation
{
    struct Post {
        static constexpr auto path{ "/api/board/attenuation" };
        static constexpr auto method{ "POST" };

        struct Data {
            struct Default {
                static constexpr int channel{ -1 };
                static constexpr int attenuation{ 0 };
            };

            struct Name {
                static constexpr auto channel{ "channel" };
                static constexpr auto attenuation{ "attenuation" };
            };

            int channel{ Default::channel };
            int attenuation{ Default::attenuation };
        };

        struct Result {
            struct Default {
                static constexpr int channel{ -1 };
                static constexpr int attenuation{ 0 };
            };

            struct Name {
                static constexpr auto channel{ "channel" };
                static constexpr auto attenuation{ "attenuation" };
            };

            int channel{ Default::channel };
            int attenuation{ Default::attenuation };
        };
    };

    static void to_json(nlohmann::json& j, const Post::Data& data) {
        using data_t = Post::Data;
        zmq_rpc::json::add_to_json(j, data_t::Name::channel, data.channel, data_t::Default::channel);
        zmq_rpc::json::add_to_json(j, data_t::Name::attenuation, data.attenuation, data_t::Default::attenuation);
    }

    static void from_json(const nlohmann::json& j, Post::Result& result) {
        using result_t = Post::Result;

        zmq_rpc::json::get_from_json_to(j, result_t::Name::channel, result.channel);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::attenuation, result.attenuation);
    }
}