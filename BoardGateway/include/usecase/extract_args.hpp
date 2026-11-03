#pragma once

#include "entity/service_args.hpp"
#include <stdexcept>
#include <vector>
#include <string>

namespace SERVICE_NAMESPACE_PREFIX::usecase
{
    static entity::ServiceArgs extract_args(std::vector<std::string_view> args) {
        using entity::ServiceArgs;

        ServiceArgs _;
        ServiceArgs::Setter(_).service_version(SERVICE_VERSION);

        for (auto iter = args.begin(); iter != args.end(); ++iter) {
            if (*iter == "--env-file")
                ServiceArgs::Setter(_).env_file(*std::next(iter));
            if (*iter == "--env-prefix")
                ServiceArgs::Setter(_).env_prefix(*std::next(iter));
        }
        return _;
    }
}