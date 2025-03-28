#include <benchmark/benchmark.h>
#include "../src/Area2D.h"

std::vector<SafeEntityHandle<EngineEntity>> selectedEntities;
int WIDTH = 1280, HEIGHT = 800;

static void BM_SquareSelection_VaryingEntityCounts(benchmark::State& state) {
    // Get the entity count from the benchmark range
    int entityCount = state.range(0);
    int selectionSize = 500;

    // Set up testing environment
    Area2D area(Point2D(WIDTH, HEIGHT));
    area.clear();
    area.addNEntities(entityCount);

    Bounds testBounds(Point2D(0, 0), Point2D(500, 500));

    // Benchmark loop
    for (auto _ : state) {
        state.PauseTiming();
        area.shuffleEntityPositions();  // Shuffle entities before each iteration
        state.ResumeTiming();

        area.selectArea(testBounds, [&](SafeEntityView<EngineEntity>& safeView) {
            selectedEntities.push_back(safeView.getHandle());
        });
        benchmark::DoNotOptimize(selectedEntities);
    }
}

BENCHMARK(BM_SquareSelection_VaryingEntityCounts)
    ->Args({ 100 })    // Entity count for benchmark
    ->Args({ 1000 })
    ->Args({ 10000 })
    ->Args({ 100000 })
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();