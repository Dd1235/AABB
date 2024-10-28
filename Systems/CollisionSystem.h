#pragma once

#include "../Core/ECS.h"
#include "BroadPhase/AABBTree.h"
#include <set>
#include <map>

class CollisionSystem
{
public:
    CollisionSystem(ECS &ecs);
    void update();

    // Getter for collision pairs
    const std::set<std::pair<Entity *, Entity *>> &getCollisionPairs() const;

    // Getter for intersection polygons, this si for visualization purposes
    const std::map<std::pair<Entity *, Entity *>, std::vector<Vector2>> &getIntersectionPolygons() const;

private:
    ECS &ecs;
    AABBTree tree;

    std::set<std::pair<Entity *, Entity *>> collisionPairs;
    std::map<std::pair<Entity *, Entity *>, std::vector<Vector2>> intersectionPolygons;

    void buildAABBTree();
    AABB calculateAABB(Entity *entity);
    void handleCollisions(const std::vector<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>> &collisions);
};
