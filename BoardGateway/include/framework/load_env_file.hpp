#pragma once

#include <fstream>
#include <filesystem>
#include <string>
#include <cstdlib>
#include <sstream>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    void load_env_file(const std::filesystem::path& path) {
        using str = std::string;

        std::ifstream file(path);
        str line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            str key;
            str value;
            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
#ifdef _WIN32
                _putenv_s(key.c_str(), value.c_str());
#else
                setenv(key.c_str(), value.c_str(), 1);
#endif
            }
        }
    }
}