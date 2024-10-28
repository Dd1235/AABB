#include "ECS.h"

void ECS::addEntity(const std::shared_ptr<Entity> &entity)
{
    entities.push_back(entity);
}

const std::vector<std::shared_ptr<Entity>> &ECS::getEntities() const
{
    return entities;
}
