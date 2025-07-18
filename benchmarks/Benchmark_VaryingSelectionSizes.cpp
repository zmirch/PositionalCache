#include <iostream>
#include <stdexcept>
#include <benchmark/benchmark.h>
#include "../src/FrameworkUser/World.h"

using namespace FrameworkUser;

std::vector<EntityHandle<WorldEntity>> selectedEntities;
int WIDTH = 1000, HEIGHT = 1000;

CacheType intToCacheType(int value) {
    switch (value) {
        case 0: return CacheType::Deque;
        case 1: return CacheType::Basic;
        case 2: return CacheType::Grid;
        case 3: return CacheType::Quadtree;
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
    ->Args({ 100, 0 })  // 0: Deque
    ->Args({ 500, 0 })
    ->Args({ std::min(WIDTH, HEIGHT) - 1, 0 })
    ->Args({ 100, 1 })  // 1: Map
    ->Args({ 500, 1 })
    ->Args({ std::min(WIDTH, HEIGHT) - 1, 1 })
    ->Args({ 100, 2 })  // 2: Grid
    ->Args({ 500, 2 })
    ->Args({ std::min(WIDTH, HEIGHT) - 1, 2 })
    ->Args({ 100, 3 })  // 3: Quadtree
    ->Args({ 500, 3 })
    ->Args({ std::min(WIDTH, HEIGHT) - 1, 3 })
    ->Iterations(1000)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();