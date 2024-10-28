#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>

class Entity
{
public:
    template <typename T>
    void addComponent(const T &component);

    template <typename T>
    T *getComponent();

private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> components;
};

#include "Entity.inl"
