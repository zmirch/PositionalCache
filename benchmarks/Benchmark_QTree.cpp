#include <stdexcept>
#include <benchmark/benchmark.h>

#include "../cmake-build-debug/_deps/benchmark-src/src/arraysize.h"
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

static void BM_QTree(benchmark::State& state) {
    // Get the entity count from the benchmark range
    int entityCount = state.range(0);
    CacheType cacheType = intToCacheType(state.range(1));
    int selectionSize = 10;

    World world(Point2D(WIDTH, HEIGHT));
    world.setCacheType(cacheType);
    world.clear();
    //world.addNEntitiesCluster(entityCount, Point2D(WIDTH/2, HEIGHT/2), 10);
    //world.addNEntitiesCluster(entityCount, Point2D(100, 150), 30);
    world.addNEntitiesCluster(entityCount, Point2D(WIDTH/2, HEIGHT/2), 200);

    //Bounds testBounds(Point2D(WIDTH/2 - selectionSize/2, HEIGHT/2 - selectionSize/2), Point2D(WIDTH/2 + selectionSize/2, HEIGHT/2 + selectionSize/2));
    //Bounds testBounds(Point2D(WIDTH/2 - selectionSize/2, HEIGHT/2 - selectionSize/2), Point2D(WIDTH/2, HEIGHT/2));
    Bounds testBounds(Point2D(300, 300), Point2D(450, 450));
    //Bounds testBounds(Point2D(80, 130), Point2D(90, 150));
    //Bounds testBounds(Point2D(750, 330), Point2D(820, 400));
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

BENCHMARK(BM_QTree)
    ->Args({5000000, 0})     // 0: Deque
    ->Args({5000000, 1})       // 1: Map
    ->Args({5000000, 2})     // 2: Grid
    ->Args({5000000, 3})       // 3: Quadtree
    ->Iterations(1000)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();