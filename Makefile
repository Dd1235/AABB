# Makefile for the Collision Detection Project

# Compiler and Flags, update according to your system!
CXX = g++
CXXFLAGS = -g -std=c++11 -Wall -I./ -I/opt/homebrew/Cellar/sfml/2.6.1/include

# SFML Library Paths, update SFML_LIB_DIR according to your system!
SFML_LIB_DIR = /opt/homebrew/Cellar/sfml/2.6.1/lib
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Source Files
SRC = \
    main.cpp \
    Core/ECS.cpp \
    Systems/CollisionSystem.cpp \
    Systems/MovementSystem.cpp \
    Systems/BroadPhase/AABB.cpp \
    Systems/BroadPhase/AABBTree.cpp \
    Systems/NarrowPhase/SAT.cpp \
    Math/Vector2.cpp \
    Utilities/ShapeFactory.cpp \
    Utilities/PolygonIntersection.cpp \
    Utilities/PolygonUtils.cpp


# Object Files
OBJ = $(SRC:.cpp=.o)

# Output Executable
TARGET = collision_example

# Default Rule
all: $(TARGET)

# Build Target
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) -L$(SFML_LIB_DIR) $(SFML_LIBS)

# Compile .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Rule
clean:
	rm -f $(OBJ) $(TARGET)

# Phony Targets
.PHONY: all clean
