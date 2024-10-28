# AABB (Work in Progress)

This repo consists of my attempt at implementing  AABB collision detection in CPP. 

# Collision Detection with ECS Architecture in C++

This project demonstrates collision detection between convex polygons using an Entity-Component-System (ECS) architecture in C++. The collision detection is visualized using the SFML library, highlighting only the overlapping regions during collisions.

---

## **Table of Contents**

- [Introduction](#introduction)
- [Features](#features)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Building the Project](#building-the-project)
- [Running the Application](#running-the-application)
- [Usage](#usage)
- [Code Overview](#code-overview)
  - [Components](#components)
  - [Systems](#systems)
  - [Utilities](#utilities)
  - [Main Application](#main-application)
- [Customization](#customization)
- [Notes](#notes)
- [Troubleshooting](#troubleshooting)
- [Resources](#resources)

---

## **Introduction**

This project implements a collision detection system using the ECS architecture. It features:

- **Broad Phase Collision Detection**: Using Axis-Aligned Bounding Box (AABB) trees to efficiently find potential collision pairs.
- **Narrow Phase Collision Detection**: Using the Separating Axis Theorem (SAT) for precise collision detection between convex polygons.
- **Visualization**: Using SFML to render shapes and visually highlight collision areas.
- **Dynamic Shapes**: Supports multiple convex shapes like triangles, squares, pentagons, etc.
- **Collision Visualization**: Only the overlapping regions of colliding shapes are highlighted, providing clear visual feedback.

---

## **Features**

- Efficient collision detection between multiple moving shapes.
- Visual representation of collision areas.
- Shapes revert to their original color when not colliding.
- Supports rotation and scaling of shapes.
- Easy-to-extend ECS architecture.

---

## **Project Structure**

```
Project/
│
├── Components/
│   ├── ColliderComponent.h
│   ├── IDComponent.h
│   ├── ShapeType.h
│   ├── TransformComponent.h
│   └── VelocityComponent.h
│
├── Systems/
│   ├── CollisionSystem.h
│   ├── CollisionSystem.cpp
│   ├── MovementSystem.h
│   ├── MovementSystem.cpp
│   ├── BroadPhase/
│   │   ├── AABB.h
│   │   ├── AABB.cpp
│   │   ├── AABBTree.h
│   │   └── AABBTree.cpp
│   └── NarrowPhase/
│       ├── SAT.h
│       └── SAT.cpp
│
├── Entities/
│   ├── Entity.h
│   └── Entity.inl
│
├── Math/
│   ├── Vector2.h
│   └── Vector2.cpp
│
├── Utilities/
│   ├── ShapeFactory.h
│   ├── ShapeFactory.cpp
│   ├── PolygonIntersection.h
│   └── PolygonIntersection.cpp
│
├── Core/
│   ├── ECS.h
│   └── ECS.cpp
│
├── main.cpp
├── Makefile
└── README.md (this file)
```

---

## **Prerequisites**

- **C++ Compiler**: A compiler that supports C++11 or higher (e.g., g++, clang++).
- **SFML Library**: SFML version 2.6.1.

---

## **Installation**

1. **Install SFML**:

   On macOS using Homebrew:

   ```bash
   brew install sfml
   ```

   Ensure that SFML is installed and make necessary adjustments to the `Makefile` if SFML is installed in a different location.

2. **Clone the Repository**:

   ```bash
   git clone https://github.com/Dd1235/AABB.git
   cd AABB
   ```

---

## **Building the Project**

### **Using the Provided Makefile**

1. **Navigate to the Project Directory**:

   ```bash
   cd path/to/Project
   ```

2. **Build the Project**:

   ```bash
   make
   ```

   This will compile all source files and create the executable `collision_example`.

### **Adjusting the Makefile (if necessary)**

- **SFML Paths**: If SFML is installed in a different location, update the following variables in the `Makefile`:

  ```Makefile
  CXXFLAGS = -std=c++11 -Wall -I./ -I/path/to/sfml/include
  SFML_LIB_DIR = /path/to/sfml/lib
  ```

---

## **Running the Application**

After successful compilation, run the application:

```bash
./collision_example
```

---

## **Usage**

- **Visualization**: A window will open displaying multiple shapes moving around.
- **Collision Indication**:
  - **Normal State**: Shapes are drawn in semi-transparent green.
  - **Collision State**: Overlapping regions are highlighted in semi-transparent red.
- **Exiting the Application**: Close the window or press the close button.

---

## **Code Overview**

### **Components**

- **TransformComponent** (`Components/TransformComponent.h`):
  - Stores position, rotation (in degrees), and scale of an entity.

- **VelocityComponent** (`Components/VelocityComponent.h`):
  - Stores the velocity vector of an entity.

- **ColliderComponent** (`Components/ColliderComponent.h`):
  - Stores the vertices defining the shape of the collider.
  - Includes the shape type (`ShapeType`).

- **IDComponent** (`Components/IDComponent.h`):
  - Assigns a unique identifier to each entity.

- **ShapeType** (`Components/ShapeType.h`):
  - Enumeration of supported shape types (e.g., Triangle, Square, Pentagon).

### **Systems**

- **CollisionSystem** (`Systems/CollisionSystem.h` / `.cpp`):
  - Performs collision detection between entities.
  - Uses AABB trees for broad-phase detection.
  - Uses SAT for narrow-phase detection.
  - Computes intersection polygons for visualization.

- **MovementSystem** (`Systems/MovementSystem.h` / `.cpp`):
  - Updates the positions of entities based on their velocities.
  - Implements screen edge bouncing logic.

- **BroadPhase** (`Systems/BroadPhase/`):
  - **AABB** (`AABB.h` / `.cpp`): Represents an Axis-Aligned Bounding Box.
  - **AABBTree** (`AABBTree.h` / `.cpp`): Implements an AABB tree for efficient collision culling.

- **NarrowPhase** (`Systems/NarrowPhase/`):
  - **SAT** (`SAT.h` / `.cpp`): Implements the Separating Axis Theorem for precise collision detection.

### **Utilities**

- **ShapeFactory** (`Utilities/ShapeFactory.h` / `.cpp`):
  - Generates standard convex polygons (triangles, squares, pentagons, etc.).

- **PolygonIntersection** (`Utilities/PolygonIntersection.h` / `.cpp`):
  - Computes the intersection polygon between two convex shapes using the Sutherland-Hodgman algorithm.

### **Main Application**

- **main.cpp**:
  - Sets up the ECS, systems, and entities.
  - Initializes entities with random positions, velocities, and shapes.
  - Runs the main game loop, updating systems and rendering entities.
  - Handles collision visualization by drawing overlapping regions.

---

## **Customization**

- **Adding New Shapes**:
  - Extend `ShapeFactory` to create additional shapes.
  - Update `ShapeType` enumeration accordingly.

- **Adjusting Entity Count**:
  - Modify the `entityCount` variable in `main.cpp` to increase or decrease the number of entities.

- **Changing Movement Behavior**:
  - Modify `MovementSystem` to implement different movement patterns or behaviors.

- **Implementing Collision Response**:
  - In `CollisionSystem`, add logic to adjust entity velocities upon collision detection.

---

## **Notes**

- **Performance**:
  - The application is designed for demonstration purposes. Performance may vary with a large number of entities.
  - Consider optimizing the AABB tree or implementing spatial partitioning for better performance with many entities.

- **Collision Detection Limitations**:
  - The implementation assumes all shapes are convex polygons.
  - For concave shapes, additional algorithms or decomposition into convex parts would be necessary.

- **SFML Integration**:
  - SFML is used for visualization. Ensure that SFML is correctly installed and linked.

---

## **Troubleshooting**

- **Compilation Errors**:
  - Ensure all source files are included in the `Makefile`.
  - Verify that the SFML include and library paths are correct.


## **Resources**

- (Polygon Clipping Algorithm)[https://en.wikipedia.org/wiki/Sutherland–Hodgman_algorithm]