#include <benchmark/benchmark.h>
#include "../src/Area2D.h"

using namespace Engine;

std::vector<EntityHandle<EngineEntity>> selectedEntities;
int WIDTH = 1280, HEIGHT = 800;

static void BM_SquareSelection_VaryingSelectionSizes(benchmark::State& state) {
    // Get the selection size from the benchmark range
    int selectionSize = state.range(0);
    int entityCount = 100000;  // Fixed entity count

    // Set up testing environment
    Area2D area(Point2D(WIDTH, HEIGHT));
    area.clear();
    area.addNEntities(entityCount);

    Bounds testBounds(Point2D(0, 0), Point2D(selectionSize, selectionSize));

    // Benchmark loop
    for (auto _ : state) {
        state.PauseTiming();
        area.shuffleEntityPositions();  // Shuffle entities before each iteration
        state.ResumeTiming();

        area.selectArea(testBounds, [&](EntityView<EngineEntity>& safeView) {
            selectedEntities.push_back(safeView.getHandle());
        });
        benchmark::DoNotOptimize(selectedEntities);
    }
}

BENCHMARK(BM_SquareSelection_VaryingSelectionSizes)
->Args({ 10 })    // Selection size for benchmark
->Args({ 100 })
->Args({ 500 })
->Args({ std::min(WIDTH, HEIGHT) - 1 })
->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();