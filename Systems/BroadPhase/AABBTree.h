#pragma once

#include <vector>
#include <memory>
#include "../../Entities/Entity.h"
#include "AABB.h"

// each node can either be a leaf node containing an entity and an AABB or an internal node that define a region by combining its child notes' bounding boxes

class AABBTreeNode
{
public:
    AABB aabb; // represents the region covered by this node and its children

    std::shared_ptr<Entity> entity; // points to an entity if leaf else nullptr
    std::shared_ptr<AABBTreeNode> left;
    std::shared_ptr<AABBTreeNode> right;

    AABBTreeNode();
    AABBTreeNode(const AABB &box, const std::shared_ptr<Entity> &ent = nullptr);
};

class AABBTree
{
public:
    AABBTree();
    void insert(const std::shared_ptr<Entity> &entity, const AABB &aabb);
    void queryPotentialCollisions(std::vector<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>> &collisions) const;

private:
    std::shared_ptr<AABBTreeNode> root;

    // recursivelt finds the correct position in the tree
    void insertNode(std::shared_ptr<AABBTreeNode> &currentNode, const std::shared_ptr<Entity> &entity, const AABB &aabb);
    void queryNode(const std::shared_ptr<AABBTreeNode> &node, std::vector<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>> &collisions) const;
    void collectLeaves(const std::shared_ptr<AABBTreeNode> &nodeA, const std::shared_ptr<AABBTreeNode> &nodeB, std::vector<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>> &collisions) const;
};
