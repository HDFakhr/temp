
#include <benchmark/benchmark.h>
#include <glog/logging.h>

int main(int argc, char** argv) 
{
    FLAGS_logtostderr = 1;
    google::InitGoogleLogging(argv[0]);
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
    return 0;
}