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
        case 2: return CacheType::Grid;
        case 3: return CacheType::Quadtree;
        default: throw std::invalid_argument("Invalid cache type value");
    }
}

static void BM_SquareSelection_VaryingEntityCounts(benchmark::State& state) {
    // Get the entity count from the benchmark range
    int entityCount = state.range(0);
    CacheType cacheType = intToCacheType(state.range(1));
    int selectionSize = 500;

    World world(Point2D(WIDTH, HEIGHT));
    world.setCacheType(cacheType);
    world.clear();
    world.addNEntities(entityCount);

    Bounds testBounds(Point2D(0, 0), Point2D(selectionSize, selectionSize));

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

BENCHMARK(BM_SquareSelection_VaryingEntityCounts)
    ->Args({1000, 0})       // 0: Vector
    ->Args({10000, 0})
    ->Args({100000, 0})
    ->Args({1000, 1})       // 1: Basic (Map)
    ->Args({10000, 1})
    ->Args({100000, 1})
    ->Args({1000, 2})       // 2: Grid
    ->Args({10000, 2})
    ->Args({100000, 2})
    ->Args({1000, 3})       // 3: Quadtree
    ->Args({10000, 3})
    ->Args({100000, 3})
    ->Iterations(100)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();