add_executable(PositionalCache
        main.cpp
        Area2D.cpp
        Bounds.cpp
        EngineEntity.cpp
        Cache.cpp
        CacheEntity.cpp
        Cache.cpp
        Error.h
        Point2D.cpp
        Button.cpp
        Button.h
        EntityHandle.cpp
        EntityHandle.h
        EntityView.cpp
        EntityView.h
)

# Link Raylib to the executable
target_link_libraries(PositionalCache PRIVATE raylib benchmark::benchmark)
