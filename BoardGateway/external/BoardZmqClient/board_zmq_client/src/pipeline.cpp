#include "board_zmq_client/pipeline.hpp"
#include "zmq_rpc/client_pipeline.hpp"
#include "zmq_rpc/json/json.hpp"
#include <fmt/format.h>
#include <algorithm>
#include <fstream>
#include <exception>

using nooyan::zmq_client::Pipeline;
using zmq_rpc_client_pipeline_t = zmq_rpc::ClientPipeline;

Pipeline& Pipeline::initial_board_client(const str& board_ip, port_t port) {
    auto& ctx{ *_tools.zmq_context.get() };
    auto url{ fmt::format("tcp://{}:{}", board_ip, port) };
    zmq_rpc_client_pipeline_t(_tools.board).initial(ctx, url);
    return *this;
}

Pipeline& Pipeline::initial_capture_client(const str& board_ip, port_t port) {
    auto& ctx{ *_tools.zmq_context.get() };
    auto url{ fmt::format("tcp://{}:{}", board_ip, port) };
    zmq_rpc_client_pipeline_t(_tools.capture).initial(ctx, url);
    return *this;
}

Pipeline& Pipeline::initial_spectrum_client(const str& board_ip, port_t port, size_t channel) {
    auto& ctx{ *_tools.zmq_context.get() };
    auto url{ fmt::format("tcp://{}:{}", board_ip, port) };
    zmq_rpc_client_pipeline_t(_tools.spectrums[channel]).initial(ctx, url);
    return *this;
}

Pipeline& Pipeline::initial_dma_client(const str& board_ip, port_t port, size_t num) {
    auto& ctx{ *_tools.zmq_context.get() };
    auto url{ fmt::format("tcp://{}:{}", board_ip, port) };
    zmq_rpc_client_pipeline_t(_tools.dma[num]).initial(ctx, url);
    return *this;
}

Pipeline& Pipeline::initial_by_config(const Config& config) {
    if (config.board_ip.empty())
        throw std::runtime_error("board ip is not set");
    if (config.ports.board != Config::Default::port)
        initial_board_client(config.board_ip, config.ports.board);
    if (config.ports.capture != Config::Default::port)
        initial_capture_client(config.board_ip, config.ports.capture);
    std::for_each(
        std::begin(config.ports.spectrums), std::end(config.ports.spectrums),
        [&, index = 0](const auto& port) mutable {
            if (port != Config::Default::port)
                initial_spectrum_client(config.board_ip, port, index);
            ++index;
        }
    );

    std::for_each(
        std::begin(config.ports.dma), std::end(config.ports.dma),
        [&, index = 0](const auto& port) mutable {
            if (port != Config::Default::port)
                initial_dma_client(config.board_ip, port, index);
            ++index;
        }
    );
    return *this;
}

Pipeline& Pipeline::initial_by_json_file(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error(fmt::format("can not open file with path {}", path.string()));
    try {
        zmq_rpc::json::json j;
        file >> j;
        Config config;
        j.get_to(config);
        initial_by_config(config);
    } catch (const zmq_rpc::json::json::parse_error& e) {
        throw std::runtime_error(fmt::format(
            "json parse error: {}", e.what()
        ));
    }
    return *this;
}