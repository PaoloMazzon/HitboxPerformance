# Hitbox Performance

## File Overview
Below is a brief overview of what each file in the source tree is doing.

  - `extern/` External git submodule libraries that will be built from source
  - `include/HitboxPerformance/`
    - `App.hpp` High-level interface containing all the actual work
    - `Constants.hpp` Constants for the whole application
    - `RendererState.hpp` Singleton holding the SDL window and renderer
  - `src/main.cpp` Handles all SDL boilerplate and calls the `App.cpp` functions
  - `src/*` Implementation for the headers
  - `CMakeLists.txt` Top-level project cmake file