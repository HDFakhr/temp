#include "board_gw/frz/config_handler.hpp"
#include "zmq_rpc/json/json.hpp"
#include <fstream>
#include <fmt/format.h>

using pooyan::board::frz::ConfigHandler;

auto ConfigHandler::load_from_json(const std::filesystem::path& path) -> Config {
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error(fmt::format("can not open file with path {}", path.string()));
    try {
        zmq_rpc::json::json j;
        file >> j;
        Config config;
        j.get_to(config);
        return config;
    } catch (const zmq_rpc::json::json::parse_error& e) {
        throw std::runtime_error(fmt::format(
            "json parse error: {}", e.what()
        ));
    }
}

void ConfigHandler::save_to_json(const Config& config, const std::filesystem::path& path) {
    std::ofstream file(path);
    if (!file)
        throw std::runtime_error(fmt::format("can not open file with path {}", path.string()));
    zmq_rpc::json::json j;
    j = config;
    file << j;
}