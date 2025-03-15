add_executable(PositionalCache
        main.cpp
        Area2D.cpp
        Bounds.cpp
        EngineEntity.cpp
        EntityCache.cpp
        CacheEntity.cpp
        EntityCache.cpp
        Error.h
        Point2D.cpp
        Timer.cpp
        Button.cpp
        Button.h
)

# Link Raylib to the executable
target_link_libraries(PositionalCache PRIVATE raylib benchmark::benchmark)
