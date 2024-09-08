find_package(benchmark REQUIRED)
add_executable(allocator_benchmark "main.cpp")
target_link_libraries(allocator_benchmark PUBLIC benchmark::benchmark allocator_lib)
#--benchmark_min_warmup_time