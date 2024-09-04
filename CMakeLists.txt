cmake_minimum_required(VERSION 3.10)
project(MySDLProject)

# Set CMake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Find PortAudio
find_package(PortAudio REQUIRED)
include_directories(${PORTAUDIO_INCLUDE_DIRS})

# Find FFTW3
find_package(FFTW REQUIRED)
include_directories(${FFTW_INCLUDE_DIRS})

# Find SDL2_ttf
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIRS})

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/lib)

# Source files
set(SOURCES
    src/window.cpp
    src/audio.cpp
    src/filters.cpp
    src/render.cpp
    main.cpp
)

# Create executable
add_executable(MySDLProject ${SOURCES})

# Link SDL2, PortAudio, FFTW3, and SDL2_ttf
target_link_libraries(MySDLProject ${SDL2_LIBRARIES} ${PORTAUDIO_LIBRARIES} ${FFTW_LIBRARIES} ${SDL2_TTF_LIBRARIES} fftw3f SDL2_ttf)