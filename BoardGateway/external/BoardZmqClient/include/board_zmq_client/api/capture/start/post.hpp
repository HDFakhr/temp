#pragma once

#include "zmq_rpc/json/json.hpp"
#include <string>
#include <cstdint>
#include <optional>

namespace nooyan::zmq_client::api::capture::start
{
    struct Post {
        using str_view = std::string_view;
        using str = std::string;

        static constexpr auto path{ "/api/capture/start" };
        static constexpr auto method{ "POST" };

        struct Channel {
            static constexpr auto none{ "" };
            static constexpr auto wb{ "wb" };
            static constexpr auto nb{ "nb" };
        };

        struct Data {
            struct Default {
                static constexpr auto device_index{ -1 };
                static constexpr auto adc{ -1 };
                static constexpr auto channel{ Channel::none };
                static constexpr auto channel_num{ -1 };
                static constexpr auto size{ 0 };
                static constexpr auto freq{ 0 };
                static constexpr auto fs{ 0 };
            };

            struct Name {
                static constexpr auto device_index{ "device_index" };
                static constexpr auto adc{ "adc" };
                static constexpr auto channel{ "channel" };
                static constexpr auto channel_num{ "channel_num" };
                static constexpr auto size{ "size" };
                static constexpr auto freq{ "freq" };
                static constexpr auto fs{ "fs" };
            };

            str channel{ Default::channel };
            std::int64_t size{ Default::size };
            std::int64_t freq{ Default::freq };
            std::int64_t fs{ Default::fs };
            int device_index{ Default::device_index };
            int adc{ Default::adc };
            int channel_num{ Default::channel_num };
        };

        struct Result {
            struct Default {
                static constexpr auto capture_id{ -1 };
                static constexpr auto adc{ -1 };
                static constexpr auto is_wb{ false };
                static constexpr auto channel_num{ -1 };
                static constexpr auto size{ 0 };
                static constexpr auto freq{ 0 };
                static constexpr auto fs{ 0 };
                static constexpr auto filename{ "" };
                static constexpr auto block_size{ 0 };
            };

            struct Name {
                static constexpr auto capture_id{ "capture_id" };
                static constexpr auto adc{ "adc" };
                static constexpr auto is_wb{ "is_wb" };
                static constexpr auto channel_num{ "channel_num" };
                static constexpr auto size{ "size" };
                static constexpr auto freq{ "freq" };
                static constexpr auto fs{ "fs" };
                static constexpr auto filename{ "filename" };
                static constexpr auto block_size{ "block_size" };
            };

            str filename{ Default::filename };
            std::int64_t size{ Default::size };
            std::int64_t freq{ Default::freq };
            std::int64_t fs{ Default::fs };
            std::int64_t capture_id{ Default::capture_id };
            std::int64_t block_size{ Default::block_size };
            int adc{ Default::adc };
            int channel_num{ Default::channel_num };
            bool is_wb{ Default::is_wb };
        };
    };

    static void to_json(nlohmann::json& j, const Post::Data& data) {
        using data_t = Post::Data;
        zmq_rpc::json::add_to_json(j, data_t::Name::device_index, data.device_index, data_t::Default::device_index);
        zmq_rpc::json::add_to_json(j, data_t::Name::adc, data.adc, data_t::Default::adc);
        zmq_rpc::json::add_to_json(j, data_t::Name::channel, data.channel, std::string(data_t::Default::channel));
        zmq_rpc::json::add_to_json(j, data_t::Name::channel_num, data.channel_num, data_t::Default::channel_num);
        zmq_rpc::json::add_to_json(j, data_t::Name::size, data.size, static_cast<std::int64_t>(data_t::Default::size));
        zmq_rpc::json::add_to_json(j, data_t::Name::freq, data.freq, static_cast<std::int64_t>(data_t::Default::freq));
        zmq_rpc::json::add_to_json(j, data_t::Name::fs, data.fs, static_cast<std::int64_t>(data_t::Default::fs));

    }

    static void from_json(const nlohmann::json& j, Post::Result& result) {
        using result_t = Post::Result;

        zmq_rpc::json::get_from_json_to(j, result_t::Name::capture_id, result.capture_id);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::adc, result.adc);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::is_wb, result.is_wb);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::channel_num, result.channel_num);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::size, result.size);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::freq, result.freq);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::fs, result.fs);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::filename, result.filename);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::block_size, result.block_size);
    }
}