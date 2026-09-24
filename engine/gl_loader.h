#pragma once

#include <windows.h>
#include <cstdint>

// ============================== OpenGL types ==============================

typedef unsigned int   GLenum;
typedef unsigned char  GLboolean;
typedef unsigned int   GLbitfield;
typedef signed char    GLbyte;
typedef short          GLshort;
typedef int            GLint;
typedef int            GLsizei;
typedef unsigned char  GLubyte;
typedef unsigned short GLushort;
typedef unsigned int   GLuint;
typedef float          GLfloat;
typedef float          GLclampf;
typedef double         GLdouble;
typedef void           GLvoid;
typedef char           GLchar;
typedef ptrdiff_t      GLintptr;
typedef ptrdiff_t      GLsizeiptr;

// ============================== OpenGL constants ==============================

#define GL_COLOR_BUFFER_BIT     0x00004000
#define GL_DEPTH_BUFFER_BIT     0x00000100
#define GL_TRIANGLES            0x0004
#define GL_UNSIGNED_INT         0x1405
#define GL_UNSIGNED_BYTE        0x1401
#define GL_FLOAT                0x1406
#define GL_FALSE                0
#define GL_TRUE                 1

#define GL_ARRAY_BUFFER         0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW          0x88E4
#define GL_DYNAMIC_DRAW         0x88E8

#define GL_VERTEX_SHADER        0x8B31
#define GL_FRAGMENT_SHADER      0x8B30
#define GL_COMPILE_STATUS       0x8B81
#define GL_LINK_STATUS          0x8B82
#define GL_INFO_LOG_LENGTH      0x8B84

#define GL_TEXTURE_2D           0x0DE1
#define GL_TEXTURE0             0x84C0
#define GL_RGBA                 0x1908
#define GL_RGBA8                0x8058
#define GL_RGB                  0x1907
#define GL_TEXTURE_MIN_FILTER   0x2801
#define GL_TEXTURE_MAG_FILTER   0x2800
#define GL_TEXTURE_WRAP_S       0x2802
#define GL_TEXTURE_WRAP_T       0x2803
#define GL_LINEAR               0x2601
#define GL_NEAREST              0x2600
#define GL_REPEAT               0x2901
#define GL_CLAMP_TO_EDGE        0x812F
#define GL_TEXTURE1             0x84C1

#define GL_DEPTH_TEST           0x0B71
#define GL_CULL_FACE            0x0B44
#define GL_BACK                 0x0405
#define GL_FRONT                0x0404
#define GL_CCW                  0x0901
#define GL_CW                   0x0900
#define GL_BLEND                0x0BE2
#define GL_SRC_ALPHA            0x0302
#define GL_ONE_MINUS_SRC_ALPHA  0x0303
#define GL_LESS                 0x0201
#define GL_LEQUAL               0x0203

// WGL
#define WGL_DRAW_TO_WINDOW_ARB          0x2001
#define WGL_SUPPORT_OPENGL_ARB          0x2010
#define WGL_DOUBLE_BUFFER_ARB           0x2011
#define WGL_PIXEL_TYPE_ARB              0x2013
#define WGL_TYPE_RGBA_ARB               0x202B
#define WGL_COLOR_BITS_ARB              0x2014
#define WGL_DEPTH_BITS_ARB              0x2022
#define WGL_STENCIL_BITS_ARB            0x2023
#define WGL_SAMPLE_BUFFERS_ARB          0x2041
#define WGL_SAMPLES_ARB                 0x2042
#define WGL_CONTEXT_MAJOR_VERSION_ARB   0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB   0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB    0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001

// ============================== Function pointer types ==============================

