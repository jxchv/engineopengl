#pragma once
#include "gl_loader.h"

// Компилирует и линкует пару шейдеров. Возвращает ID программы, 0 при ошибке.
GLuint createShaderProgram(const char* vertexSrc, const char* fragmentSrc);