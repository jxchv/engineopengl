// ======================== Minimal OpenGL Engine (no external deps) ========================

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <cstdint>
#include <cmath>
#include <chrono>
#include <vector>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

#include "gl_loader.h"
#include "gl_context.h"
#include "gl_shader.h"
#include "gl_mesh.h"
#include "shaders.h"

// ============================== Простая математика ==============================

struct Vec3 {
    float x = 0, y = 0, z = 0;
    Vec3() {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 cross(const Vec3& v) const {
        return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    float length() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3 normalized() const {
        float l = length();
        if (l < 1e-8f) return Vec3();
        return Vec3(x / l, y / l, z / l);
    }
};

// ============================== Матрица 4x4 (column-major) ==============================

struct Mat4 {
    float m[4][4];   // m[column][row]

    Mat4() { identity(); }

    void identity() {
        for (int c = 0; c < 4; ++c)
            for (int r = 0; r < 4; ++r)
                m[c][r] = (c == r) ? 1.0f : 0.0f;
    }

    static Mat4 zero() {
        Mat4 r;
        for (int c = 0; c < 4; ++c)
            for (int rr = 0; rr < 4; ++rr)
                r.m[c][rr] = 0.0f;
        return r;
    }

    Mat4 operator*(const Mat4& o) const {
        Mat4 r = Mat4::zero();
        for (int c = 0; c < 4; ++c)
            for (int rr = 0; rr < 4; ++rr)
                for (int k = 0; k < 4; ++k)
                    r.m[c][rr] += m[k][rr] * o.m[c][k];
        return r;
    }

    static Mat4 translate(const Vec3& t) {
        Mat4 r;
        r.m[3][0] = t.x;
        r.m[3][1] = t.y;
        r.m[3][2] = t.z;
        return r;
    }

    static Mat4 scale(const Vec3& s) {
        Mat4 r;
        r.m[0][0] = s.x;
        r.m[1][1] = s.y;
        r.m[2][2] = s.z;
        return r;
    }

    static Mat4 rotateY(float a) {
        Mat4 r;
        float c = std::cos(a), s = std::sin(a);
        r.m[0][0] = c;  r.m[2][0] = s;
        r.m[0][2] = -s; r.m[2][2] = c;
        return r;
    }
    static Mat4 rotateX(float a) {
        Mat4 r;
        float c = std::cos(a), s = std::sin(a);
        r.m[1][1] = c;  r.m[2][1] = -s;
        r.m[1][2] = s;  r.m[2][2] = c;
        return r;
    }

    static Mat4 perspective(float fovY, float aspect, float zNear, float zFar) {
        Mat4 r = Mat4::zero();
        float f = 1.0f / std::tan(fovY * 0.5f);
        r.m[0][0] = f / aspect;
        r.m[1][1] = f;
        r.m[2][2] = (zFar + zNear) / (zNear - zFar);
        r.m[2][3] = -1.0f;
        r.m[3][2] = (2.0f * zFar * zNear) / (zNear - zFar);
        return r;
    }

    static Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
        Vec3 f = (center - eye).normalized();
        Vec3 s = f.cross(up).normalized();
        Vec3 u = s.cross(f);
        Mat4 r;
        r.m[0][0] = s.x;  r.m[1][0] = s.y;  r.m[2][0] = s.z;
        r.m[0][1] = u.x;  r.m[1][1] = u.y;  r.m[2][1] = u.z;
        r.m[0][2] = -f.x; r.m[1][2] = -f.y; r.m[2][2] = -f.z;
        r.m[3][0] = -s.dot(eye);
        r.m[3][1] = -u.dot(eye);
        r.m[3][2] = f.dot(eye);
        return r;
    }
};

// ============================== Генератор куба ==============================

static void buildCube(std::vector<float>& outVertices, std::vector<uint32_t>& outIndices) {
    // Формат: pos(3) + normal(3) + uv(2) = 8 float на вершину
    struct Face { Vec3 n; Vec3 v[4]; };
    Face faces[6] = {
        { Vec3(0,0,1),  { Vec3(-1,-1, 1), Vec3(1,-1, 1), Vec3(1, 1, 1), Vec3(-1, 1, 1) } },
        { Vec3(0,0,-1), { Vec3(1,-1,-1), Vec3(-1,-1,-1), Vec3(-1, 1,-1), Vec3(1, 1,-1) } },
        { Vec3(1,0,0),  { Vec3(1,-1, 1), Vec3(1,-1,-1), Vec3(1, 1,-1), Vec3(1, 1, 1) } },
        { Vec3(-1,0,0), { Vec3(-1,-1,-1), Vec3(-1,-1, 1), Vec3(-1, 1, 1), Vec3(-1, 1,-1) } },
        { Vec3(0,1,0),  { Vec3(-1, 1, 1), Vec3(1, 1, 1), Vec3(1, 1,-1), Vec3(-1, 1,-1) } },
        { Vec3(0,-1,0), { Vec3(-1,-1,-1), Vec3(1,-1,-1), Vec3(1,-1, 1), Vec3(-1,-1, 1) } },
    };
    float uvs[4][2] = { {0,1}, {1,1}, {1,0}, {0,0} };

    outVertices.clear();
    outIndices.clear();

    for (int f = 0; f < 6; ++f) {
        uint32_t base = (uint32_t)(outVertices.size() / 8);
        for (int i = 0; i < 4; ++i) {
            outVertices.push_back(faces[f].v[i].x);
            outVertices.push_back(faces[f].v[i].y);
            outVertices.push_back(faces[f].v[i].z);
            outVertices.push_back(faces[f].n.x);
            outVertices.push_back(faces[f].n.y);
            outVertices.push_back(faces[f].n.z);
            outVertices.push_back(uvs[i][0]);
            outVertices.push_back(uvs[i][1]);
        }
        outIndices.push_back(base + 0);
        outIndices.push_back(base + 1);
        outIndices.push_back(base + 2);
        outIndices.push_back(base + 0);
        outIndices.push_back(base + 2);
        outIndices.push_back(base + 3);
    }
}

// ============================== Окно ==============================

static bool g_running = true;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        g_running = false;
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) g_running = false;
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE,
    _In_ LPSTR, _In_ int nCmdShow)
{
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MinimalOpenGL";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassW(&wc);

    const int winW = 800;
    const int winH = 600;

    RECT rc = { 0, 0, winW, winH };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hwnd = CreateWindowExW(
        0, L"MinimalOpenGL", L"Minimal OpenGL Engine",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        nullptr, nullptr, hInstance, nullptr);
    if (!hwnd) return 1;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    SetForegroundWindow(hwnd);

    if (!createGLContext(hwnd)) {
        MessageBoxW(hwnd, L"Failed to create OpenGL 3.3 context", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    gl.glEnable(GL_DEPTH_TEST);
    gl.glDepthFunc(GL_LESS);
    gl.glEnable(GL_CULL_FACE);
    gl.glCullFace(GL_BACK);
    gl.glFrontFace(GL_CCW);

    GLuint program = createShaderProgram(SHADER_BASIC_VS, SHADER_BASIC_FS);
    if (!program) {
        MessageBoxW(hwnd, L"Failed to create shader program", L"Error", MB_OK | MB_ICONERROR);
        destroyGLContext(hwnd);
        return 1;
    }

    // Куб
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    buildCube(vertices, indices);

    GLMesh cube;
    cube.upload(vertices.data(), (int)(vertices.size() / 8),
        indices.data(), (int)indices.size());

    // Uniform locations
    GLint uModelLoc = gl.glGetUniformLocation(program, "uModel");
    GLint uViewProjLoc = gl.glGetUniformLocation(program, "uViewProj");
    GLint uAmbientLoc = gl.glGetUniformLocation(program, "uAmbient");
    GLint uLightDirLoc = gl.glGetUniformLocation(program, "uDirLightDir");
    GLint uLightColLoc = gl.glGetUniformLocation(program, "uDirLightColor");
    GLint uLightIntLoc = gl.glGetUniformLocation(program, "uDirLightIntensity");
    GLint uUseTexLoc = gl.glGetUniformLocation(program, "uUseTexture");
    GLint uFlatColorLoc = gl.glGetUniformLocation(program, "uFlatColor");

    auto lastTime = std::chrono::high_resolution_clock::now();
    float totalTime = 0.0f;

    MSG msg = {};

    while (g_running) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) { g_running = false; break; }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (!g_running) break;

        auto now = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(now - lastTime).count();
        lastTime = now;
        totalTime += dt;

        gl.glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RECT clientRc;
        GetClientRect(hwnd, &clientRc);
        int cw = clientRc.right - clientRc.left;
        int ch = clientRc.bottom - clientRc.top;
        if (cw > 0 && ch > 0) {
            gl.glViewport(0, 0, cw, ch);
        }

        // Камера
        Vec3 eye(3.0f * std::sin(totalTime * 0.5f),
            2.0f,
            3.0f * std::cos(totalTime * 0.5f));
        Vec3 target(0, 0, 0);
        Vec3 up(0, 1, 0);

        Mat4 view = Mat4::lookAt(eye, target, up);
        Mat4 proj = Mat4::perspective(60.0f * 3.14159f / 180.0f,
            (float)cw / (float)ch, 0.1f, 100.0f);
        Mat4 viewProj = proj * view;

        Mat4 model = Mat4::rotateY(totalTime * 0.8f) * Mat4::rotateX(totalTime * 0.4f);

        gl.glUseProgram(program);

        gl.glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, &model.m[0][0]);
        gl.glUniformMatrix4fv(uViewProjLoc, 1, GL_FALSE, &viewProj.m[0][0]);

        gl.glUniform3f(uAmbientLoc, 0.15f, 0.15f, 0.2f);
        gl.glUniform3f(uLightDirLoc, -0.4f, -0.7f, -0.5f);
        gl.glUniform3f(uLightColLoc, 1.0f, 0.95f, 0.85f);
        gl.glUniform1f(uLightIntLoc, 1.0f);

        gl.glUniform1i(uUseTexLoc, 0);
        gl.glUniform3f(uFlatColorLoc, 0.8f, 0.7f, 0.5f);

        cube.draw();

        swapBuffers();
    }

    cube.destroy();
    gl.glDeleteProgram(program);
    destroyGLContext(hwnd);

    return 0;
}