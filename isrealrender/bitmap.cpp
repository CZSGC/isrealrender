
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


void graphics_buffers::swap() 
{
    std::swap(front_, back_);
}



void graphics_buffers::clear() 
{
    std::fill(back_.data, back_.data + size(), 0);
}

graphics_buffers::graphics_buffers(int wd, int hgt) :wd_(wd), hgt_(hgt), front_(create_graphics_buffer(wd, hgt)), back_(create_graphics_buffer(wd, hgt)) 
{
    clear();
}

graphics_buffers::~graphics_buffers() 
{
    DeleteObject(front_.hbm);
    DeleteObject(back_.hbm);
}

int graphics_buffers::height() const
{
    return hgt_;
}

int graphics_buffers::width() const
{
    return wd_;
}

size_t graphics_buffers::size() const 
{
    return static_cast<size_t>(wd_ * hgt_);
}

HBITMAP graphics_buffers::front_bmp() 
{
    return front_.hbm;
}