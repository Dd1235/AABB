#pragma once

template <typename T>
void Entity::addComponent(const T &component)
{
    components[std::type_index(typeid(T))] = std::make_shared<T>(component);
}

template <typename T>
T *Entity::getComponent()
{
    auto it = components.find(std::type_index(typeid(T)));
    if (it != components.end())
    {
        return static_cast<T *>(it->second.get());
    }
    return nullptr;
}
