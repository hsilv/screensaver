cmake_minimum_required(VERSION 3.10)
project(MySDLProject)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Find GLM
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# Include directories
include_directories(lib)

# Source files
set(SOURCES
    src/window.cpp
    main.cpp
)

# Create executable
add_executable(MySDLProject ${SOURCES})

# Link SDL2 and GLM
target_link_libraries(MySDLProject ${SDL2_LIBRARIES} glm::glm)