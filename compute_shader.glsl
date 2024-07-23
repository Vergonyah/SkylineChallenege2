#version 430 core

layout(local_size_x = 16, local_size_y = 16) in;

layout(rgba32f, binding = 0) uniform image2D heightMap;
layout(rgba32f, binding = 1) writeonly uniform image2D normalMap;

uniform float terrainSize;
uniform int gridSize;

void main() {
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    
    if(pixelCoords.x >= gridSize || pixelCoords.y >= gridSize) {
        return;
    }

    float cellSize = terrainSize / float(gridSize - 1);

    vec3 pos = vec3(
        float(pixelCoords.x) * cellSize - terrainSize / 2.0,
        imageLoad(heightMap, pixelCoords).r,
        float(pixelCoords.y) * cellSize - terrainSize / 2.0
    );

    vec3 n = vec3(0.0, 1.0, 0.0);

    if(pixelCoords.x < gridSize - 1 && pixelCoords.y < gridSize - 1) {
        vec3 pos_dx = vec3(
            pos.x + cellSize,
            imageLoad(heightMap, pixelCoords + ivec2(1, 0)).r,
            pos.z
        );
        vec3 pos_dz = vec3(
            pos.x,
            imageLoad(heightMap, pixelCoords + ivec2(0, 1)).r,
            pos.z + cellSize
        );

        n = normalize(cross(pos_dx - pos, pos_dz - pos));
    }

    imageStore(normalMap, pixelCoords, vec4(n, 1.0));
}