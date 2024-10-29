#include <iostream>
#include "Core/ECS.h"
#include "Systems/CollisionSystem.h"
#include "Systems/MovementSystem.h"
#include "Components/TransformComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/VelocityComponent.h"
#include "Components/IDComponent.h" // Include IDComponent
#include "Math/Vector2.h"
#include <memory>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Utilities/PolygonIntersection.h"
#include "Utilities/PolygonUtils.h"

// Include ShapeFactory
#include "Utilities/ShapeFactory.h"

// Include SFML for visualization
#include <SFML/Graphics.hpp>

void drawEntity(sf::RenderWindow &window, Entity *entity, bool isColliding, const std::vector<std::vector<Vector2>> &collisionPolygons);

struct ShapeData
{
    std::vector<Vector2> vertices;
    ShapeType type;
};

int main()
{
    ECS ecs;

    // Seed random number generator
    std::srand(static_cast<unsigned>(std::time(0)));

    // Create shapes
    std::vector<ShapeData> shapes = {
        {ShapeFactory::createRegularPolygon(3, 30.0f), // Triangle
         ShapeType::Triangle},
        {ShapeFactory::createRegularPolygon(4, 30.0f), // Square
         ShapeType::Square},
        {ShapeFactory::createRegularPolygon(5, 30.0f), // Pentagon
         ShapeType::Pentagon},
        {ShapeFactory::createRegularPolygon(6, 30.0f), // Hexagon
         ShapeType::Hexagon}};

    // Create multiple entities
    const int entityCount = 10;
    for (int i = 0; i < entityCount; ++i)
    {
        auto entity = std::make_shared<Entity>();

        // Random position
        Vector2 position(std::rand() % 700 + 50, std::rand() % 500 + 50);

        // Random velocity
        // Vector2 velocity((std::rand() % 200 - 100) / 50.0f, (std::rand() % 200 - 100) / 50.0f);
        Vector2 velocity((std::rand() % 400 - 200) / 10.0f, (std::rand() % 400 - 200) / 10.0f);

        // Random shape
        const auto &shapeData = shapes[std::rand() % shapes.size()];

        // Assign an ID
        std::string id = "Entity_" + std::to_string(i);

        entity->addComponent<IDComponent>(IDComponent(id));
        entity->addComponent<TransformComponent>(TransformComponent(position));
        entity->addComponent<VelocityComponent>(VelocityComponent(velocity));
        entity->addComponent<ColliderComponent>(ColliderComponent(shapeData.vertices, shapeData.type));

        ecs.addEntity(entity);
    }

    // Create Systems
    CollisionSystem collisionSystem(ecs);
    MovementSystem movementSystem(ecs);

    // Setup SFML window for visualization
    sf::RenderWindow window(sf::VideoMode(800, 600), "Collision Detection Visualization", sf::Style::Resize);
    window.setFramerateLimit(60);

    // Clock for delta time
    sf::Clock clock;

    // Pause flag
    bool isPaused = false;

    // Game loop
    while (window.isOpen())
    {
        // Calculate delta time
        float deltaTime = clock.restart().asSeconds();

        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::P)
                {
                    isPaused = !isPaused; // Toggle pause state
                    std::cout << (isPaused ? "Paused" : "Unpaused") << std::endl;
                }
            }
            if (event.type == sf::Event::Resized)
            {
                // Optionally, adjust the view to the new window size
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        // update the systems
        if (!isPaused)
        {
            sf::Vector2u windowSize = window.getSize();
            movementSystem.update(deltaTime, windowSize);
            collisionSystem.update();
        }

        // Clear the window
        window.clear(sf::Color::Black);

        // Map to store collision polygons for each entity
        std::map<Entity *, std::vector<std::vector<Vector2>>> entityCollisionPolygons;

        // Build the map from collision system data
        for (const auto &collisionPair : collisionSystem.getIntersectionPolygons())
        {
            Entity *entityA = collisionPair.first.first;
            Entity *entityB = collisionPair.first.second;
            const auto &intersectionPolygon = collisionPair.second;

            entityCollisionPolygons[entityA].push_back(intersectionPolygon);
            entityCollisionPolygons[entityB].push_back(intersectionPolygon);
        }

        // Draw entities
        for (const auto &entity : ecs.getEntities())
        {
            // Check if entity is colliding
            bool isColliding = entityCollisionPolygons.find(entity.get()) != entityCollisionPolygons.end();

            // Get collision polygons for this entity
            const auto &collisionPolygons = entityCollisionPolygons[entity.get()];

            drawEntity(window, entity.get(), isColliding, collisionPolygons);
        }

        // Display the contents of the window
        window.display();
    }

    return 0;
}

void drawEntity(sf::RenderWindow &window, Entity *entity, bool isColliding, const std::vector<std::vector<Vector2>> &collisionPolygons)
{
    auto transform = entity->getComponent<TransformComponent>();
    auto collider = entity->getComponent<ColliderComponent>();

    if (!transform || !collider)
        return;

    sf::ConvexShape shape;
    size_t vertexCount = collider->vertices.size();
    shape.setPointCount(vertexCount);

    // Convert rotation to radians
    float rotationRad = transform->rotation * (M_PI / 180.0f);

    for (size_t i = 0; i < vertexCount; ++i)
    {
        Vector2 localVert = collider->vertices[i] * transform->scale;

        // Apply rotation
        float rotatedX = localVert.x * std::cos(rotationRad) - localVert.y * std::sin(rotationRad);
        float rotatedY = localVert.x * std::sin(rotationRad) + localVert.y * std::cos(rotationRad);

        Vector2 worldVert = transform->position + collider->offset + Vector2(rotatedX, rotatedY);
        shape.setPoint(i, sf::Vector2f(worldVert.x, worldVert.y));
    }

    // Set original color
    shape.setFillColor(sf::Color(0, 255, 0, 100)); // Semi-transparent green
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(1.0f);

    window.draw(shape);
    for (const auto &poly : collisionPolygons)
    {
        size_t polyVertexCount = poly.size();
        if (polyVertexCount < 3)
            continue; // Not a valid polygon

        // Compute area
        float area = PolygonUtils::computeArea(poly);
        if (area < 1.0f)
            continue; // Skip tiny polygons

        sf::ConvexShape intersectionShape;
        intersectionShape.setPointCount(polyVertexCount);
        for (size_t i = 0; i < polyVertexCount; ++i)
        {
            intersectionShape.setPoint(i, sf::Vector2f(poly[i].x, poly[i].y));
        }

        intersectionShape.setFillColor(sf::Color(255, 0, 0, 150)); // Semi-transparent red
        intersectionShape.setOutlineColor(sf::Color::Yellow);
        intersectionShape.setOutlineThickness(1.0f);

        window.draw(intersectionShape);
    }
}
