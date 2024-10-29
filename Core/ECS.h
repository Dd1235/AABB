#pragma once

#include <vector>
#include <memory>
#include "../Entities/Entity.h"

class ECS
{
public:
    void addEntity(const std::shared_ptr<Entity> &entity);

    const std::vector<std::shared_ptr<Entity>> &getEntities() const;
    bool paused = false;

private:
    std::vector<std::shared_ptr<Entity>> entities;
};
