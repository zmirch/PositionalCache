#include <iostream>
#include <stdexcept>
#include <benchmark/benchmark.h>
#include "../src/FrameworkUser/World.h"

using namespace FrameworkUser;

std::vector<EntityHandle<WorldEntity>> selectedEntities;
int WIDTH = 1280, HEIGHT = 800;

CacheType intToCacheType(int value) {
    switch (value) {
        case 0: return CacheType::Deque;
        case 1: return CacheType::Basic;
        case 2: return CacheType::Quadtree;
        default: throw std::invalid_argument("Invalid cache type value");
    }
}

static void BM_SquareSelection_VaryingSelectionSizes(benchmark::State& state) {
    // Get the selection size from the benchmark range
    int selectionSize = state.range(0);
    CacheType cacheType = intToCacheType(state.range(1));
    int entityCount = 100000;  // Fixed entity count

    World world(Point2D(WIDTH, HEIGHT));
    world.setCacheType(cacheType);
    world.clear();
    world.addNEntities(entityCount);

    Bounds testBounds(Point2D(0, 0), Point2D(selectionSize, selectionSize));
    // Benchmark loop
    for (auto _ : state) {
        state.PauseTiming();
        selectedEntities.clear();
        world.shuffleEntityPositions();  // Shuffle entities before each iteration
        state.ResumeTiming();

        world.selectArea(testBounds, [&](EntityView<WorldEntity>& safeView) {
            selectedEntities.push_back(safeView.getHandle());
        });
        benchmark::DoNotOptimize(selectedEntities);
    }
}

BENCHMARK(BM_SquareSelection_VaryingSelectionSizes)
    ->Args({ 10, 0 })    // 0: Vector
    ->Args({ 100, 0 })
    ->Args({ 500, 0 })
    ->Args({ std::min(WIDTH, HEIGHT) - 1, 0 })
    ->Args({ 10, 1 })    // 1: Basic (Map)
    ->Args({ 100, 1 })
    ->Args({ 500, 1 })
    ->Args({ std::min(WIDTH, HEIGHT) - 1, 1 })
    ->Args({ 10, 2 })    // 2: StaticQTree (Grid)
    ->Args({ 100, 2 })
    ->Args({ 500, 2 })
    ->Args({ std::min(WIDTH, HEIGHT) - 1, 2 })
->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();