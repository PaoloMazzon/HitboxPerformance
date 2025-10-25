# Hitbox Performance

## Building
To build this project you need to clone this repo and its submodules, then use CMake
to generate and whatever else to build.

```bash
git clone https://github.com/PaoloMazzon/HitboxPerformance
cd HitboxPerformance
git submodule init --recursive
cmake CMakeLists.txt
cmake --build ./build
./build/Hitbox-Performance
```

## File Overview
Below is a brief overview of what each file in the source tree is doing.

  - `extern/` External git submodule libraries that will be built from source
  - `include/HitboxPerformance/`
    - `App.hpp` High-level interface containing all the actual work
    - `Constants.hpp` Constants for the whole application
    - `RendererState.hpp` Singleton holding the SDL window and renderer
    - `Hitbox.hpp` Hitbox and bounding box code
    - `SpatialHashmap.hpp` Spatial hashmap code, handles organizing hitboxes in space
  - `src/main.cpp` Handles all SDL boilerplate and calls the `App.cpp` functions
  - `src/*` Implementation for the headers
  - `CMakeLists.txt` Top-level project cmake file

## 3rd Party
This project uses two other projects:

 - [SDL3](https://github.com/libsdl-org/SDL) (zlib)
 - [spdlog](https://github.com/gabime/spdlog) (MIT)