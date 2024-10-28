#pragma once
// this is not idli dosa coffee lol :)
#include <string>

struct IDComponent
{
    std::string id;

    IDComponent(const std::string &identifier = "")
        : id(identifier) {}
};
