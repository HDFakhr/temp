
find_package(Microsoft.GSL REQUIRED)
find_package(cppzmq REQUIRED)
find_package(nlohmann_json REQUIRED)

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

set(Microsoft.GSL_COMPONENTS Microsoft.GSL::GSL)
set(ZMQ_COMPONENTS cppzmq)
set(NLOHMANN_JSON_COMPONENTS nlohmann_json::nlohmann_json)