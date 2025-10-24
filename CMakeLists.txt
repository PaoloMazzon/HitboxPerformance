cmake_minimum_required(VERSION 3.20)
project(Hitbox-Performance LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add 3rd party tools
add_subdirectory(extern/SDL EXCLUDE_FROM_ALL)
add_subdirectory(extern/spdlog EXCLUDE_FROM_ALL)

include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/RendererState.cpp src/App.cpp src/Hitbox.cpp src/SpatialHashmap.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3::SDL3 spdlog)