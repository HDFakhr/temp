#pragma once

#if YAML_ENABLE
#include <yaml-cpp/yaml.h>
#endif
#if JSON_ENABLE
#include <nlohmann/json.hpp>
#endif

namespace configs::internal
{
    class IConfigItemLoader
    {
    public:
        virtual ~IConfigItemLoader() = default;
#if YAML_ENABLE
        virtual void load(const YAML::Node& node, bool silent) = 0;
#endif
#if JSON_ENABLE
        virtual void load(const nlohmann::json& node, bool silent) = 0;
#endif
#if ENV_ENABLE
        virtual void load(const std::string env_prefix, bool silent) = 0;
#endif
    };
}
