#pragma once

#include "entity/service_config.hpp"
#include <string>
#include <libenvpp/env.hpp>

namespace SERVICE_NAMESPACE_PREFIX::framework
{
    class LoadEnvVariables {
    public:
        using str = std::string;
        using config_t = entity::ServiceConfig;

        static config_t load(const str& env_prefix) {
            config_t _;
            env::prefix ee{ env_prefix };
            const auto service_name{ ee.register_variable<str>("SERVICE_NAME") };
            const auto parsed{ ee.parse_and_validate() };
            
            config_t::Setter(_)
                .service_name(parsed.get_or(service_name, _.service_name));
            if (_.service_name.empty())
                throw std::runtime_error(fmt::format("{}_SERVICE_NAME is not set!", env_prefix));
            return _;
        }
    };
}