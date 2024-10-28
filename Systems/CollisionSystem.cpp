#include "CollisionSystem.h"
#include "NarrowPhase/SAT.h"
#include "../Components/TransformComponent.h"
#include "../Components/ColliderComponent.h"
#include <iostream>
#include <cfloat>

#include "../Components/IDComponent.h"
#include "../Components/ShapeType.h"
#include "../Utilities/PolygonIntersection.h"

#include "../Math/Vector2.h"

std::string shapeTypeToString(ShapeType type)
{
    switch (type)
    {
    case ShapeType::Triangle:
        return "Triangle";
    case ShapeType::Square:
        return "Square";
    case ShapeType::Pentagon:
        return "Pentagon";
    case ShapeType::Hexagon:
        return "Hexagon";
    case ShapeType::Custom:
    default:
        return "Custom Shape";
    }
}

CollisionSystem::CollisionSystem(ECS &ecs) : ecs(ecs) {}
const std::map<std::pair<Entity *, Entity *>, std::vector<Vector2>> &CollisionSystem::getIntersectionPolygons() const
{
    return intersectionPolygons;
}

void CollisionSystem::update()
{
    // Build the AABB tree with current entities
    buildAABBTree();

    // Query the tree for potential collisions
    std::vector<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>> potentialCollisions;
    tree.queryPotentialCollisions(potentialCollisions);

    // Handle collisions
    handleCollisions(potentialCollisions);
}

void CollisionSystem::buildAABBTree()
{
    tree = AABBTree(); // Reset the tree

    for (const auto &entity : ecs.getEntities())
    {
        AABB aabb = calculateAABB(entity.get());
        tree.insert(entity, aabb);
    }
}
const std::set<std::pair<Entity *, Entity *>> &CollisionSystem::getCollisionPairs() const
{
    return collisionPairs;
}

AABB CollisionSystem::calculateAABB(Entity *entity)
{
    auto transform = entity->getComponent<TransformComponent>();
    auto collider = entity->getComponent<ColliderComponent>();

    if (!transform || !collider)
    {
        return AABB();
    }

    Vector2 min(FLT_MAX, FLT_MAX);
    Vector2 max(-FLT_MAX, -FLT_MAX);

    for (const auto &vert : collider->vertices)
    {
        // Apply transformations
        Vector2 worldVert = transform->position + collider->offset + (vert * transform->scale);

        min.x = std::min(min.x, worldVert.x);
        min.y = std::min(min.y, worldVert.y);
        max.x = std::max(max.x, worldVert.x);
        max.y = std::max(max.y, worldVert.y);
    }

    return AABB(min, max);
}
void CollisionSystem::handleCollisions(const std::vector<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>> &collisions)
{
    for (const auto &pair : collisions)
    {
        Entity *entityA = pair.first.get();
        Entity *entityB = pair.second.get();

        auto transformA = entityA->getComponent<TransformComponent>();
        auto colliderA = entityA->getComponent<ColliderComponent>();
        auto idA = entityA->getComponent<IDComponent>();

        auto transformB = entityB->getComponent<TransformComponent>();
        auto colliderB = entityB->getComponent<ColliderComponent>();
        auto idB = entityB->getComponent<IDComponent>();

        if (!transformA || !colliderA || !transformB || !colliderB)
            continue;

        // Generate world space vertices
        std::vector<Vector2> shapeA_world, shapeB_world;

        // Apply rotation to shape A
        float rotationA_rad = transformA->rotation * (M_PI / 180.0f);
        for (const auto &vert : colliderA->vertices)
        {
            Vector2 localVert = vert * transformA->scale;
            float rotatedX = localVert.x * std::cos(rotationA_rad) - localVert.y * std::sin(rotationA_rad);
            float rotatedY = localVert.x * std::sin(rotationA_rad) + localVert.y * std::cos(rotationA_rad);
            Vector2 worldVert = transformA->position + colliderA->offset + Vector2(rotatedX, rotatedY);
            shapeA_world.push_back(worldVert);
        }

        // Apply rotation to shape B
        float rotationB_rad = transformB->rotation * (M_PI / 180.0f);
        for (const auto &vert : colliderB->vertices)
        {
            Vector2 localVert = vert * transformB->scale;
            float rotatedX = localVert.x * std::cos(rotationB_rad) - localVert.y * std::sin(rotationB_rad);
            float rotatedY = localVert.x * std::sin(rotationB_rad) + localVert.y * std::cos(rotationB_rad);
            Vector2 worldVert = transformB->position + colliderB->offset + Vector2(rotatedX, rotatedY);
            shapeB_world.push_back(worldVert);
        }

        // Narrow Phase collision detection
        if (SAT::checkCollision(shapeA_world, shapeB_world))
        {
            // Store the pair for visualization
            collisionPairs.insert({entityA, entityB});

            // Compute the intersection polygon
            std::vector<Vector2> intersectionPolygon = PolygonIntersection::computeIntersection(shapeA_world, shapeB_world);
            if (!intersectionPolygon.empty())
            {
                intersectionPolygons[{entityA, entityB}] = intersectionPolygon;
            }

            // Output detailed collision information
            std::string idA_str = idA ? idA->id : "Unknown";
            std::string idB_str = idB ? idB->id : "Unknown";

            std::string shapeA_str = shapeTypeToString(colliderA->shapeType);
            std::string shapeB_str = shapeTypeToString(colliderB->shapeType);

            Vector2 posA = transformA->position;
            Vector2 posB = transformB->position;

            // std::cout << "Collision detected between Entity " << idA_str << " (" << shapeA_str << ") at position (" << posA.x << ", " << posA.y << ")"
            //           << " and Entity " << idB_str << " (" << shapeB_str << ") at position (" << posB.x << ", " << posB.y << ")\n";
        }
    }
}