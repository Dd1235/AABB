#include "MovementSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/ColliderComponent.h"
#include <cfloat>
#include <SFML/Graphics.hpp>

MovementSystem::MovementSystem(ECS &ecs) : ecs(ecs) {}

void MovementSystem::update(float deltaTime, const sf::Vector2u &windowSize)
{
    for (const auto &entity : ecs.getEntities())
    {
        auto transform = entity->getComponent<TransformComponent>();
        auto velocity = entity->getComponent<VelocityComponent>();
        auto collider = entity->getComponent<ColliderComponent>();

        if (transform && velocity && collider)
        {
            // Update position
            transform->position = transform->position + (velocity->velocity * deltaTime);

            // Get entity bounds
            Vector2 min(FLT_MAX, FLT_MAX);
            Vector2 max(-FLT_MAX, -FLT_MAX);

            // Convert rotation to radians
            float rotationRad = transform->rotation * (M_PI / 180.0f);

            for (const auto &vert : collider->vertices)
            {
                Vector2 localVert = vert * transform->scale;

                // Apply rotation
                float rotatedX = localVert.x * std::cos(rotationRad) - localVert.y * std::sin(rotationRad);
                float rotatedY = localVert.x * std::sin(rotationRad) + localVert.y * std::cos(rotationRad);

                Vector2 worldVert = transform->position + collider->offset + Vector2(rotatedX, rotatedY);

                min.x = std::min(min.x, worldVert.x);
                min.y = std::min(min.y, worldVert.y);
                max.x = std::max(max.x, worldVert.x);
                max.y = std::max(max.y, worldVert.y);
            }

            // Check window bounds using dynamic window size
            float windowWidth = static_cast<float>(windowSize.x);
            float windowHeight = static_cast<float>(windowSize.y);

            if (min.x < 0.0f || max.x > windowWidth)
            {
                velocity->velocity.x *= -1.0f;
                // Correct position if out of bounds
                if (min.x < 0.0f)
                {
                    transform->position.x += -min.x;
                }
                else if (max.x > windowWidth)
                {
                    transform->position.x -= (max.x - windowWidth);
                }
            }
            if (min.y < 0.0f || max.y > windowHeight)
            {
                velocity->velocity.y *= -1.0f;
                // Correct position if out of bounds
                if (min.y < 0.0f)
                {
                    transform->position.y += -min.y;
                }
                else if (max.y > windowHeight)
                {
                    transform->position.y -= (max.y - windowHeight);
                }
            }
        }
    }
}