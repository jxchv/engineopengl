#pragma once

#include <windows.h>

// Создаёт OpenGL 3.3 Core контекст для окна.
// Загружает все функции через gl.loadAll().
// Возвращает true при успехе.
bool createGLContext(HWND hwnd);

// Освобождает контекст и DC. Вызывать перед закрытием окна.
void destroyGLContext(HWND hwnd);

// SwapBuffers обёртка (использует сохранённый HDC)
void swapBuffers();