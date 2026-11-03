#pragma once

#include <vector>
#include <string>
#include <filesystem>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <glog/logging.h>
#include <configs.hpp>

#include "testing_params.hpp"
#include "silent_output.hpp"

using namespace ::testing;

extern int argc_;
extern char** argv_;


#if ARGS_ENABLE
class ConfigItemArgsTest : public ::testing::Test
{
protected:
    configs::ConfigItem<TestingParams> config{ "test" };

    void SetUp() override {
        SilentOutput s;
        config.load_from_args(argc_, argv_);
    }
};


TEST(ConfigsTest, __load_from_args) {
    configs::ConfigItem<TestingParams> config{ "test" };
    std::vector<std::string> args;
    args.assign(argv_, argv_ + argc_);
    auto app = config.load_from_args(argc_, argv_);
    //std::cout << app->help() << std::endl;
}


TEST_F(ConfigItemArgsTest, __check_string) {
    EXPECT_EQ(config().string_value(), "hello world!");
}

TEST_F(ConfigItemArgsTest, __check_int) {
    EXPECT_EQ(config().int_value(), 123);
}

TEST_F(ConfigItemArgsTest, __check_float) {
    EXPECT_EQ(config().float_value(), 10.5f);
}

TEST_F(ConfigItemArgsTest, __check_bool) {
    EXPECT_EQ(config().boolean_value(), true);
}

#if 1
TEST_F(ConfigItemArgsTest, __check_path) {
    EXPECT_EQ(config().path_value(), std::filesystem::path("/usr/bin"));
    EXPECT_EQ(config().path_value().parent_path(), std::filesystem::path("/usr"));
}
#endif

TEST_F(ConfigItemArgsTest, __check_object) {
    auto object_person = config().object_person();
    EXPECT_EQ(object_person.name(), "ali");
    EXPECT_EQ(object_person.age(), 51);
}

TEST_F(ConfigItemArgsTest, __check_vector_string) {
    EXPECT_THAT(config().vector_string(), ElementsAre("str1", "str2", "str3"));
}

TEST_F(ConfigItemArgsTest, __check_vector_int) {
    EXPECT_THAT(config().vector_int(), ElementsAre(11, 12));
}

#if 1
TEST_F(ConfigItemArgsTest, __check_vector_path) {
    EXPECT_THAT(config().vector_path(), ElementsAre(
        std::filesystem::path("/usr/path1"), std::filesystem::path("/usr/path2")));
}
#endif

TEST_F(ConfigItemArgsTest, __check_vector_object) {
    auto vector_person = config().vector_person();
    EXPECT_EQ(vector_person.size(), 2);
    EXPECT_EQ(vector_person[0].name(), "reza");
    EXPECT_EQ(vector_person[0].age(), 52);
    EXPECT_EQ(vector_person[1].name(), "omid");
    EXPECT_EQ(vector_person[1].age(), 53);
}

TEST_F(ConfigItemArgsTest, __check_string_with_default) {
    EXPECT_EQ(config().string_with_default(), "default-str");
}

TEST_F(ConfigItemArgsTest, __check_int_with_default) {
    EXPECT_EQ(config().int_with_default(), -99);
}

TEST_F(ConfigItemArgsTest, __check_boolean_with_default) {
    EXPECT_EQ(config().boolean_with_default(), true);
}

TEST_F(ConfigItemArgsTest, __check_path_with_default) {
    EXPECT_EQ(config().path_with_default(), std::filesystem::path("/home"));
    EXPECT_EQ(config().path_with_default().parent_path(), std::filesystem::path("/"));
}

TEST_F(ConfigItemArgsTest, __check_object_person_with_default) {
    auto person_with_default = config().person_with_default();
    EXPECT_EQ(person_with_default.name(), "newborn");
    EXPECT_EQ(person_with_default.age(), 1);
}

TEST_F(ConfigItemArgsTest, __check_vector_string_with_default) {
    EXPECT_THAT(config().vector_string_with_default(), ElementsAre("str_def_0", "str_def_1"));
}

TEST_F(ConfigItemArgsTest, __check_vector_int_with_default) {
    EXPECT_THAT(config().vector_int_with_default(), ElementsAre(101, 102));
}

TEST_F(ConfigItemArgsTest, __check_vector_path_with_default) {
    EXPECT_THAT(config().vector_path_with_default(), ElementsAre(
        std::filesystem::path("/etc/ssl"), std::filesystem::path("/etc/vim")));
    EXPECT_EQ(config().vector_path_with_default()[0].parent_path(), std::filesystem::path("/etc"));
    EXPECT_EQ(config().vector_path_with_default()[1].parent_path(), std::filesystem::path("/etc"));
}

TEST_F(ConfigItemArgsTest, __check_vector_object_person_with_default) {
    auto vector_person_with_default = config().vector_person_with_default();
    EXPECT_EQ(vector_person_with_default.size(), 2);
    EXPECT_EQ(vector_person_with_default[0].name(), "newborn-boy");
    EXPECT_EQ(vector_person_with_default[0].age(), 2);
    EXPECT_EQ(vector_person_with_default[1].name(), "newborn-girl");
    EXPECT_EQ(vector_person_with_default[1].age(), 3);
}
#endif
