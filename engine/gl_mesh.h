#pragma once
#include "gl_loader.h"
#include <cstdint>
#include <vector>

// Формат вершины: pos(3) + normal(3) + uv(2) = 8 float
struct GLMesh {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ibo = 0;
    int indexCount = 0;

    // Загрузка из interleaved-массива: pos, normal, uv (8 float на вершину).
    void upload(const float* vertices, int vertexCount,
        const uint32_t* indices, int indexCount_);

    // Рисование.
    void draw() const;

    // Освобождение.
    void destroy();
};