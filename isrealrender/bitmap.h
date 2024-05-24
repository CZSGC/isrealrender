#pragma once
#include <windows.h>
#include <stdint.h>
#include <utility>
#include <algorithm>
#include "geometry.h"
#include "model.h"

constexpr int kTimerID = 101;



struct graphics_buffer
{
    HBITMAP hbm;
    uint32_t* data;
};

graphics_buffer create_graphics_buffer(int wd, int hgt);

class graphics_buffers
{
public:
    graphics_buffer front_;
    graphics_buffer back_;
    int wd_;
    int hgt_;
    graphics_buffers(int wd, int hgt);
    ~graphics_buffers();

    HBITMAP front_bmp();

    size_t size() const;

    int width() const;

    int height() const;

    void clear();

    void swap();



};

