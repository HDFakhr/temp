#pragma once

#include <vector>
#include <string>
#include <filesystem>

#include <configs.hpp>

template <typename T> using config_t = configs::ConfigItem<T>;

struct Person
{
    config_t <std::string> name{ "name" };
    config_t <int> age{ "age" };

    DECLARE_MEMBERS(&Person::name, &Person::age)
};

struct TestingParams
{
    config_t <std::string>                          string_value{ "string value" };
    config_t <int>                                  int_value{ "int value" };
    config_t <float>                                float_value{ "float value" };
    config_t <bool>                                 boolean_value{ "boolean value" };
    config_t <std::filesystem::path>                path_value{ "path value" };
    config_t <Person>                               object_person{ "object person" };
    config_t <std::vector<std::string>>             vector_string{ "vector string" };
    config_t <std::vector<int>>                     vector_int{ "vector int" };
    config_t <std::vector<std::filesystem::path>>   vector_path{ "vector path" };
    config_t <std::vector<Person>>                  vector_person{ "vector person" };

    // these items are not in the files to test the default values
    config_t <std::string> string_with_default{ "string with default", std::string("default-str") };
    config_t <int> int_with_default{ "int with default", -99 };
    config_t <bool> boolean_with_default{ "boolean with default", true };
    config_t <std::filesystem::path> path_with_default{ "path with default", std::filesystem::path("/home") };
    config_t <Person> person_with_default{ "person with default", {std::string("newborn"), 1 } };
    config_t <std::vector<std::string>> vector_string_with_default{ "vector string with default",
        std::vector<std::string>{"str_def_0", "str_def_1"}
    };
    config_t <std::vector<int>> vector_int_with_default{ "vector int with default", {101,102} };
    config_t <std::vector<std::filesystem::path>> vector_path_with_default{ "vector path with default",
        std::vector<std::filesystem::path>{"/etc/ssl", "/etc/vim"}
    };
    config_t <std::vector<Person>> vector_person_with_default{ "vector person with default",
        std::vector<Person>{ {std::string("newborn-boy"), 2 }, {std::string("newborn-girl"), 3}}
    };


    DECLARE_MEMBERS(
    &TestingParams::string_value,
        &TestingParams::int_value,
        &TestingParams::float_value,
        &TestingParams::boolean_value,
        &TestingParams::path_value,
        &TestingParams::object_person,
        &TestingParams::vector_string,
        &TestingParams::vector_int,
        &TestingParams::vector_path,
        &TestingParams::vector_person,
        &TestingParams::string_with_default,
        &TestingParams::int_with_default,
        &TestingParams::boolean_with_default,
        &TestingParams::path_with_default,
        &TestingParams::person_with_default,
        &TestingParams::vector_string_with_default,
        &TestingParams::vector_int_with_default,
        &TestingParams::vector_path_with_default,
        &TestingParams::vector_person_with_default
        )
};