typedef void (APIENTRY* PFN_glClear)(GLbitfield);
typedef void (APIENTRY* PFN_glClearColor)(GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (APIENTRY* PFN_glViewport)(GLint, GLint, GLsizei, GLsizei);
typedef void (APIENTRY* PFN_glEnable)(GLenum);
typedef void (APIENTRY* PFN_glDisable)(GLenum);
typedef void (APIENTRY* PFN_glDepthFunc)(GLenum);
typedef void (APIENTRY* PFN_glCullFace)(GLenum);
typedef void (APIENTRY* PFN_glFrontFace)(GLenum);
typedef void (APIENTRY* PFN_glBlendFunc)(GLenum, GLenum);

typedef void (APIENTRY* PFN_glGenBuffers)(GLsizei, GLuint*);
typedef void (APIENTRY* PFN_glBindBuffer)(GLenum, GLuint);
typedef void (APIENTRY* PFN_glBufferData)(GLenum, GLsizeiptr, const void*, GLenum);
typedef void (APIENTRY* PFN_glDeleteBuffers)(GLsizei, const GLuint*);

typedef void (APIENTRY* PFN_glGenVertexArrays)(GLsizei, GLuint*);
typedef void (APIENTRY* PFN_glBindVertexArray)(GLuint);
typedef void (APIENTRY* PFN_glDeleteVertexArrays)(GLsizei, const GLuint*);
typedef void (APIENTRY* PFN_glEnableVertexAttribArray)(GLuint);
typedef void (APIENTRY* PFN_glVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);

typedef GLuint(APIENTRY* PFN_glCreateShader)(GLenum);
typedef void (APIENTRY* PFN_glShaderSource)(GLuint, GLsizei, const GLchar**, const GLint*);
typedef void (APIENTRY* PFN_glCompileShader)(GLuint);
typedef void (APIENTRY* PFN_glGetShaderiv)(GLuint, GLenum, GLint*);
typedef void (APIENTRY* PFN_glGetShaderInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef void (APIENTRY* PFN_glDeleteShader)(GLuint);

typedef GLuint(APIENTRY* PFN_glCreateProgram)(void);
typedef void (APIENTRY* PFN_glAttachShader)(GLuint, GLuint);
typedef void (APIENTRY* PFN_glLinkProgram)(GLuint);
typedef void (APIENTRY* PFN_glGetProgramiv)(GLuint, GLenum, GLint*);
typedef void (APIENTRY* PFN_glGetProgramInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef void (APIENTRY* PFN_glUseProgram)(GLuint);
typedef void (APIENTRY* PFN_glDeleteProgram)(GLuint);
typedef GLint(APIENTRY* PFN_glGetUniformLocation)(GLuint, const GLchar*);
typedef void (APIENTRY* PFN_glUniform1i)(GLint, GLint);
typedef void (APIENTRY* PFN_glUniform1f)(GLint, GLfloat);
typedef void (APIENTRY* PFN_glUniform3f)(GLint, GLfloat, GLfloat, GLfloat);
typedef void (APIENTRY* PFN_glUniform3fv)(GLint, GLsizei, const GLfloat*);
typedef void (APIENTRY* PFN_glUniformMatrix4fv)(GLint, GLsizei, GLboolean, const GLfloat*);

typedef void (APIENTRY* PFN_glGenTextures)(GLsizei, GLuint*);
typedef void (APIENTRY* PFN_glBindTexture)(GLenum, GLuint);
typedef void (APIENTRY* PFN_glTexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void*);
typedef void (APIENTRY* PFN_glTexParameteri)(GLenum, GLenum, GLint);
typedef void (APIENTRY* PFN_glActiveTexture)(GLenum);
typedef void (APIENTRY* PFN_glDeleteTextures)(GLsizei, const GLuint*);
typedef void (APIENTRY* PFN_glGenerateMipmap)(GLenum);

typedef void (APIENTRY* PFN_glDrawElements)(GLenum, GLsizei, GLenum, const void*);
typedef void (APIENTRY* PFN_glDrawArrays)(GLenum, GLint, GLsizei);

// ============================== Loader ==============================

struct GLLoader {
    PFN_glClear glClear = nullptr;
    PFN_glClearColor glClearColor = nullptr;
    PFN_glViewport glViewport = nullptr;
    PFN_glEnable glEnable = nullptr;
    PFN_glDisable glDisable = nullptr;
    PFN_glDepthFunc glDepthFunc = nullptr;
    PFN_glCullFace glCullFace = nullptr;
    PFN_glFrontFace glFrontFace = nullptr;
    PFN_glBlendFunc glBlendFunc = nullptr;

    PFN_glGenBuffers glGenBuffers = nullptr;
    PFN_glBindBuffer glBindBuffer = nullptr;
    PFN_glBufferData glBufferData = nullptr;
    PFN_glDeleteBuffers glDeleteBuffers = nullptr;

    PFN_glGenVertexArrays glGenVertexArrays = nullptr;
    PFN_glBindVertexArray glBindVertexArray = nullptr;
    PFN_glDeleteVertexArrays glDeleteVertexArrays = nullptr;
    PFN_glEnableVertexAttribArray glEnableVertexAttribArray = nullptr;
    PFN_glVertexAttribPointer glVertexAttribPointer = nullptr;

    PFN_glCreateShader glCreateShader = nullptr;
    PFN_glShaderSource glShaderSource = nullptr;
    PFN_glCompileShader glCompileShader = nullptr;
    PFN_glGetShaderiv glGetShaderiv = nullptr;
    PFN_glGetShaderInfoLog glGetShaderInfoLog = nullptr;
    PFN_glDeleteShader glDeleteShader = nullptr;

    PFN_glCreateProgram glCreateProgram = nullptr;
    PFN_glAttachShader glAttachShader = nullptr;
    PFN_glLinkProgram glLinkProgram = nullptr;
    PFN_glGetProgramiv glGetProgramiv = nullptr;
    PFN_glGetProgramInfoLog glGetProgramInfoLog = nullptr;
    PFN_glUseProgram glUseProgram = nullptr;
    PFN_glDeleteProgram glDeleteProgram = nullptr;
    PFN_glGetUniformLocation glGetUniformLocation = nullptr;
    PFN_glUniform1i glUniform1i = nullptr;
    PFN_glUniform1f glUniform1f = nullptr;
    PFN_glUniform3f glUniform3f = nullptr;
    PFN_glUniform3fv glUniform3fv = nullptr;
    PFN_glUniformMatrix4fv glUniformMatrix4fv = nullptr;

    PFN_glGenTextures glGenTextures = nullptr;
    PFN_glBindTexture glBindTexture = nullptr;
    PFN_glTexImage2D glTexImage2D = nullptr;
    PFN_glTexParameteri glTexParameteri = nullptr;
    PFN_glActiveTexture glActiveTexture = nullptr;
    PFN_glDeleteTextures glDeleteTextures = nullptr;
    PFN_glGenerateMipmap glGenerateMipmap = nullptr;

    PFN_glDrawElements glDrawElements = nullptr;
    PFN_glDrawArrays glDrawArrays = nullptr;

    bool loadAll();
};

extern GLLoader gl;