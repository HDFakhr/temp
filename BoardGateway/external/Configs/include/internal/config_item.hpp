#pragma once

#include <stdlib.h>

#include <string>
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <exception>

#include "internal/source_type.hpp"
#include "internal/i_config_item_loader.hpp"
#include "internal/utils.hpp"
#if YAML_ENABLE
#include "internal/config_item_yaml.hpp"
#endif
#if JSON_ENABLE
#include "internal/config_item_json.hpp"
#endif
#if ENV_ENABLE
#include "internal/config_item_env.hpp"
#endif
#if ARGS_ENABLE
#include "internal/config_item_args.hpp"
#endif

namespace configs::internal
{
    template <typename T>
    class ConfigItem : public IConfigItemLoader
#if YAML_ENABLE
        , public yaml::ConfigItemYaml <T>
#endif
#if JSON_ENABLE
        , public json::ConfigItemJson <T>
#endif
#if ENV_ENABLE
        , public env::ConfigItemEnv <T>
#endif
#if ARGS_ENABLE
        , public args::ConfigItemArgs<T>
#endif
    {
    public:
        explicit ConfigItem(const char* key, const char* description = "") : IConfigItemLoader()
#if YAML_ENABLE
            , yaml::ConfigItemYaml<T>(_key, _value)
#endif
#if JSON_ENABLE
            , json::ConfigItemJson<T>(_key, _value)
#endif
#if ENV_ENABLE
            , env::ConfigItemEnv<T>(_key, _value)
#endif
#if ARGS_ENABLE
            , args::ConfigItemArgs<T>(_key, _value, _description)
#endif
            , _key(key)
            , _description(description)
        { }

        explicit ConfigItem(const char* key, const T default_value, const char* description = "") : IConfigItemLoader()
#if YAML_ENABLE
            , yaml::ConfigItemYaml<T>(_key, _value)
#endif
#if JSON_ENABLE
            , json::ConfigItemJson<T>(_key, _value)
#endif
#if ENV_ENABLE
            , env::ConfigItemEnv<T>(_key, _value)
#endif
#if ARGS_ENABLE
            , args::ConfigItemArgs<T>(_key, _value, _description)
#endif
            , _key(key)
            , _description(description)
        {
            if constexpr (is_std_vector<std::decay_t<T>>::value)
                _value.resize(default_value.size());
            _value = default_value;
        }

        ConfigItem(const T default_value) : IConfigItemLoader()
#if YAML_ENABLE
            , yaml::ConfigItemYaml<T>(_key, _value)
#endif
#if JSON_ENABLE
            , json::ConfigItemJson<T>(_key, _value)
#endif
#if ENV_ENABLE
            , env::ConfigItemEnv<T>(_key, _value)
#endif
#if ARGS_ENABLE
            , args::ConfigItemArgs<T>(_key, _value, _description)
#endif
        {
            if constexpr (is_std_vector<std::decay_t<T>>::value)
                _value.resize(default_value.size());
            _value = default_value;
        }

        ConfigItem& operator=(const T& val)
        {
            if (&this->_value != &val)
                this->_value = val;
            return *this;
        }

        ConfigItem& operator=(const ConfigItem& ci)
        {
            if (&(this->_value) != &(ci._value))
                this->_value = ci._value;
            return *this;
        }

        T& operator()() { return _value; }

        T operator()() const { return _value; }

        operator T() const { return _value; }

        operator T& () { return _value; }
        
        operator const T& () const { return _value; }

#if YAML_ENABLE
        void load(const YAML::Node& yaml_node, bool silent) override
        {
            yaml::ConfigItemYaml<T>::load(yaml_node, silent);
        }
#endif

#if JSON_ENABLE
        void load(const nlohmann::json& json_node, bool silent) override
        {
            json::ConfigItemJson<T>::load(json_node, silent);
        }
#endif

#if ENV_ENABLE
        void load(const std::string env_prefix_node, bool silent) override
        {
            env::ConfigItemEnv<T>::load(env_prefix_node, silent);
        }
#endif

        template <SourceType T>
        void load_from_(const std::string file_path, bool silent = false)
        {

        }

#if YAML_ENABLE
        template <>
        void load_from_<SourceType::Yaml>(const std::string file_path, bool silent)
        {
            try { load(YAML::LoadFile(file_path), silent); } catch (const std::exception&) { }
        }
#endif

#if JSON_ENABLE
        template <>
        void load_from_<SourceType::Json>(const std::string file_path, bool silent)
        {
            std::ifstream json_file(file_path);

            if (!json_file.is_open())
                std::cerr << "Error: could not open the JSON file \'" << file_path << "\'." << std::endl;
            else
                try { load(nlohmann::json::parse(json_file), silent); } catch (const std::exception&) { }
        }
#endif

#if ENV_ENABLE
        template <>
        void load_from_<SourceType::Env>(const std::string env_prefix, bool silent)
        {
            try { load(env_prefix, silent); }
            catch (const std::exception&) {}
        }

        ConfigItem& set_env(const std::string file_path, bool silent = false)
        {
            try {
                std::ifstream env_file(file_path);
                if (!env_file.is_open())
                    std::cerr << "Error: could not open the Env file \'" << file_path << "\'." << std::endl;
                else {
                    std::string line, key, value;
                    while (std::getline(env_file, line)) {
                        std::istringstream iss(line);
                        if (std::getline(iss, key, '=')) {
                            std::getline(iss, value);
                            #if defined _MSVC_LANG
                            putenv((key + "=" + value).c_str());
                            #else
                            setenv(key.c_str(), value.c_str(), 1);
                            #endif
                        }
                    };
                }
            } catch (const std::exception&) {}

            return *this;
        }
#endif
#if ARGS_ENABLE
        std::shared_ptr<CLI::App> load_from_args(std::vector<std::string_view> args)
        {
            std::vector<char*> argv;
            for (auto arg : args)
                argv.push_back(const_cast<char*>(arg.data()));

            return load_from_args(static_cast<int>(argv.size()), argv.data());
        }

        std::shared_ptr<CLI::App> load_from_args(int argc, char** argv)
        {
            try {
                this->_app = std::make_shared<CLI::App>("");
                this->_app->allow_windows_style_options(false);
                this->add_to_args(*this->_app);
                this->_app->parse(argc, argv);
            }
            catch (const std::bad_alloc&) {
                std::cerr << "failed to allocate memory for CLI::App" << std::endl;
                return nullptr;
            }
            catch (const CLI::ParseError& e) {
                std::exit(this->_app->exit(e));
            }

            return this->_app;
        }
#endif

    private:
        std::string _key;
        T _value;
        std::string _description;
    };
}
