#pragma once

#include "../Core/ECS.h"
#include <SFML/Graphics.hpp>

class MovementSystem
{
public:
    MovementSystem(ECS &ecs);
    void update(float deltaTime, const sf::Vector2u &windowSize);

private:
    ECS &ecs;
};
