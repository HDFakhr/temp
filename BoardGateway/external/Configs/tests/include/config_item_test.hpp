#pragma once

#include <configs.hpp>
#include "testing_params.hpp"

using namespace ::testing;


TEST(ConfigsTest, __object_instantiation) {
    EXPECT_NO_THROW(configs::ConfigItem<TestingParams> config{ "testing params" });
}
