#include <gtest/gtest.h>
#include "board_zmq_client/version.hpp"

static constexpr std::string_view project_version{ PROJECT_VERSION };

TEST(TestVersion, CheckVersion) {
    ASSERT_EQ(nooyan::zmq_client::version(), project_version);
}