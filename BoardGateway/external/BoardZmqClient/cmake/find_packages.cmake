
find_package(fmt REQUIRED CONFIG)
find_package(cppzmq REQUIRED CONFIG)
find_package(nlohmann_json REQUIRED CONFIG)

if (BUILD_WITH_TESTS)
    find_package(GTest REQUIRED)
    set(GTest_COMPONENTS gtest::gtest)
endif()

if (BUILD_WITH_BENCHMARKS)
    find_package(benchmark REQUIRED)
    set(BENCHMARK_COMPONENTS benchmark::benchmark)
endif()

if (BUILD_WITH_TESTS OR BUILD_WITH_BENCHMARKS)
    find_package(glog REQUIRED)
    set(GLOG_COMPONENTS glog::glog)
endif()

set(FMT_COMPONENTS fmt::fmt)
set(ZMQ_COMPONENTS cppzmq)
set(NLOHMANN_JSON_COMPONENTS nlohmann_json::nlohmann_json)