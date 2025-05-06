#pragma once
#include <iostream>
#include <optional>

namespace PositionalCache
{

template <typename E>
class StaticQuadtreeAlgorithm
{
public:

    StaticQuadtreeAlgorithm(int width, int height, int maxDepth = 4) : maxDepth(maxDepth), width(width), height(height)
    {
        root = std::make_unique<Node>(Bounds(Point2D(0, 0), Point2D(width, height)), 0, maxDepth);
    };
    void addEntity(std::unique_ptr<E>&& entity, const Point2D& position, int id)
    {
        Error::ASSERT(entitiesMap.find(id) == entitiesMap.end(), "Entity has already been added.");
        Entity<E> newHandle(std::move(entity), id, position, [this](Entity<E>& en, const Point2D& pos){updateEntityPosition(en, pos);});
        std::shared_ptr<Entity<E>> newEntity = std::make_shared<Entity<E>>(std::move(newHandle));
        entitiesMap.emplace(id, newEntity);
        root->insert(newEntity);
    }
    void removeEntityById(int id)
    {
        auto it = entitiesMap.find(id);
        if (it != entitiesMap.end())
        {
            root->remove(it->second);
            entitiesMap.erase(it);
        }
    }
    int getEntityCount()
    {
        return entitiesMap.size();
    }
    void selectArea(const PositionalCache::Bounds& boundingBox,
                std::function<void(EntityView<E>& handle)> consumer)
    {
        root->select(boundingBox, consumer);
    }
    void getAllEntities(std::function<void(EntityView<E>& view)> consumer)
    {
        for (auto& [id, entity] : entitiesMap)
        {
            EntityView<E> safeView(entity);
            consumer(safeView);
        }
    }
    Entity<E>& getEntityById(int id)
    {
        return *entitiesMap.at(id).get();
    }
    bool contains(int id)
    {
        return entitiesMap.find(id) != entitiesMap.end();
    }
    void clear()
    {
        entitiesMap.clear();
        root = std::make_unique<Node>(Bounds(Point2D(0, 0), Point2D(width, height)), 0, maxDepth);
    }

    void updateEntityPosition(Entity<E>& entity, const Point2D& oldPosition)
    {
        // Find quadtree node that has this position.
        Node* node = root->findLowestNode(oldPosition);
        if (!node) return;

        auto it = std::find_if(node->entities.begin(), node->entities.end(),
            [&entity](const std::shared_ptr<Entity<E>>& e) {
                return e.get() == &entity;
            });

        if (it != node->entities.end())
        {
            // Check if entity's new position is still within the current node's boundaries
            if (!node->bounds.containsPosition(entity.getPosition()))
            {
                node->entities.erase(it);
                node->tryCollapse();
                root->insert(entitiesMap.at(entity.getId())); // re-classify it into the proper node
            }
        }
    }

    void forEachNode(std::function<void(const Bounds&)> consumer) const
    {
        if (root)
            root->forEachNode(consumer);
    }

private:
    int maxDepth = 4;
    int width, height;
    std::unordered_map <int, std::shared_ptr<Entity<E>>> entitiesMap{};
    struct Node;
    std::unique_ptr<Node> root;

    struct Node
    {
        Bounds bounds;
        int depth;
        int maxDepth;
        std::deque<std::shared_ptr<Entity<E>>> entities;
        Node* parent = nullptr;
        std::unique_ptr<Node> children[4]; // NW, NE, SW, SE

        Node(Bounds bounds, int depth, int maxDepth)
            : bounds(bounds), depth(depth), maxDepth(maxDepth) {}

        bool isLeaf() const
        {
            return children[0] == nullptr;
        }

        void subdivide()
        { // pointA is the top left corner, and pointB is the bottom right corner
            Point2D topLeft = bounds.getPointA();
            Point2D bottomRight = bounds.getPointB();
            Point2D center((topLeft.getX() + bottomRight.getX()) / 2.0, (topLeft.getY() + bottomRight.getY()) / 2.0);

            children[0] = std::make_unique<Node>(Bounds(            // NW (top-left quadrant)
                topLeft,
                center
            ), depth + 1, maxDepth);

            children[1] = std::make_unique<Node>(Bounds(            // NE (top-right quadrant)
                Point2D(center.getX(), topLeft.getY()),
                Point2D(bottomRight.getX(), center.getY())
            ), depth + 1, maxDepth);

            children[2] = std::make_unique<Node>(Bounds(            // SW (bottom-left quadrant)
                Point2D(topLeft.getX(), center.getY()),
                Point2D(center.getX(), bottomRight.getY())
            ), depth + 1, maxDepth);

            children[3] = std::make_unique<Node>(Bounds(            // SE (bottom-right quadrant)
                center,
                bottomRight
            ), depth + 1, maxDepth);

            children[0]->parent = this;
            children[1]->parent = this;
            children[2]->parent = this;
            children[3]->parent = this;
        }

        void insert(const std::shared_ptr<Entity<E>>& entity)
        {
            if (depth >= maxDepth)
            {
                entities.push_back(entity);
                return;
            }

            if (isLeaf())
                subdivide();

            bool inserted = false;
            for (auto& child : children)
            {
                if (child->bounds.containsPosition(entity->getPosition()))
                {
                    child->insert(entity);
                    inserted = true;
                    break;
                }
            }

            if (!inserted)
            {
                // If it somehow doesn't fit into a child node, keep it here
                entities.push_back(entity);
            }
        }

        void remove(const std::shared_ptr<Entity<E>>& entity)
        {
            if (isLeaf())
            {
                entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
            }
            else
            {
                for (auto& child : children)
                {
                    if (child->bounds.containsPosition(entity->getPosition()))
                    {
                        child->remove(entity);
                        return;
                    }
                }
            }
        }

        void tryCollapse()
        {
            if (!isLeaf())
            {
                bool allEmpty = true;
                for (auto& child : children)
                {
                    if (!child->isLeaf() || !child->entities.empty())
                    {
                        allEmpty = false;
                        break;
                    }
                }

                if (allEmpty)
                {
                    for (auto& child : children)
                        child.reset();
                }
            }

            if (parent)
                parent->tryCollapse();
        }


        void select(const Bounds& selection, std::function<void(EntityView<E>& handle)> consumer)
        {
            if (!bounds.intersects(selection))
                return;

            for (auto& entity : entities)
            {
                if (selection.containsPosition(entity->getPosition()))
                {
                    EntityView<E> safeView(entity);
                    consumer(safeView);
                }
            }

            if (!isLeaf())
            {
                for (auto& child : children)
                {
                    child->select(selection, consumer);
                }
            }
        }
        Node* findLowestNode(const Point2D& position)
        {
            if (isLeaf())
            {
                return this;
            }

            for (auto& child : children)
            {
                if (child->bounds.containsPosition(position))
                {
                    return child->findLowestNode(position);
                }
            }

            return this;
        }

        void forEachNode(std::function<void(const Bounds&)> consumer) const
        {
            consumer(bounds);
            if (!isLeaf())
            {
                for (const auto& child : children)
                {
                    if (child)
                        child->forEachNode(consumer);
                }
            }
        }

    public:
        Bounds getBounds()
        {
            return bounds;
        }
    };
};

}
