add_executable(PositionalCache
        FrameworkUser/Main.cpp
        FrameworkUser/World.cpp
        Framework/Bounds.cpp
        FrameworkUser/WorldEntity.cpp
        Framework/Error.h
        Framework/Point2D.cpp
        FrameworkUser/Button.cpp
        FrameworkUser/Button.h
        Framework/EntityHandle.h
        Framework/EntityView.h
        Framework/Algorithms/BasicAlgorithm.h
        Framework/Algorithms/DequeAlgorithm.h
        Framework/Algorithms/QuadtreeAlgorithm.h
        Framework/Algorithms/GridAlgorithm.h
)

# Link Raylib to the executable
target_link_libraries(PositionalCache PRIVATE raylib benchmark::benchmark)
