#include "usecase/extract_args.hpp"
#include "framework/service_runner.hpp"
#include "framework/load_env_file.hpp"
#include <iostream>
#include <string>
#include <string_view>
#include <thread>
#include <filesystem>
#include <vector>

using SERVICE_NAMESPACE_PREFIX::framework::ServiceRunner;

using path_t = std::filesystem::path;

int better_main(const std::vector<std::string_view>& args) {
    auto service_args{ SERVICE_NAMESPACE_PREFIX::usecase::extract_args(args) };

    int result{ 0 };
    do {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        result = ServiceRunner::run(args, service_args);
    } while (result < 0);
    return result;
}

int main(int argc, char const* argv[])
{
    std::vector<std::string_view> args;
    args.assign(argv, argv + argc);
    return better_main(args);
}
