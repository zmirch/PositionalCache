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
        default: throw std::invalid_argument("Invalid cache type value");
    }
}

static void BM_SquareSelection_VaryingEntityCounts(benchmark::State& state) {
    // Get the entity count from the benchmark range
    int entityCount = state.range(0);
    CacheType cacheType = intToCacheType(state.range(1));
    int selectionSize = 500;

    // Set up testing environment
    World world(Point2D(WIDTH, HEIGHT));
    world.setCacheType(cacheType);
    world.clear();
    world.addNEntities(entityCount);

    Bounds testBounds(Point2D(0, 0), Point2D(selectionSize, selectionSize));

    // Benchmark loop
    for (auto _ : state) {
        state.PauseTiming();
        world.shuffleEntityPositions();  // Shuffle entities before each iteration
        state.ResumeTiming();

        world.selectArea(testBounds, [&](EntityView<WorldEntity>& safeView) {
            selectedEntities.push_back(safeView.getHandle());
        });
        benchmark::DoNotOptimize(selectedEntities);
    }
}

BENCHMARK(BM_SquareSelection_VaryingEntityCounts)
    ->Args({100, 0})     // 0: Vector
    ->Args({1000, 0})
    ->Args({10000, 0})
    ->Args({100000, 0})
    ->Args({100, 1})     // 1: Basic (Map)
    ->Args({1000, 1})
    ->Args({10000, 1})
    ->Args({100000, 1})
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();