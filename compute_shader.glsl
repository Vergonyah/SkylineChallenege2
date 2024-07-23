#version 430

layout(local_size_x = 256) in;

layout(std430, binding = 0) buffer VertexBuffer {
    vec4 vertices[];
};

layout(std430, binding = 1) buffer IndexBuffer {
    uint indices[];
};

layout(std430, binding = 2) buffer NormalBuffer {
    vec4 normals[];
};

uniform int gridSize;

void main() {
    uint gid = gl_GlobalInvocationID.x;
    if (gid >= gridSize * gridSize) return;

    vec3 normal = vec3(0.0, 0.0, 0.0);

    // Calculate normals for surrounding faces
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;

            int x = int(gid % gridSize) + i;
            int z = int(gid / gridSize) + j;

            if (x < 0 || x >= gridSize - 1 || z < 0 || z >= gridSize - 1) continue;

            uint index = z * gridSize + x;
            
            vec3 v1 = vertices[indices[index * 6]].xyz;
            vec3 v2 = vertices[indices[index * 6 + 1]].xyz;
            vec3 v3 = vertices[indices[index * 6 + 2]].xyz;

            vec3 edge1 = v2 - v1;
            vec3 edge2 = v3 - v1;
            normal += normalize(cross(edge1, edge2));
        }
    }

    normals[gid] = vec4(normalize(normal), 0.0);
}