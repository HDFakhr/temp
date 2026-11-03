#pragma once

#include "board_zmq_client/tools.hpp"
#include "board_zmq_client/config.hpp"
#include <string>
#include <filesystem>

namespace nooyan::zmq_client
{
    class Pipeline {
    public:
        using str = std::string;
        using size_t = std::size_t;
        using tools_t = Tools;
        using port_t = unsigned short;

        tools_t& _tools;

        explicit Pipeline(tools_t& tools) : _tools{ tools } {}

        Pipeline& initial_board_client(const str& board_ip, port_t port);
        Pipeline& initial_capture_client(const str& board_ip, port_t port);
        Pipeline& initial_spectrum_client(const str& board_ip, port_t port, size_t channel = 0);
        Pipeline& initial_dma_client(const str& board_ip, port_t port, size_t num = 0);
        Pipeline& initial_by_config(const Config& config);
        Pipeline& initial_by_json_file(const std::filesystem::path& path);
    };
}