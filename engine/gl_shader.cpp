#include "gl_shader.h"
#include <windows.h>

static GLuint compileShader(GLenum type, const char* src) {
    GLuint s = gl.glCreateShader(type);
    if (!s) return 0;
    gl.glShaderSource(s, 1, &src, nullptr);
    gl.glCompileShader(s);

    GLint ok = 0;
    gl.glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[2048];
        GLsizei len = 0;
        gl.glGetShaderInfoLog(s, sizeof(log), &len, log);
        OutputDebugStringA("=== Shader compile error ===\n");
        OutputDebugStringA(src);
        OutputDebugStringA("\n--- log ---\n");
        OutputDebugStringA(log);
        OutputDebugStringA("\n");
        gl.glDeleteShader(s);
        return 0;
    }
    return s;
}

GLuint createShaderProgram(const char* vertexSrc, const char* fragmentSrc) {
    GLuint v = compileShader(GL_VERTEX_SHADER, vertexSrc);
    GLuint f = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    if (!v || !f) return 0;

    GLuint p = gl.glCreateProgram();
    gl.glAttachShader(p, v);
    gl.glAttachShader(p, f);
    gl.glLinkProgram(p);

    GLint ok = 0;
    gl.glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[2048];
        GLsizei len = 0;
        gl.glGetProgramInfoLog(p, sizeof(log), &len, log);
        OutputDebugStringA("=== Program link error ===\n");
        OutputDebugStringA(log);
        OutputDebugStringA("\n");
        gl.glDeleteProgram(p);
        return 0;
    }
    gl.glDeleteShader(v);
    gl.glDeleteShader(f);
    return p;
}