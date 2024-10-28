#include "AABBTree.h"

AABBTreeNode::AABBTreeNode() : aabb(), entity(nullptr), left(nullptr), right(nullptr) {}

AABBTreeNode::AABBTreeNode(const AABB &box, const std::shared_ptr<Entity> &ent)
    : aabb(box), entity(ent), left(nullptr), right(nullptr) {}

AABBTree::AABBTree() : root(nullptr) {}

void AABBTree::insert(const std::shared_ptr<Entity> &entity, const AABB &aabb)
{
    insertNode(root, entity, aabb);
}

void AABBTree::insertNode(std::shared_ptr<AABBTreeNode> &currentNode, const std::shared_ptr<Entity> &entity, const AABB &aabb)
{
    if (!currentNode)
    {
        currentNode = std::make_shared<AABBTreeNode>(aabb, entity);
        return;
    }

    // if current node is leaf
    if (currentNode->entity)
    {
        // Convert leaf node to internal node
        auto oldEntity = currentNode->entity;
        auto oldAABB = currentNode->aabb;

        currentNode->entity = nullptr;

        currentNode->left = std::make_shared<AABBTreeNode>(oldAABB, oldEntity);
        currentNode->right = std::make_shared<AABBTreeNode>(aabb, entity);

        // Update parent AABB
        currentNode->aabb.min.x = std::min(currentNode->left->aabb.min.x, currentNode->right->aabb.min.x);
        currentNode->aabb.min.y = std::min(currentNode->left->aabb.min.y, currentNode->right->aabb.min.y);
        currentNode->aabb.max.x = std::max(currentNode->left->aabb.max.x, currentNode->right->aabb.max.x);
        currentNode->aabb.max.y = std::max(currentNode->left->aabb.max.y, currentNode->right->aabb.max.y);
    }
    else
    {
        // Update AABB to encompass the new entity's AABB as well
        currentNode->aabb.min.x = std::min(currentNode->aabb.min.x, aabb.min.x);
        currentNode->aabb.min.y = std::min(currentNode->aabb.min.y, aabb.min.y);
        currentNode->aabb.max.x = std::max(currentNode->aabb.max.x, aabb.max.x);
        currentNode->aabb.max.y = std::max(currentNode->aabb.max.y, aabb.max.y);

        // Heuristic: insert into the child with smaller increase in perimeter
        float leftPerimeterIncrease = 0.0f;
        float rightPerimeterIncrease = 0.0f;

        if (currentNode->left)
        {
            AABB leftUnion = currentNode->left->aabb;
            leftUnion.min.x = std::min(leftUnion.min.x, aabb.min.x);
            leftUnion.min.y = std::min(leftUnion.min.y, aabb.min.y);
            leftUnion.max.x = std::max(leftUnion.max.x, aabb.max.x);
            leftUnion.max.y = std::max(leftUnion.max.y, aabb.max.y);

            leftPerimeterIncrease = (leftUnion.max.x - leftUnion.min.x + leftUnion.max.y - leftUnion.min.y) * 2.0f -
                                    (currentNode->left->aabb.max.x - currentNode->left->aabb.min.x + currentNode->left->aabb.max.y - currentNode->left->aabb.min.y) * 2.0f;
        }

        if (currentNode->right)
        {
            AABB rightUnion = currentNode->right->aabb;
            rightUnion.min.x = std::min(rightUnion.min.x, aabb.min.x);
            rightUnion.min.y = std::min(rightUnion.min.y, aabb.min.y);
            rightUnion.max.x = std::max(rightUnion.max.x, aabb.max.x);
            rightUnion.max.y = std::max(rightUnion.max.y, aabb.max.y);

            rightPerimeterIncrease = (rightUnion.max.x - rightUnion.min.x + rightUnion.max.y - rightUnion.min.y) * 2.0f -
                                     (currentNode->right->aabb.max.x - currentNode->right->aabb.min.x + currentNode->right->aabb.max.y - currentNode->right->aabb.min.y) * 2.0f;
        }

        if (!currentNode->left || leftPerimeterIncrease <= rightPerimeterIncrease)
        {
            insertNode(currentNode->left, entity, aabb);
        }
        else
        {
            insertNode(currentNode->right, entity, aabb);
        }
    }
}

void AABBTree::queryPotentialCollisions(std::vector<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>> &collisions) const
{
    if (!root)
        return;
    queryNode(root, collisions);
}

// recursively traverses teh tree structure to identify and ocllect pairs of entities whose AABBs intesect.
void AABBTree::queryNode(const std::shared_ptr<AABBTreeNode> &node, std::vector<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>> &collisions) const
{
    if (!node || (!node->left && !node->right))
        return;

    if (node->left && node->right)
    {
        if (node->left->aabb.intersects(node->right->aabb))
        {
            collectLeaves(node->left, node->right, collisions);
        }

        queryNode(node->left, collisions);
        queryNode(node->right, collisions);
    }
}

// helper function that recursively explores two nodes nodeA and ndoeB to collect all pairs of intersecting leaf ndoes
void AABBTree::collectLeaves(const std::shared_ptr<AABBTreeNode> &nodeA, const std::shared_ptr<AABBTreeNode> &nodeB, std::vector<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>> &collisions) const
{
    if (!nodeA || !nodeB)
        return;

    if (nodeA->entity && nodeB->entity)
    {
        // Both are leaves
        collisions.emplace_back(nodeA->entity, nodeB->entity);
    }
    else if (nodeA->entity)
    {
        // nodeA is leaf, nodeB is internal
        collectLeaves(nodeA, nodeB->left, collisions);
        collectLeaves(nodeA, nodeB->right, collisions);
    }
    else if (nodeB->entity)
    {
        // nodeB is leaf, nodeA is internal
        collectLeaves(nodeA->left, nodeB, collisions);
        collectLeaves(nodeA->right, nodeB, collisions);
    }
    else
    {
        // Both are internal
        collectLeaves(nodeA->left, nodeB->left, collisions);
        collectLeaves(nodeA->left, nodeB->right, collisions);
        collectLeaves(nodeA->right, nodeB->left, collisions);
        collectLeaves(nodeA->right, nodeB->right, collisions);
    }
}
