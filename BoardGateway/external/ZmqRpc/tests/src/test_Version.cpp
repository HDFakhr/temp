#include <gtest/gtest.h>
#include "zmq_rpc/version.hpp"


TEST(TestVersion, CheckVersion) {
    const std::string_view project_version{ PROJECT_VERSION };
    const std::string_view version{ zmq_rpc::version() };
    ASSERT_EQ(version, project_version);
}