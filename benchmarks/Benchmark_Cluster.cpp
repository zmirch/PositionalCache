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

static void BM_Cluster(benchmark::State& state) {
    // Get the entity count from the benchmark range
    int entityCount = state.range(0);
    CacheType cacheType = intToCacheType(state.range(1));
    int selectionSize = 30;

    World world(Point2D(WIDTH, HEIGHT));
    world.setCacheType(cacheType);
    world.clear();
    //world.addEntityAtPosition(Point2D(10, 10));
    world.addNEntitiesCluster(1000, Point2D(10, 10), 10);
    world.addNEntitiesCluster(entityCount, Point2D(WIDTH/2, HEIGHT/2));


    Bounds testBounds(Point2D(0, 0), Point2D(selectionSize, selectionSize));

    for (auto _ : state) {
        state.PauseTiming();
        selectedEntities.clear();
        state.ResumeTiming();
        world.selectArea(testBounds, [&](EntityView<WorldEntity>& safeView) {
            selectedEntities.push_back(safeView.getHandle());
        });
        benchmark::DoNotOptimize(selectedEntities);
    }
}

BENCHMARK(BM_Cluster)
    ->Args({10000, 0})       // 0: Deque
    ->Args({100000, 0})
    ->Args({1000000, 0})
    ->Args({10000, 1})       // 1: Map
    ->Args({100000, 1})
    ->Args({1000000, 1})
    ->Args({10000, 2})       // 2: Grid
    ->Args({100000, 2})
    ->Args({1000000, 2})
    ->Args({10000, 3})       // 3: Quadtree
    ->Args({100000, 3})
    ->Args({1000000, 3})
    ->Iterations(1000)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();