#include "gl_loader.h"

GLLoader gl;

static void* getGLProc(const char* name) {
    void* p = (void*)wglGetProcAddress(name);
    if (p == nullptr || p == (void*)0x1 || p == (void*)0x2 || p == (void*)0x3 || p == (void*)-1) {
        static HMODULE glModule = LoadLibraryA("opengl32.dll");
        if (glModule) p = (void*)GetProcAddress(glModule, name);
    }
    return p;
}

bool GLLoader::loadAll() {
#define LOAD(name) name = (decltype(name))getGLProc(#name); if (!name) { OutputDebugStringA("Failed to load: " #name "\n"); return false; }
    LOAD(glClear)
        LOAD(glClearColor)
        LOAD(glViewport)
        LOAD(glEnable)
        LOAD(glDisable)
        LOAD(glDepthFunc)
        LOAD(glCullFace)
        LOAD(glFrontFace)
        LOAD(glBlendFunc)
        LOAD(glGenBuffers)
        LOAD(glBindBuffer)
        LOAD(glBufferData)
        LOAD(glDeleteBuffers)
        LOAD(glGenVertexArrays)
        LOAD(glBindVertexArray)
        LOAD(glDeleteVertexArrays)
        LOAD(glEnableVertexAttribArray)
        LOAD(glVertexAttribPointer)
        LOAD(glCreateShader)
        LOAD(glShaderSource)
        LOAD(glCompileShader)
        LOAD(glGetShaderiv)
        LOAD(glGetShaderInfoLog)
        LOAD(glDeleteShader)
        LOAD(glCreateProgram)
        LOAD(glAttachShader)
        LOAD(glLinkProgram)
        LOAD(glGetProgramiv)
        LOAD(glGetProgramInfoLog)
        LOAD(glUseProgram)
        LOAD(glDeleteProgram)
        LOAD(glGetUniformLocation)
        LOAD(glUniform1i)
        LOAD(glUniform1f)
        LOAD(glUniform3f)
        LOAD(glUniform3fv)
        LOAD(glUniformMatrix4fv)
        LOAD(glGenTextures)
        LOAD(glBindTexture)
        LOAD(glTexImage2D)
        LOAD(glTexParameteri)
        LOAD(glActiveTexture)
        LOAD(glDeleteTextures)
        LOAD(glGenerateMipmap)
        LOAD(glDrawElements)
        LOAD(glDrawArrays)
#undef LOAD
        return true;
}