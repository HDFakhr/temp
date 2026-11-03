#pragma once

#include <cstdlib>
#include <cstdint>
#include <string>
#include <vector>
#include "zmq_rpc/json/json.hpp"

namespace nooyan::zmq_client
{
    struct Config {
        using str = std::string;
        using size_t = std::size_t;
        using port_t = unsigned short;

        struct Default {
            static constexpr auto ip{ "" };
            static constexpr port_t port{ 0 };
        };

        struct Ports {
            std::vector<port_t> dma{};
            std::vector<port_t> spectrums{};
            port_t board{ Default::port };
            port_t capture{ Default::port };
        };

        str board_ip{ Default::ip };
        Ports ports{};
    };

    static void from_json(const zmq_rpc::json::json& j, Config::Ports& ports) {
        zmq_rpc::json::get_from_json_to(j, "board", ports.board);
        zmq_rpc::json::get_from_json_to(j, "capture", ports.capture);
        zmq_rpc::json::get_from_json_to(j, "spectrum", ports.spectrums);
        zmq_rpc::json::get_from_json_to(j, "dma", ports.dma);
    }

    static void from_json(const zmq_rpc::json::json& j, Config& config) {
        zmq_rpc::json::get_from_json_to(j, "board_ip", config.board_ip);
        zmq_rpc::json::get_from_json_to(j, "ports", config.ports);
    }

    static void to_json(zmq_rpc::json::json& j, const Config::Ports& ports) {
        zmq_rpc::json::add_to_json(j, "board", ports.board);
        zmq_rpc::json::add_to_json(j, "capture", ports.capture);
        zmq_rpc::json::add_to_json(j, "spectrum", ports.spectrums);
        zmq_rpc::json::add_to_json(j, "dma", ports.dma);
    }

    static void to_json(zmq_rpc::json::json& j, const Config& config) {
        zmq_rpc::json::add_to_json(j, "board_ip", config.board_ip);
        zmq_rpc::json::add_to_json(j, "ports", config.ports);
    }
}