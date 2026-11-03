#pragma once

#include <string>

namespace SERVICE_NAMESPACE_PREFIX::entity
{
    struct ServiceArgs {
        using str = std::string;

        struct Default {
            using str_view = std::string_view;

#ifdef NDEBUG
            static constexpr str_view env_file{ "" };
#else
            static constexpr str_view env_file{ DEBUG_ENV_FILE };
#endif
            static constexpr str_view service_version{ "" };
            static constexpr str_view env_prefix{ ENV_VARIABLES_PREFIX };
        };

        str env_file{ Default::env_file };
        str env_prefix{ Default::env_prefix };
        str service_version{ "" };

        struct Setter {
            ServiceArgs& _args;

            explicit Setter(ServiceArgs& args) noexcept : _args{ args } {}

            template<typename T>
            Setter& env_file(T&& name) {
                _args.env_file.assign(std::forward<T>(name));
                return *this;
            }

            template<typename T>
            Setter& env_prefix(T&& prefix) {
                _args.env_prefix.assign(std::forward<T>(prefix));
                return *this;
            }

            template<typename T>
            Setter& service_version(T&& version) {
                _args.service_version.assign(std::forward<T>(version));
                return *this;
            }
        };
    }; 
}
