# Benchmark for varying entity counts
add_executable(PositionalCache_Benchmarks_VaryingEntityCounts Benchmark_VaryingEntityCounts.cpp)
target_link_libraries(PositionalCache_Benchmarks_VaryingEntityCounts PRIVATE benchmark::benchmark)
target_include_directories(PositionalCache_Benchmarks_VaryingEntityCounts PRIVATE ../src)
target_sources(PositionalCache_Benchmarks_VaryingEntityCounts PRIVATE
        ../src/FrameworkUser/World.cpp
        ../src/Framework/Point2D.cpp
        ../src/Framework/Bounds.cpp
        ../src/FrameworkUser/WorldEntity.cpp
)

# Benchmark for varying selection sizes
add_executable(PositionalCache_Benchmarks_VaryingSelectionSizes Benchmark_VaryingSelectionSizes.cpp)
target_link_libraries(PositionalCache_Benchmarks_VaryingSelectionSizes PRIVATE benchmark::benchmark)
target_include_directories(PositionalCache_Benchmarks_VaryingSelectionSizes PRIVATE ../src)
target_sources(PositionalCache_Benchmarks_VaryingSelectionSizes PRIVATE
        ../src/FrameworkUser/World.cpp
        ../src/Framework/Point2D.cpp
        ../src/Framework/Bounds.cpp
        ../src/FrameworkUser/WorldEntity.cpp
)
