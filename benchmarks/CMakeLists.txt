# Benchmark for varying entity counts
add_executable(PositionalCache_Benchmarks_VaryingEntityCounts Benchmark_VaryingEntityCounts.cpp)
target_link_libraries(PositionalCache_Benchmarks_VaryingEntityCounts PRIVATE benchmark::benchmark)
target_include_directories(PositionalCache_Benchmarks_VaryingEntityCounts PRIVATE ../src)
target_sources(PositionalCache_Benchmarks_VaryingEntityCounts PRIVATE
        ../src/World.cpp
        ../src/Point2D.cpp
        ../src/Bounds.cpp
        ../src/WorldEntity.cpp
)

# Benchmark for varying selection sizes
add_executable(PositionalCache_Benchmarks_VaryingSelectionSizes Benchmark_VaryingSelectionSizes.cpp)
target_link_libraries(PositionalCache_Benchmarks_VaryingSelectionSizes PRIVATE benchmark::benchmark)
target_include_directories(PositionalCache_Benchmarks_VaryingSelectionSizes PRIVATE ../src)
target_sources(PositionalCache_Benchmarks_VaryingSelectionSizes PRIVATE
        ../src/World.cpp
        ../src/Point2D.cpp
        ../src/Bounds.cpp
        ../src/WorldEntity.cpp
)
