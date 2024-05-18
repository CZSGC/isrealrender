#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <stdint.h>
#include <utility>
#include <algorithm>
#include "geometry.h"
#include "model.h"


constexpr int kTimerID = 101;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

struct graphics_buffer
{
    HBITMAP hbm;
    uint32_t* data;
};

graphics_buffer create_graphics_buffer(int wd, int hgt) 
{
    HDC hdcScreen = GetDC(NULL);

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = wd;
    bmi.bmiHeader.biHeight = hgt;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    graphics_buffer gb;
    gb.hbm = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, reinterpret_cast<void**>(&gb.data), NULL, NULL);

    ReleaseDC(NULL, hdcScreen);
    return gb;
}

class graphics_buffers
{
public:
    graphics_buffer front_;
    graphics_buffer back_;
    int wd_;
    int hgt_;
    graphics_buffers(int wd, int hgt) :wd_(wd), hgt_(hgt), front_(create_graphics_buffer(wd, hgt)), back_(create_graphics_buffer(wd, hgt)) {
        clear();
    }
    ~graphics_buffers() {
        DeleteObject(front_.hbm);
        DeleteObject(back_.hbm);
    }

    HBITMAP front_bmp() {
        return front_.hbm;
    }

    size_t size() const {
        return static_cast<size_t>(wd_ * hgt_);
    }

    int width() const {
        return wd_;
    }

    int height() const {
        return hgt_;
    }

    void clear() {
        std::fill(back_.data, back_.data + size(), 0);
    }

    void set_pixel(Vec3f v, uint32_t pix) {
        int x = v.x, y = v.y;
        if (x >= 0 && x < wd_ && y >= 0 && y < hgt_)
        {
            back_.data[(y)*wd_ + x] = pix;
        }
    }

    void set_line(Vec3f v0,Vec3f v1,uint32_t pex)
    {
        int x0 = v0.x, y0 = v0.y, x1 = v1.x, y1 = v1.y;
        bool steep = false;
        if (std::abs(y1 - y0) > std::abs(x1 - x0)) {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }

        int dx = x1 - x0;
        int dy = y1 - y0;
        int derror = 2 * std::abs(dy);
        int error = 0;
        int y = y0;

        for (int x = x0; x <= x1; x++) {
            if (steep) {
                set_pixel(Vec3f(y,x), 0xffffffff);
            }
            else {
                set_pixel(Vec3f(x,y), 0xffffffff);
            }
            error += derror;
            if (error > dx) {
                y += (y1 > y0) ? 1 : -1;
                error -= 2 * dx;
            }
        }

    }
    
    void set_triangle(Vec3f a, Vec3f b, Vec3f c) {
        int left, right, bottom, top;
        if (a.x < b.x)
            left = a.x < c.x ? a.x : c.x;
        else
            left = b.x < c.x ? b.x : c.x;


        if (a.y < b.y)
            bottom = a.y < c.y ? a.y : c.y;
        else
            bottom = b.y < c.y ? b.y : c.y;


        if (a.x > b.x)
            right = a.x > c.x ? a.x : c.x;
        else
            right = b.x > c.x ? b.x : c.x;


        if (a.y > b.y)
            top = a.y > c.y ? a.y : c.y;
        else
            top = b.y > c.y ? b.y : c.y;
        for (int i = left; i <= right; i++) {
            for (int j = bottom; j <= top; j++) {
                if (isInside(a, b, c, Vec3f(i, j))) {
                    set_pixel(Vec3f(i,j), 0xffffffff);
                }
            }
        }


    }



    void swap() {
        std::swap(front_, back_);
    }

};


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"isRealRender";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        wc.lpszClassName,                     // Window class
        L"isRealRender",    // Window text
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,            // Window style

        // Size and position
        0, 0, 800, 800,

        0,       // Parent window    
        0,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void draw_something(graphics_buffers& buffs) 
{
    int wd = buffs.width();
    int hgt = buffs.height();

    static int x = 0;
    static int y = 0;
    static int x_vel = 4;
    static int y_vel = 7;

    if (x >= 0 && x < wd && y >= 0 && y < hgt) 
    {
        buffs.set_pixel(Vec3f(x,y), 0xffffffff);
    }

    x += x_vel;
    y += y_vel;
    if (x<0 || x>wd) 
    {
        x_vel *= -1;
    }
    if (y<0 || y>hgt) 
    {
        y_vel *= -1;
    }
}

void drawModelFace(graphics_buffers& buffs) {
    Model model("E:/tinyrender/tinyrenderer/obj/african_head/african_head.obj");
    //model.faceVertexIndexVector.size()
    for (int i = 3; i < model.faceVertexIndexVector.size(); i++) {

        Vec3f v = model.faceVertexIndexVector[i];
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = model.vertexVector[v[j]];
            Vec3f v1 = model.vertexVector[v[(j + 1) % 3]];
            float x0 = (v0.x + 1.) * buffs.width() / 2.;
            float y0 = (v0.y + 1.) * buffs.height() / 2.;
            float x1 = (v1.x + 1.) * buffs.width() / 2.;
            float y1 = (v1.y + 1.) * buffs.height() / 2.;
            buffs.set_line(Vec3f(x0, y0), Vec3f(x1, y1), 0xffffffff);
        }
    }
}




LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE: 
        {
            RECT r;
            GetClientRect(hwnd, &r);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(new graphics_buffers(r.right - r.left, r.bottom - r.top)));
            SetTimer(hwnd, kTimerID, 1, NULL);
        }
        break;
        case WM_TIMER: 
        {
            auto buffs = reinterpret_cast<graphics_buffers*>(GetWindowLongPtr(hwnd, GWLP_USERDATA ));
            //draw_something(*buffs);
            //buffs->set_triangle(Vec3f(50,70), Vec3f(100,300), Vec3f(300,200));
            drawModelFace(*buffs);
            buffs->swap();
            buffs->clear();
            InvalidateRect(hwnd, NULL, FALSE);

        }
        break;
        case WM_PAINT: 
        {
            auto buffs = reinterpret_cast<graphics_buffers*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HDC hdc_bmp = CreateCompatibleDC(hdc);
            auto old_bmp = SelectObject(hdc_bmp, buffs->front_bmp());

            BitBlt(hdc, 0, 0, buffs->width(), buffs->height(), hdc_bmp, 0, 0, SRCCOPY);
            SelectObject(hdc, old_bmp);
            DeleteDC(hdc_bmp);
            EndPaint(hwnd, &ps);
        }
        break;
        case WM_DESTROY: 
        {
            auto buffs = reinterpret_cast<graphics_buffers*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            delete buffs;
        }
        break;
        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}



