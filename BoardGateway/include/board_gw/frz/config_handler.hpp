#pragma once

#include "board_gw/frz/config.hpp"
#include <filesystem>

namespace pooyan::board::frz
{
    class ConfigHandler {
    public:
        static Config load_from_json(const std::filesystem::path& path);
        static void save_to_json(const Config& config, const std::filesystem::path& path);
    private:
    };
    
}