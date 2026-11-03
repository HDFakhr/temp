#pragma once

#include "zmq_rpc/json/json.hpp"
#include <string>
#include <cstdint>
#include <optional>

namespace nooyan::zmq_client::api::capture::device::info
{
    struct Get {
        using str_view = std::string_view;
        using str = std::string;

        static constexpr auto path{ "/api/capture/device/info" };
        static constexpr auto method{ "GET" };

        struct Info {
            struct Default {
                static constexpr auto name{ "" };
                static constexpr auto addr{ "" };
                static constexpr auto index{ -1 };
                static constexpr auto total_space{ 0 };
                static constexpr auto free_space{ 0 };
                static constexpr auto block_size{ 0 };
            };

            struct Name {
                static constexpr auto name{ "name" };
                static constexpr auto addr{ "addr" };
                static constexpr auto index{ "index" };
                static constexpr auto total_space{ "total_space" };
                static constexpr auto free_space{ "free_space" };
                static constexpr auto block_size{ "block_size" };
            };

            str name{ Default::name };
            str addr{ Default::addr };
            std::int64_t total_space{ Default::total_space };
            std::int64_t free_space{ Default::free_space };
            std::int64_t block_size{ Default::block_size };
            int index{ Default::index };
        };

        using Result = std::vector<Info>;
    };

    static void from_json(const nlohmann::json& j, Get::Info& info) {
        using info_t = Get::Info;

        zmq_rpc::json::get_from_json_to(j, info_t::Name::name, info.name);
        zmq_rpc::json::get_from_json_to(j, info_t::Name::addr, info.addr);
        zmq_rpc::json::get_from_json_to(j, info_t::Name::index, info.index);
        zmq_rpc::json::get_from_json_to(j, info_t::Name::total_space, info.total_space);
        zmq_rpc::json::get_from_json_to(j, info_t::Name::free_space, info.free_space);
        zmq_rpc::json::get_from_json_to(j, info_t::Name::block_size, info.block_size);
    }
}