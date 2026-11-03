#pragma once

#include "zmq_rpc/json/json.hpp"
#include <string>
#include <cstdint>
#include <optional>

namespace nooyan::zmq_client::api::capture::status
{
    struct Get {
        using str_view = std::string_view;
        using str = std::string;

        static constexpr auto path{ "/api/capture/status" };
        static constexpr auto method{ "GET" };

        struct Result {
            struct Default {
                static constexpr auto id{ -1 };
                static constexpr auto adc{ -1 };
                static constexpr auto is_wb{ false };
                static constexpr auto channel_num{ 0 };
                static constexpr auto size{ 0 };
                static constexpr auto freq{ 0 };
                static constexpr auto fs{ 0 };
                static constexpr auto filename{ "" };
                static constexpr auto block_size{ 0 };
                static constexpr auto error{ "" };
                static constexpr auto captured_size{ 0 };
                static constexpr auto drop_size{ 0 };
                static constexpr auto dma_capacity{ 0 };
                static constexpr auto dma_size{ 0 };
                static constexpr auto rate{ 0.0 };
                static constexpr auto progress{ 0.0 };
                static constexpr auto is_run{ false };
            };

            struct Name {
                static constexpr auto id{ "id" };
                static constexpr auto adc{ "adc" };
                static constexpr auto is_wb{ "is_wb" };
                static constexpr auto channel_num{ "channel_num" };
                static constexpr auto size{ "size" };
                static constexpr auto freq{ "freq" };
                static constexpr auto fs{ "fs" };
                static constexpr auto filename{ "filename" };
                static constexpr auto block_size{ "block_size" };
                static constexpr auto error{ "error" };
                static constexpr auto captured_size{ "captured_size" };
                static constexpr auto drop_size{ "drop_size" };
                static constexpr auto dma_capacity{ "dma_capacity" };
                static constexpr auto dma_size{ "dma_size" };
                static constexpr auto rate{ "rate" };
                static constexpr auto progress{ "progress" };
                static constexpr auto is_run{ "is_run" };
            };

            str filename{ Default::filename };
            str error{ Default::error };
            double rate{ Default::rate };
            double progress{ Default::progress };
            std::int64_t id{ Default::id };
            std::int64_t size{ Default::size };
            std::int64_t freq{ Default::freq };
            std::int64_t fs{ Default::fs };
            std::int64_t block_size{ Default::block_size };
            std::int64_t captured_size{ Default::captured_size };
            std::int64_t drop_size{ Default::drop_size };
            std::int64_t dma_capacity{ Default::dma_capacity };
            std::int64_t dma_size{ Default::dma_size };
            int adc{ Default::adc };
            int channel_num{ Default::channel_num };
            bool is_wb{ Default::is_wb };
            bool is_run{ Default::is_run };
        };
    };

    static void from_json(const nlohmann::json& j, Get::Result& result) {
        using result_t = Get::Result;

        zmq_rpc::json::get_from_json_to(j, result_t::Name::id, result.id);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::adc, result.adc);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::is_wb, result.is_wb);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::channel_num, result.channel_num);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::size, result.size);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::freq, result.freq);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::fs, result.fs);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::filename, result.filename);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::block_size, result.block_size);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::error, result.error);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::captured_size, result.captured_size);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::drop_size, result.drop_size);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::dma_capacity, result.dma_capacity);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::dma_size, result.dma_size);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::rate, result.rate);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::progress, result.progress);
        zmq_rpc::json::get_from_json_to(j, result_t::Name::is_run, result.is_run);
    }
}