#pragma once

#include "../Core/ECS.h"

class MovementSystem
{
public:
    MovementSystem(ECS &ecs);
    void update(float deltaTime);

private:
    ECS &ecs;
};
