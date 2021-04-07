#ifndef RENDER_PLATFORM
#define RENDER_PLATFORM
/* Contains Functions To Draw Graphics */
#include <iostream>
#include <Windows.h>
#include <string.h>
#include <math.h>
#include <algorithm>

#include "Math.cpp"

namespace Croipe::RenderPlatform {
    /* Globals */
    static HDC bitmapHandle;
    static unsigned int *bitmap;
    static HWND window;
    static HDC dc;

    static int width;
    static int height;

    /* Internal Functions */
    void CreateDIB(HWND windowHandle) {
        int DIBrowByteWidth;
        HBITMAP m_hDIBBitmap;
        BITMAPINFO mybmi;
        RECT mainClientRect;
        int bitCount;
        int totalBytes;
        
        HDC hdc = GetDC(windowHandle);
        GetClientRect(windowHandle, &mainClientRect);
        width = mainClientRect.right;
        height = mainClientRect.bottom;
        bitCount = 32;
        DIBrowByteWidth = ((width * (bitCount / 8) + 3) & -4);
        totalBytes = DIBrowByteWidth * height;

        mybmi.bmiHeader.biSize = sizeof(mybmi);
        mybmi.bmiHeader.biWidth = width;
        mybmi.bmiHeader.biHeight = -height;
        mybmi.bmiHeader.biPlanes = 1;
        mybmi.bmiHeader.biBitCount = (WORD)bitCount;
        mybmi.bmiHeader.biCompression = BI_RGB;
        mybmi.bmiHeader.biSizeImage = totalBytes;
        mybmi.bmiHeader.biXPelsPerMeter = 0;
        mybmi.bmiHeader.biYPelsPerMeter = 0;

        bitmapHandle = CreateCompatibleDC(hdc);
        m_hDIBBitmap = CreateDIBSection(hdc, &mybmi, DIB_RGB_COLORS, (VOID **)&bitmap, NULL, 0);
        SelectObject(bitmapHandle, m_hDIBBitmap);
        ReleaseDC(windowHandle, hdc);
    }

    /* Screen Functions */
    void CreateScreen(const std::string title, const std::vector<std::string> loadingMessages) {
        /* Display Loading Messages */
        std::cout << "Made With CroipeEngine!" << std::endl;
        for (auto message : loadingMessages) {
            std::cout << message << std::endl;
        }

        /* Set Title, Delay, Then Clear Screen*/
        SetConsoleTitleA(title.c_str());
        Sleep(3000);
        system("cls");

        /* Grab Window */
        window = GetConsoleWindow();
        dc = GetDC(window);
        CreateDIB(window);
    }

    void Dispose() {
        ReleaseDC(window, dc);
    }

    void ClearScreen() {
        memset(bitmap, 0, width * height * 4);
    }

    void UpdateScreen() {
        BitBlt(dc, 0, 0, width, height, bitmapHandle, 0, 0, SRCCOPY);
    }

    /* Util Functions */
    std::vector<float> Interpolate(float i1, float d1, float i2, float d2) {
        std::vector<float> values;

        if (i1 == i2) {
            values.push_back(d1);
            return values;
        }

        float a = (d2 - d1) / (i2 - i1);
        float d = d1;

        for (float i = i1; i < i2; i++) {
            values.push_back(d);
            d += a;
        }

        return values;
    }

    /* Drawing Functions */
    void DrawPixel(int x, int y, int r, int g, int b) {
        bitmap[y * width + x] = (r << 16) + (g << 8) + (b);
    }

    void DrawLine(float x1, float y1, float x2, float y2, int r, int g, int b) {
        float dx = x2 - x1;
        float dy = y2 - y1;

        /* Horizontal-ish Line */
        if (abs(dx) > abs(dy)) {
            if (x1 > x2) {
                /* Swap */
                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            std::vector<float> ys = Interpolate(x1, y1, x2, y2);

            for (float x = x1; x < x2; x++)  {
                /* Basic Clipping */
                if (x > 0.0f && x < width && ys[(int)(x - x1)] > 0.0f && ys[(int)(x - x1)] < height-1) 
                    DrawPixel((int)x, (int)ys[(int)(x - x1)], r, g, b);
            }
        }

        /* Vertical-ish Line */
        else {
            if (y1 > y2) {
                /* Swap */
                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            std::vector<float> xs = Interpolate(y1, x1, y2, x2);

            for (float y = y1; y < y2; y++)  {
                /* Basic Clipping */
                if (xs[(int)(y-y1)] > 0.0f && xs[(int)(y-y1)] < width && y > 0.0f && y < height-1) 
                    DrawPixel((int)xs[(int)(y - y1)], (int)y, r, g, b);
            }
        }
    }

    void DrawTriangle(Croipe::Math::Triangle &triangle, int r, int g, int b) {
        /* Triangle Is Made Of Three Lines */
        DrawLine(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, r, g, b);
        DrawLine(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, r, g, b);
        DrawLine(triangle.points[2].x, triangle.points[2].y, triangle.points[0].x, triangle.points[0].y, r, g, b);
    }

    void RenderWireframe(Croipe::Math::Wireframe &wireframe, int r, int g, int b) {
        /* Rotate, Translate, Project and Render Triangles */
        for (auto &triangle : wireframe.mesh.triangles) {
            Croipe::Math::Triangle rotatedTriangle = Croipe::Math::RotateTriangle(triangle, wireframe.rotation);
            Croipe::Math::Triangle translatedTriangle = Croipe::Math::TranslateTriangle(rotatedTriangle, wireframe.translation);
            Croipe::Math::Vector3 normal = Croipe::Math::CalculateNormal(translatedTriangle);

            if (normal.x * (translatedTriangle.points[0].x - wireframe.camera.x) + 
                normal.y * (translatedTriangle.points[0].y - wireframe.camera.y) +
                normal.z * (translatedTriangle.points[0].z - wireframe.camera.z) < 0.0f) {
                    Croipe::Math::Triangle clippedTriangle = Croipe::Math::ClipTriangle(translatedTriangle);
                    Croipe::Math::Triangle projectedTriangle = Croipe::Math::ProjectTriangle(clippedTriangle, width, height, 0.1f, 1000.0f, 90.0f);
                    Croipe::RenderPlatform::DrawTriangle(projectedTriangle, r, g, b);
            }
        }
    }
}
#endif