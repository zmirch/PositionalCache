#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>

namespace PositionalCache
{

template <typename E>
class GridAlgorithm
{
public:
    GridAlgorithm(int width, int height, int rowNum, int colNum) : width(width), height(height), rowNum(rowNum), colNum(colNum)
    {
        cells.resize(rowNum * colNum);

        float cellWidth = static_cast<float>(width) / colNum;
        float cellHeight = static_cast<float>(height) / rowNum;

        for (int row = 0; row < rowNum; ++row) {
            for (int col = 0; col < colNum; ++col) {
                float x = col * cellWidth;
                float y = row * cellHeight;
                Bounds b{Point2D(x, y), Point2D(x + cellWidth, y + cellHeight)};
                cells[toIndex(row, col)] = Cell(b);
            }
        }
    };

    void addEntity(std::unique_ptr<E>&& entity, const Point2D& position, int id)
    {
        Error::ASSERT(entitiesMap.find(id) == entitiesMap.end(), "Entity has already been added.");
        Entity<E> newHandle (std::move(entity), id, position, [this](Entity<E>& en, const Point2D& pos){updateEntityPosition(en, pos);});
        std::shared_ptr<Entity<E>> newEntity = std::make_shared<Entity<E>>(std::move(newHandle));
        entitiesMap.emplace(id, newEntity);

        auto [row, col] = toGridIndices(position);
        cells[toIndex(row, col)].entities.push_back(newEntity);
    };
    void removeEntityById(int id)
    {
        auto it = entitiesMap.find(id);
        if (it == entitiesMap.end()) return;

        std::shared_ptr<Entity<E>> entity = it->second;

        auto [row, col] = toGridIndices(entity->getPosition());
        auto& cellEntities = cells[toIndex(row, col)].entities;

        cellEntities.erase(
            std::remove(cellEntities.begin(), cellEntities.end(), entity),
            cellEntities.end()
        );

        entitiesMap.erase(it);
    }

    int getEntityCount()
    {
        return entitiesMap.size();
    };
    void selectArea(const PositionalCache::Bounds& selection,
                std::function<void(EntityView<E>& handle)> consumer)
    {
        std::pair<int, int> gridIndexA = toGridIndices(selection.getPointA());
        std::pair<int, int> gridIndexB = toGridIndices(selection.getPointB());

        int indexA = toIndex(gridIndexA.first, gridIndexA.second);
        int indexB = toIndex(gridIndexB.first, gridIndexB.second);
        for (int i = indexA; i < indexB; ++i)
        {
            auto& cell = cells[i];
            if (!cell.bounds.intersects(selection)) continue;
            for (auto& entity : cell.entities) {
                if (selection.containsPosition(entity->getPosition())) {
                    EntityView<E> safeView(entity);
                    consumer(safeView);
                }
            }
        }
    };
    void getAllEntities(std::function<void(EntityView<E>& view)> consumer)
    {
        for (auto& [id, entity] : entitiesMap)
        {
            EntityView<E> safeView(entity);
            consumer(safeView);
        }
    };
    Entity<E>& getEntityById(int id)
    {
        return *entitiesMap.at(id).get();
    };
    bool contains(int id)
    {
        return entitiesMap.find(id) != entitiesMap.end();
    };
    void clear()
    {
        entitiesMap.clear();
        for (auto& cell : cells)
        {
            cell.entities.clear();
        }
    };
    void updateEntityPosition(Entity<E>& entity, const Point2D& oldPosition)
    {
        auto oldIndices = toGridIndices(oldPosition);
        auto newIndices = toGridIndices(entity.getPosition());

        if (oldIndices == newIndices) return; // Still in the same cell

        auto& oldCell = cells[toIndex(oldIndices.first, oldIndices.second)].entities;
        auto& newCell = cells[toIndex(newIndices.first, newIndices.second)].entities;

        std::shared_ptr<Entity<E>> entityPtr = entitiesMap.at(entity.getId());

        // Remove from old cell
        oldCell.erase(
            std::remove(oldCell.begin(), oldCell.end(), entityPtr),
            oldCell.end()
        );

        // Add to new cell
        newCell.push_back(entityPtr);
    }
    void getNodeBounds(std::function<void(const Bounds&)> consumer) const
    {
        for (auto& cell : cells)
        {
            consumer(cell.bounds);
        }
    }

private:
    int width, height, rowNum, colNum;
    std::unordered_map<int, std::shared_ptr<Entity<E>>> entitiesMap;

    struct Cell {
        Bounds bounds;
        std::deque<std::shared_ptr<Entity<E>>> entities;
        Cell() : bounds(Point2D(0, 0), Point2D(0, 0)) {}
        Cell(Bounds bounds): bounds(bounds) {}
    };
    std::vector<Cell> cells;

    inline int toIndex(int row, int col) const {
        return row * colNum + col;
    }

    inline std::pair<int, int> toGridIndices(const Point2D& position) const {
        int col = static_cast<int>(position.getX() / (width / colNum));
        int row = static_cast<int>(position.getY() / (height / rowNum));
        col = std::min(std::max(col, 0), colNum - 1);
        row = std::min(std::max(row, 0), rowNum - 1);
        return {row, col};
    }
};
}
