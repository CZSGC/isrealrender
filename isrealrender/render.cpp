
#include "bitmap.h"


void drawModelLine(graphics_buffers& buffs) {
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
            buffs.setLine(Vec2f(x0, y0), Vec2f(x1, y1), 0xffffffff);
        }
    }
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
        buffs.setPixel(Vec3f(x, y), 0xffffffff);
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

}