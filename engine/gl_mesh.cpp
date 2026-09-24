#include "gl_mesh.h"

void GLMesh::upload(const float* vertices, int vertexCount,
    const uint32_t* indices, int indexCount_) {
    gl.glGenVertexArrays(1, &vao);
    gl.glBindVertexArray(vao);

    gl.glGenBuffers(1, &vbo);
    gl.glBindBuffer(GL_ARRAY_BUFFER, vbo);
    gl.glBufferData(GL_ARRAY_BUFFER, vertexCount * 8 * sizeof(float), vertices, GL_STATIC_DRAW);

    gl.glGenBuffers(1, &ibo);
    gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    gl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount_ * sizeof(uint32_t), indices, GL_STATIC_DRAW);

    const GLsizei stride = 8 * sizeof(float);
    // position
    gl.glEnableVertexAttribArray(0);
    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    // normal
    gl.glEnableVertexAttribArray(1);
    gl.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    // uv
    gl.glEnableVertexAttribArray(2);
    gl.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

    gl.glBindVertexArray(0);
    indexCount = indexCount_;
}

void GLMesh::draw() const {
    if (!vao || indexCount == 0) return;
    gl.glBindVertexArray(vao);
    gl.glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void GLMesh::destroy() {
    if (vao) { gl.glDeleteVertexArrays(1, &vao); vao = 0; }
    if (vbo) { gl.glDeleteBuffers(1, &vbo); vbo = 0; }
    if (ibo) { gl.glDeleteBuffers(1, &ibo); ibo = 0; }
    indexCount = 0;
}