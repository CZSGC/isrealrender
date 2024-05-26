#pragma once
#include "bitmap.h"
#include "geometry.h"
void drawModelLine(graphics_buffers& buffs);

void draw_something(graphics_buffers& buffs);

void drawModelFace(graphics_buffers& buffs);

void setPixel(Vec3f v, uint32_t pix, graphics_buffers& buffers);

void setLine(Vec3f v0, Vec3f v1, uint32_t pex, graphics_buffers& buffers);

void setTriangle(Vec3f a, Vec3f b, Vec3f c, std::vector<float> zbuffer, uint32_t color, graphics_buffers* buffers);

Matrix4f cameraTransformation(Vec4f e, Vec4f u, Vec4f g );

void test(graphics_buffers* buffs);