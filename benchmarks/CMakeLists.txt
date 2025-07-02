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

# Benchmark for selection of an isolated entity in the presence of a large cluster
add_executable(PositionalCache_Benchmarks_Cluster Benchmark_Cluster.cpp)
target_link_libraries(PositionalCache_Benchmarks_Cluster PRIVATE benchmark::benchmark)
target_include_directories(PositionalCache_Benchmarks_Cluster PRIVATE ../src)
target_sources(PositionalCache_Benchmarks_Cluster PRIVATE
        ../src/FrameworkUser/World.cpp
        ../src/Framework/Point2D.cpp
        ../src/Framework/Bounds.cpp
        ../src/FrameworkUser/WorldEntity.cpp
)

# Benchmark for selection of the center part of a dense cluster
add_executable(PositionalCache_Benchmarks_QTree Benchmark_QTree.cpp)
target_link_libraries(PositionalCache_Benchmarks_QTree PRIVATE benchmark::benchmark)
target_include_directories(PositionalCache_Benchmarks_QTree PRIVATE ../src)
target_sources(PositionalCache_Benchmarks_QTree PRIVATE
        ../src/FrameworkUser/World.cpp
        ../src/Framework/Point2D.cpp
        ../src/Framework/Bounds.cpp
        ../src/FrameworkUser/WorldEntity.cpp
)
