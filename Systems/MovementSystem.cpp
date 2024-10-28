#include "MovementSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/ColliderComponent.h"
#include <cfloat>

MovementSystem::MovementSystem(ECS &ecs) : ecs(ecs) {}

// void MovementSystem::update(float deltaTime)
// {
//     for (const auto &entity : ecs.getEntities())
//     {
//         auto transform = entity->getComponent<TransformComponent>();
//         auto velocity = entity->getComponent<VelocityComponent>();

//         if (transform && velocity)
//         {
//             transform->position = transform->position + (velocity->velocity * deltaTime);
//         }
//     }
// }
void MovementSystem::update(float deltaTime)
{
    for (const auto &entity : ecs.getEntities())
    {
        auto transform = entity->getComponent<TransformComponent>();
        auto velocity = entity->getComponent<VelocityComponent>();
        auto collider = entity->getComponent<ColliderComponent>();

        if (transform && velocity && collider)
        {
            transform->position = transform->position + (velocity->velocity * deltaTime);

            // Get entity bounds
            Vector2 min(FLT_MAX, FLT_MAX);
            Vector2 max(-FLT_MAX, -FLT_MAX);

            for (const auto &vert : collider->vertices)
            {
                Vector2 worldVert = transform->position + collider->offset + (vert * transform->scale);

                min.x = std::min(min.x, worldVert.x);
                min.y = std::min(min.y, worldVert.y);
                max.x = std::max(max.x, worldVert.x);
                max.y = std::max(max.y, worldVert.y);
            }

            // Check window bounds (assuming window size 800x600)
            if (min.x < 0.0f || max.x > 800.0f)
            {
                velocity->velocity.x *= -1.0f;
            }
            if (min.y < 0.0f || max.y > 600.0f)
            {
                velocity->velocity.y *= -1.0f;
            }
        }
    }
}
