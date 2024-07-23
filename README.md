# SkylineChallenege2
Second challenge for Skyline Nav AI.

## Features
- Heightmap-based terrain generation.
- Camera movement and rotation via mouse and keyboard (WASD keys)
- Dynamic lighting system
- Wireframe toggle mode (T key)
- Normal vector visualization (P key)
- Light placement (L key)
- Performance testing mode

## Dependencies 
For you to build and run the main code, you need the following dependencies:
- GLEW
- GLFW 
- GLM
- stb_image

## Building
To build the project, do the following: 
1. Have all dependencies ready.
2. Open a terminal in the project directory. 
3. Run the following command: 'make'
4. Once terrain_renderer has been made, run it by typing './terrain_renderer <heightmap_path> [--performance]
- '<heightmap_path>': Path to the heightmap image file to be used.
- '--performance': Optional flag to have it start in performance mode.

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

- Will fill out later.


## Potential bottlenecks

1. Memory transfer: Although we've minimized data transfer between CPU and GPU, there might still be some overhead in updating dynamic terrain data.
2. Texture sampling: For large terrains, cache misses during height map texture sampling could impact performance.
3. Draw calls: A high number of draw calls for complex terrains could limit performance.

### Further Optimization Opportunities

1. Level of Detail (LOD): Implement a LOD system to reduce the number of triangles rendered for distant terrain parts.
2. Tessellation: Use tessellation shaders to dynamically adjust terrain detail based on camera distance.
3. Frustum culling: Implement frustum culling to avoid rendering terrain sections outside the camera's view.
4. Multithreading: Implement CPU multithreading for tasks that cannot be GPU-accelerated.
