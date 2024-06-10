#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "geometry.h"
#include "model.h"
#include "bitmap.h"
#include "render.h"
#define WIDTH 800
#define HEIGHT 800


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);




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
        0, 0, WIDTH, HEIGHT,

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








LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE: 
        {
            RECT r;
            GetClientRect(hwnd, &r);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(new graphics_buffers(WIDTH, HEIGHT)));
            SetTimer(hwnd, kTimerID, 1, NULL);
        }
        break;
        case WM_TIMER: 
        {
            auto buffs = reinterpret_cast<graphics_buffers*>(GetWindowLongPtr(hwnd, GWLP_USERDATA ));


            test(buffs);
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



