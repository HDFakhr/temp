#pragma once

#include "zmq_rpc/json/json.hpp"

namespace nooyan::zmq_client::api::board::adc
{
    struct Post {
        static constexpr auto path{ "/api/board/adc" };
        static constexpr auto method{ "POST" };

        struct Data {
            struct Default {
                static constexpr std::int64_t adc_freq{ -1 };
                static constexpr int adc_prec{ -1 };
                static constexpr int adc{ -1 };
            };

            struct Name {
                static constexpr auto adc_freq{ "adc_freq" };
                static constexpr auto adc_prec{ "adc_prec" };
                static constexpr auto adc{ "adc" };
            };

            std::int64_t adc_freq{ Default::adc_freq };
            int adc_prec{ Default::adc_prec };
            int adc{ Default::adc };
        };

        struct Result {
            struct Default {
                static constexpr std::int64_t adc_freq{ -1 };
                static constexpr std::int64_t fs{ -1 };
                static constexpr std::int64_t nco{ -1 };
                static constexpr int adc{ -1 };
                static constexpr int adc_prec{ -1 };
            };

            struct Name {
                static constexpr auto adc{ "adc" };
                static constexpr auto adc_freq{ "adc_freq" };
                static constexpr auto adc_prec{ "adc_prec" };
                static constexpr auto fs{ "fs" };
                static constexpr auto nco{ "nco" };
            };

            std::int64_t adc_freq{ Default::adc_freq };
            std::int64_t fs{ Default::fs };
            std::int64_t nco{ Default::nco };
            int adc{ Default::adc };
            int adc_prec{ Default::adc_prec };
        };
    };

    static void to_json(nlohmann::json& j, const Post::Data& data) {
        using data_t = Post::Data;
        zmq_rpc::json::add_to_json(j, data_t::Name::adc_freq, data.adc_freq, data_t::Default::adc_freq);
        zmq_rpc::json::add_to_json(j, data_t::Name::adc_prec, data.adc_prec, data_t::Default::adc_prec);
        zmq_rpc::json::add_to_json(j, data_t::Name::adc, data.adc, data_t::Default::adc);
    }

    static void from_json(const nlohmann::json& j, Post::Result& result) {
        using result_t = Post::Result;

        zmq_rpc::json::get_from_json_to(j, result_t::Name::adc_freq, result.adc_freq);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::fs, result.fs);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::nco, result.nco);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::adc, result.adc);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::adc_prec, result.adc_prec);
    }
}