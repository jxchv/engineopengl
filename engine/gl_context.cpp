#include "gl_context.h"
#include "gl_loader.h"

typedef BOOL(WINAPI* PFNWGLCHOOSEPIXELFORMATARB)(HDC, const int*, const FLOAT*, UINT, int*, UINT*);
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARB)(HDC, HGLRC, const int*);

static HGLRC g_glContext = nullptr;
static HDC g_hdc = nullptr;

bool createGLContext(HWND hwnd) {
    g_hdc = GetDC(hwnd);

    // Шаг 1: временный контекст для получения WGL-функций
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int format = ChoosePixelFormat(g_hdc, &pfd);
    if (format == 0) return false;
    if (!SetPixelFormat(g_hdc, format, &pfd)) return false;

    HGLRC tempCtx = wglCreateContext(g_hdc);
    if (!tempCtx) return false;
    wglMakeCurrent(g_hdc, tempCtx);

    PFNWGLCHOOSEPIXELFORMATARB wglChoosePixelFormatARB =
        (PFNWGLCHOOSEPIXELFORMATARB)wglGetProcAddress("wglChoosePixelFormatARB");
    PFNWGLCREATECONTEXTATTRIBSARB wglCreateContextAttribsARB =
        (PFNWGLCREATECONTEXTATTRIBSARB)wglGetProcAddress("wglCreateContextAttribsARB");

    if (!wglCreateContextAttribsARB) {
        OutputDebugStringA("wglCreateContextAttribsARB not available\n");
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(tempCtx);
        return false;
    }

    // Шаг 2: создаём настоящий контекст 3.3 Core
    const int ctxAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    g_glContext = wglCreateContextAttribsARB(g_hdc, nullptr, ctxAttribs);

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(tempCtx);

    if (!g_glContext) {
        OutputDebugStringA("Failed to create OpenGL 3.3 context\n");
        return false;
    }

    if (!wglMakeCurrent(g_hdc, g_glContext)) {
        OutputDebugStringA("wglMakeCurrent failed\n");
        return false;
    }

    // Шаг 3: загружаем функции OpenGL
    if (!gl.loadAll()) {
        OutputDebugStringA("Failed to load OpenGL functions\n");
        return false;
    }

    return true;
}

void destroyGLContext(HWND hwnd) {
    wglMakeCurrent(nullptr, nullptr);
    if (g_glContext) {
        wglDeleteContext(g_glContext);
        g_glContext = nullptr;
    }
    if (g_hdc) {
        ReleaseDC(hwnd, g_hdc);
        g_hdc = nullptr;
    }
}

void swapBuffers() {
    if (g_hdc) SwapBuffers(g_hdc);
}