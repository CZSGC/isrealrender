
#include <windows.h>
#include "bitmap.h"
#include "geometry.h"




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
void graphics_buffers::setTriangle(Vec3f a, Vec3f b, Vec3f c, float* zbuffer, uint32_t color) {
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
            if (isInside(a, b, c, Vec2f(i, j))) {
                float z = 0.f;
                Vec3f v = Vec2f::barycentric(a, b, c, Vec2f(i, j));
                z = a.z * v[0] + b.z * v[1] + c.z * v[2];
                if (z > zbuffer[j * wd_ + i]) {
                    zbuffer[j * wd_ + i] = z;
                    setPixel(Vec2f(i, j), color);
                }
            }
        }
    }


}

void graphics_buffers::setLine(Vec2f v0, Vec2f v1, uint32_t pex)
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
            setPixel(Vec2f(y, x), 0xffffffff);
        }
        else {
            setPixel(Vec2f(x, y), 0xffffffff);
        }
        error += derror;
        if (error > dx) {
            y += (y1 > y0) ? 1 : -1;
            error -= 2 * dx;
        }
    }

}

void graphics_buffers::swap() {
    std::swap(front_, back_);
}

void graphics_buffers::setPixel(Vec2f v, uint32_t pix) {
    int x = v.x, y = v.y;
    if (x >= 0 && x < wd_ && y >= 0 && y < hgt_)
    {
        back_.data[(y)*wd_ + x] = pix;
    }
}

void graphics_buffers::clear() {
    std::fill(back_.data, back_.data + size(), 0);
}

graphics_buffers::graphics_buffers(int wd, int hgt) :wd_(wd), hgt_(hgt), front_(create_graphics_buffer(wd, hgt)), back_(create_graphics_buffer(wd, hgt)) {
    clear();
}

graphics_buffers::~graphics_buffers() {
    DeleteObject(front_.hbm);
    DeleteObject(back_.hbm);
}

int graphics_buffers::height() const{
    return hgt_;
}

int graphics_buffers::width() const{
    return wd_;
}

size_t graphics_buffers::size() const {
    return static_cast<size_t>(wd_ * hgt_);
}

HBITMAP graphics_buffers::front_bmp() {
    return front_.hbm;
}