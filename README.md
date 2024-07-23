# SkylineChallenege2
Second challenge for Skyline Nav AI.

## Features
- Heightmap-based terrain generation.
- Camera movement and rotation via mouse and keyboard (WASD keys)
- Dynamic lighting system
- Wireframe toggle mode 
- Normal vector visualization 
- Light placement 
- Performance testing mode
- GPU-accelerated normal calculations using compute shaders
- CPU-based normal calculations for comparison

## Dependencies 
For you to build and run the main code, you need the following dependencies:

- GLFW
- GLEW 
- GLM 
- stb_image (included)

When I made this program, I did so on Linux, specifically on an Ubuntu based distro. To install these depenencies on Ubuntu/Debian based distros, run the following command on a Bash terminal:

`sudo apt install libglfw3-dev libglew-dev libglm-dev`

Only Windows, you must download each depdency through their website. 
GLFW LInk: https://www.glfw.org/download
GLEW Link: https://glew.sourceforge.net/index.html

## Building
To build the project, do the following: 
1. Have all dependencies ready.
2. Open a terminal in the project directory. 
3. Run the following command: 'make'
    3a. If this does not work, you might need to download cmake. Can be done on bash with following command: `sudo apt install build-essential cmake`
4. Once terrain_renderer has been made, run it by typing './terrain_renderer <heightmap_path> [--performance] [--cpu-only]'
- `<heightmap_path>`: Path to the heightmap image file to be used.
- `--performance`: Optional flag to have it start in performance mode.
- `--cpu-only`: Optional flag to use CPU-only rendering (disables GPU compute shaders)

For testing purposes, I've included a file I've been using - `World_elevation_map.png`, however, any other file works. 

## Controls 
- WASD: Moves camera
- Mouse: Rotate the camera (Must be holding left mouse button to rotate)
- P: Toggle wireframe mode
- N: Toggle vector visualization
- L: Place light at current position
- ESC: Exit program

## Structure

- `main.cpp`: Entry point and main rendering loop
- `terrain.h/cpp`: Terrain generation and rendering
- `camera.h/cpp`: Camera management
- `input.h/cpp`: Input processing
- `light.h/cpp`: Light structure and cube rendering for light visualization
- `window.h/cpp`: GLFW window management

## GPU Kernel Optimization

GPU kernels are useful when used here for the following reasons:

1. Parallelism: Normal calculations for each vertex can be performed independently, making it an ideal candidate for parallel computation on the GPU.
2. Data locality: The terrain data is already stored in GPU memory for rendering, so compute shaders can access this data efficiently.
3. Offloading work from CPU: By moving the normal calculations to the GPU, we free up CPU resources for other tasks.

## Performance Comparison

- Your performance will be dependent on your computers specs, however, running with GPU should overall yield a higher average FPS, lower average frame time, and lower normal calculation time. As an example, here are my results. 

When running without --cpu-only:
Average FPS: 144.05
Average Frame Time: 6.94 ms
Average Normal Calculation Time: 0.01 ms
Triangle Count: 79202

When running with --cpu-only:
Average FPS: 59.96
Average Frame Time: 16.68 ms
Average Normal Calculation Time: 15.78 ms
Triangle Count: 79202

For reference, I have an EVGA Geforce GTX 1060 with 6GB of VRAM and a Ryzen 5 3600 CPU 6-core CPU.

## Potential bottlenecks

1. Memory transfer: Although we've minimized data transfer between CPU and GPU, there might still be some overhead in updating dynamic terrain data.
2. Texture sampling: For large terrains, cache misses during height map texture sampling could impact performance.
3. Draw calls: A high number of draw calls for complex terrains could limit performance.

### Further Optimization Opportunities

1. Level of Detail (LOD): Implement a LOD system to reduce the number of triangles rendered for distant terrain parts.
2. Tessellation: Use tessellation shaders to dynamically adjust terrain detail based on camera distance.
3. Frustum culling: Implement frustum culling to avoid rendering terrain sections outside the camera's view.
4. Multithreading: Implement CPU multithreading for tasks that cannot be GPU-accelerated.
