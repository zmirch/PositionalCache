cmake_minimum_required(VERSION 3.30)
project(PositionalCache)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wno-old-style-cast)
endif()

# Dependencies
include(FetchContent)

## Fetch Raylib
set(RAYLIB_VERSION 4.5.0)
FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
        FIND_PACKAGE_ARGS ${RAYLIB_VERSION} EXACT
)
set(RAYLIB_BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(raylib)

## Fetch GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

## Fetch Google Benchmark
FetchContent_Declare(
        benchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.8.2.zip
)
FetchContent_MakeAvailable(benchmark)

# Add the performance tests
add_subdirectory(benchmarks)

# Include the main project (src directory)
add_subdirectory(src)